#if 0
name="sh"
gcc -lreadline -Wall -Werror -Wshadow -fsanitize=address -O0 -g "$name".c -o /tmp/"$name"-$$ || exit 1
/tmp/"$name"-$$ "$@"
ret=$?
rm /tmp/"$name"-$$
exit $ret
#endif

#ifndef __USE_ISOC99
int isblank(int x)
{
	return x == ' ' || x == '\t';
}
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>
#include <pwd.h>

int is_independent_shell = 0;
int exit_flag = 0;
int signal_received = 0;

const char *HOME;

#define X_LESS_BUILTIN_COMMANDS \
	X(command) \
	X(builtin) \
	X(set) \
	X(cd) \
	X(exit)

#define X_MORE_BUILTIN_COMMANDS \
/* basic builtin  */ \
X(echo) \
X(exit) \
X(cd) \
X(pwd) \
/* advanced builtin */ \
X(kill) \
/* basic external */ \
X(ls) \
X(cat) \
X(xargs) \
X(touch) \
X(mkdir) \
/* advanced external */ \
X(less) \
X(cp) \
X(ln) \
X(mv) \
X(rm) \
/* interesting things */ \
X(set) \
X(sh) \
X(command) \
X(builtin)
/* END_OF_X_MORE_BUILTIN_COMMANDS */

#define X_NOFORK_COMMANDS \
	X(set) \
	X(cd) \
	X(exit)

#define X(x) int x ## _main(int argc, char *argv[]);
X_MORE_BUILTIN_COMMANDS
#undef X

int find_command_and_call(int argc, char *argv[])
{
	char *name;

	for (name = argv[0]; *name != '\0'; ++name)
		;
	while (*name != '/' && name != argv[0])
		--name;
	if (*name == '/')
		++name;

#define X(x) if (strcmp(name, #x) == 0) return (x ## _main(argc, argv)) << 8;
	if (is_independent_shell) {
		X_MORE_BUILTIN_COMMANDS
	} else {
		X_LESS_BUILTIN_COMMANDS
	}
#undef X

	return 1;
}

int main(int argc, char *argv[])
{
	is_independent_shell = 1;
	return find_command_and_call(argc, argv) >> 8;
}

void *alloc_or_die(void *p, size_t size)
{
	p = realloc(p, size);
	if (p == NULL) {
		fprintf(stderr, "%s\n", strerror(errno));
		exit(1);
	}
	return p;
}

char *strndup_or_die(char *head, int size)
{
	char *buf = NULL;
	buf = alloc_or_die(buf, size + 1);
	strncpy(buf, head, size);
	buf[size] = '\0';
	return buf;
}

int scan_token(char *line, char ***res)
{
	char *token_start = line;
	char *pos;
	int token_cnt = 0;
	int array_size = 64;
	enum state {
		read_blank, read_word, read_string
       	} state = read_blank;

	*res = NULL;
	*res = alloc_or_die(*res, sizeof((*res)[0]) * array_size);

	for (pos = line; ; ++pos) {
		switch (state) {
			case read_blank:
				switch(*pos) {
					case '<':
					case '>':
					case '|':
					case ';':
					case '&':
						(*res)[token_cnt] = strndup_or_die(pos, 1);
						++token_cnt;
						state = read_blank;
						break;
					case '\0':
						goto scan_over;
						break;
					case '\\':
						if (pos[1] == '\0') {
							(*res)[token_cnt] = NULL;
							puts("Illegal escape sequence");
							goto error;
						}

						/* reserve a '\' to distinguish
						 * escaped characters from operators,
						 * remove before execute.
						 */
						token_start = pos;
						++pos;
						state = read_word;
						break;
					case '\'':
						*pos = '\\';
						token_start = pos;
						state = read_string;
						break;
					default:
						if (!isblank(*pos)) {
							token_start = pos;
							state = read_word;
						}
				}

				break;
			case read_word:
				switch (*pos) {
					case '<':
					case '>':
					case '|':
					case ';':
					case '&':
						/* save token found */
						(*res)[token_cnt] = strndup_or_die(token_start, pos - token_start);
						++token_cnt;

						/* ensure that only 1 or 0 token
						 * be added into array in one
						 * roll */
						--pos;
						state = read_blank;
						break;
					case '\0':
						(*res)[token_cnt] = strndup_or_die(token_start, pos - token_start);
						++token_cnt;
						goto scan_over;
					case '\\':
						memmove(pos, pos + 1, strlen(pos));
						break;
					case '\'':
						memmove(pos, pos + 1, strlen(pos));
						state = read_string;
						break;
					default:
						if (isblank(*pos)) {
							(*res)[token_cnt] = strndup_or_die(token_start, pos - token_start);
							++token_cnt;
							state = read_blank;
						}
				}

				break;
			case read_string:
				switch (*pos) {
					case '\0':
						(*res)[token_cnt] = NULL;
						puts("Syntax error");
						goto error;
					case '\'':
						memmove(pos, pos + 1, strlen(pos));
						--pos;
						state = read_word;
						break;
					default:
						break;
				}
		}

		if (token_cnt == array_size) {
			array_size *= 2;
			*res = alloc_or_die(*res, sizeof((*res)[0]) * array_size);
		}
	}

scan_over:
	(*res)[token_cnt] = NULL;
	return 0;
error:
	return 1;
}

struct cmd_line {
	char *cmd;
	char **paramters;
	char *input;
	char *output;

	int is_pipe;
	int is_background;

	struct cmd_line *next;
};

int create_command_list(char **tokens, struct cmd_line **res)
{
	struct cmd_line **cur = res;
	char **pos;
	enum {
		get_command, get_paramters, get_redirection
	} state;

	char **redirect = NULL;
	int paramters_cnt = 0;
	int paramters_size = 0;

	int require_command = 0;

	*cur = NULL;
	state = get_command;
	for (pos = tokens; ; ++pos) {
		int is_keyword;

		if (*pos != NULL) {
			is_keyword = (strchr("><|;&", **pos) != NULL);
			if (**pos == '~' &&
					((*pos)[1] == '/' || (*pos)[1] == '\0')) {
				char *p = alloc_or_die(NULL,
						strlen(HOME) + strlen(*pos) + 1);
				strcat(strcpy(p, HOME), *pos + 1);
				free(*pos);
				*pos = p;
			}
		}

		switch (state) {
			case get_command:
				if (*pos == NULL) {
					if (require_command) {
						printf("Syntax error %d\n", __LINE__);
						goto error;
					} else {
						goto success;
					}
				}

				if (is_keyword) {
					printf("Syntax error %d\n", __LINE__);
					goto error;
				}

				*cur = alloc_or_die(*cur, sizeof(struct cmd_line));
				(*cur)->cmd = *pos;

				(*cur)->paramters = NULL;
				paramters_size = 8;
				paramters_cnt = 1;
				(*cur)->paramters =
					alloc_or_die((*cur)->paramters, sizeof(char *) * paramters_size);

				(*cur)->paramters[0] = *pos;

				(*cur)->input = NULL;
				(*cur)->output = NULL;
				(*cur)->is_pipe = 0;
				(*cur)->is_background = 0;
				(*cur)->next = NULL;

				redirect = NULL;
				state = get_paramters;

				break;
			case get_paramters:
				if (*pos == NULL || is_keyword) {
					if (*pos == NULL || **pos == '|' ||
							**pos == '&' || **pos == ';') {
						require_command = 0;
						if (*pos == NULL) {
							;
						} else if (**pos == '|') {
							require_command = 1;
							(*cur)->is_pipe = 1;
						} else if (**pos == '&') {
							(*cur)->is_background = 1;
						}

						(*cur)->paramters =
							alloc_or_die((*cur)->paramters, sizeof(char *) * (paramters_cnt + 1));
						(*cur)->paramters[paramters_cnt] = NULL;
						cur = &((*cur)->next);

						state = get_command;

						if (*pos == NULL)
							goto success;
					} else {
						redirect = (**pos == '<') ?
							&((*cur)->input) :
							&((*cur)->output);
						state = get_redirection;
					}
				} else {
					(*cur)->paramters[paramters_cnt++] = *pos;
					if (paramters_cnt == paramters_size) {
						paramters_size *= 2;
						(*cur)->paramters = alloc_or_die((*cur)->paramters, sizeof(char *) * paramters_size);
					}
				}
				break;
			case get_redirection:
				if (*pos == NULL) {
					printf("Syntax error %d\n", __LINE__);
					goto error;
				}

				if (is_keyword) {
					printf("Syntax error %d\n", __LINE__);
					goto error;
				}

				/* mark to make it deleted in success: */
				if (*redirect != NULL)
					**redirect = '&';

				*redirect = *pos;
				state = get_paramters;
				break;
			default:
				puts("Fuck");
		}
	}

success:
	for (pos = tokens; *pos != NULL; ++pos) {
		if (strchr("><|;&", **pos) != NULL) {
			free(*pos);
		} else if (**pos == '\\') {
			memmove(*pos, *pos + 1, strlen(*pos));
		}
	}
	return 0;
error:
	return 1;
}

int parse_cmd(char *cmd, struct cmd_line **res)
{
	char **tokens = NULL;
	int errcode = 0;
	char **p;

	*res = NULL;
	if ((errcode = scan_token(cmd, &tokens)) != 0)
		goto free_tokens;

	/* all spaces and tabs */
	if (*tokens == NULL)
		goto free_tokens;

#ifdef DEBUG
	for (p = tokens; *p != NULL; ++p)
		printf("[%s], ", *p);
	putchar('\n');
#endif

	if ((errcode = create_command_list(tokens, res)) != 0)
		goto free_all;

	free(tokens);
	return 0;

free_all:
	while (*res != NULL) {
		struct cmd_line *next = (*res)->next;
		free((*res)->paramters);
		free(*res);
		*res = next;
	}

free_tokens:
	for (p = tokens; *p != NULL; ++p)
		free(*p);
	free(tokens);

	return errcode;
}

int is_nofork_command(char *cmd)
{
#define X(c) if (strcmp(cmd, #c) == 0) return 1;
	X_NOFORK_COMMANDS
#undef X

	return 0;
}

int execute_single_command(struct cmd_line *cmd)
{
	int cnt = 0;
	int ret;

	while (cmd->paramters[cnt] != NULL)
		++cnt;
	ret = find_command_and_call(cnt, cmd->paramters);
	if ((ret & 0xff) == 0)
		return ret;
	execvp(cmd->cmd, cmd->paramters);

	printf("%s not found\n", cmd->cmd);
	return 127;
}

int execute_commands(struct cmd_line *cmds)
{
	struct cmd_line *cur, *last;
	int status;

	for (cur = cmds, last = cmds; cur != NULL; cur = cur->next) {
		if (!cur->is_pipe) {
			struct pid_list {
				int pid;
				struct pid_list *next;
			};
			struct pid_list *pid_to_wait = NULL;
			struct pid_list **p = &pid_to_wait;

			int last_pipe[2] = { -1, -1 };
			int cur_pipe[2] = { -1, -1 };

			if (last == cur && is_nofork_command(cur->cmd)) {
				status = execute_single_command(cur) << 8;
				continue;
			}

			for (; last != cur->next; last = last->next) {
				int pid;

				if (last != cur) {
					pipe(cur_pipe);
				} else {
					cur_pipe[0] = -1;
					cur_pipe[1] = -1;
				}

				pid = fork();
				if (pid == -1) {
					puts("error");
				} else if (pid == 0) {
					if (last_pipe[0] >= 0) {
						close(last_pipe[1]);
						dup2(last_pipe[0], STDIN_FILENO);
					}
					if (cur_pipe[0] >= 0) {
						close(cur_pipe[0]);
						dup2(cur_pipe[1], STDOUT_FILENO);
					}
					if (last->input != NULL) {
						int fd = open(last->input, O_RDONLY);
						if (fd == -1) {
							perror("sh: ");
							exit(1);
						}
						dup2(fd, STDIN_FILENO);
					}
					if (last->output != NULL) {
						int fd = open(last->output, O_WRONLY | O_CREAT | O_TRUNC, 0644);
						if (fd == -1) {
							perror("sh: ");
							exit(1);
						}
						dup2(fd, STDOUT_FILENO);
					}
					signal(SIGINT, SIG_DFL);
					signal(SIGQUIT, SIG_DFL);
					exit(execute_single_command(last));
				} else {
					if (!cur->is_background) {
						*p = alloc_or_die(NULL, sizeof(struct pid_list));
						(*p)->next = NULL;
						(*p)->pid = pid;
						p = &((*p)->next);
					}
					if (last_pipe[0] >= 0) {
						close(last_pipe[0]);
						close(last_pipe[1]);
					}
					last_pipe[0] = cur_pipe[0];
					last_pipe[1] = cur_pipe[1];
				}
			}

			while (pid_to_wait != NULL) {
				struct pid_list *next = pid_to_wait->next;

				while (waitpid(pid_to_wait->pid, &status, 0) == -1)
					if (errno != EINTR)
						break;
				free(pid_to_wait);
				pid_to_wait = next;
			}
		}
	}

	return (unsigned int)status >> 8;
}

char *sh_readline(char *prompt)
{
	return readline(prompt);
}

void signal_handler(int sig)
{
	signal_received = 1;
}

int sh_main(int argc, char *argv[])
{
	int line_cnt = 0;
	int return_value = 0;

	is_independent_shell = 0;

	if ((HOME = getenv("HOME")) == NULL)
		HOME = getpwuid(getuid())->pw_dir;

	signal(SIGINT, signal_handler);
	signal(SIGQUIT, signal_handler);

	while (!exit_flag) {
		int errcode = 0;
		char PS1[1024];
		char *line;
		struct cmd_line *cmds;

		if (signal_received) {
			signal_received = 0;
			return_value = 130;
			putchar('\n');
			/* beautify */
		}
		sprintf(PS1, "%-4d[%-3d] sh $ ", line_cnt, return_value);
		line = sh_readline(PS1);

		/* process ctrl-d */
		if (line == NULL) {
			puts("exit");
			break;
		}

		add_history(line);

		cmds = NULL;
		errcode = parse_cmd(line, &cmds);
		free(line);

		if (errcode == 0) {
#ifdef DEBUG
			struct cmd_line *p;
			for (p = cmds; p != NULL; p = p->next) {
				char **c;
				puts("========");
				printf("prog: %s\n", p->cmd);
				printf("args:");
				for (c = p->paramters; *c != NULL; ++c)
					printf(" %s", *c);
				putchar('\n');
				if (p->input != NULL)
					printf("in  : %s\n", p->input);
				if (p->output != NULL)
					printf("out : %s\n", p->output);
				printf("attr: %s%s\n", p->is_pipe ? "Pipe " : "",
						p->is_background ? "Background" : "");
			}
#endif
			return_value = execute_commands(cmds);
		}

		while (cmds != NULL) {
			struct cmd_line *next = cmds->next;
			char **c;

			/*
			 * cmd is paramter[0], free later
			 free(cmds->cmd);
			 */

			for (c = cmds->paramters; *c != NULL; ++c)
				free(*c);
			free(cmds->paramters);

			if (cmds->input != NULL)
				free(cmds->input);
			if (cmds->output != NULL)
				free(cmds->output);

			free(cmds);

			cmds = next;
		}

		++line_cnt;
	}

	return 0;
}

int echo_main(int argc, char *argv[])
{
	while (*++argv != NULL)
		printf("%s%c", *argv, argv[1] == NULL ? '\n' : ' ');
	return 0;
}

int ls_main(int argc, char *argv[])
{
	puts("ls: not implemented yet");
	return 0;
}

int cat_copy_stream_to_stdout(FILE *stream)
{
	int ch;

	errno = 0;

	while ((ch = fgetc(stream)) != EOF)
		if (putchar(ch) == EOF)
			break;

	if (errno) {
		perror("cat");
		return 1;
	}

	return 0;
}

int cat_main(int argc, char *argv[])
{
	int file_specified = 0;
	int error_occured = 0;

	while (*++argv != NULL) {
		if (strcmp(*argv, "-u") == 0) {
			setvbuf(stdout, NULL, _IONBF, 0);
			continue;
		} else if (strcmp(*argv, "-") == 0) {
			file_specified = 1;

			if (cat_copy_stream_to_stdout(stdin))
				error_occured = 1;
		} else {
			FILE *in = fopen(*argv, "r");
			file_specified = 1;

			if (in != NULL) {
				if (cat_copy_stream_to_stdout(in))
					error_occured = 1;
				fclose(in);
			} else {
				error_occured = 1;
				perror("cat");
			}
		}
	}

	if (!file_specified)
		if (cat_copy_stream_to_stdout(stdin))
			error_occured = 1;

	if (error_occured)
		return 1;
	return 0;
}

int less_main(int argc, char *argv[])
{
	puts("less: not implemented yet");
	return 0;
}

int xargs_main(int argc, char *argv[])
{
	puts("xargs: not implemented yet");
	return 0;
}

int touch_main(int argc, char *argv[])
{
	puts("touch: not implemented yet");
	return 0;
}

int mkdir_main(int argc, char *argv[])
{
	puts("mkdir: not implemented yet");
	return 0;
}

int cp_main(int argc, char *argv[])
{
	puts("cp: not implemented yet");
	return 0;
}

int mv_main(int argc, char *argv[])
{
	puts("mv: not implemented yet");
	return 0;
}

int rm_main(int argc, char *argv[])
{
	puts("rm: not implemented yet");
	return 0;
}

int ln_main(int argc, char *argv[])
{
	puts("ln: not implemented yet");
	return 0;
}

int cd_main(int argc, char *argv[])
{
	if (argv[1] == NULL) {
		puts("Too few arguments");
		return 1;
	}
	return chdir(argv[1]);
}

int exit_main(int argc, char *argv[])
{
	exit_flag = 1;
	return 0;
}

int set_main(int argc, char *argv[])
{
	if (argv[1] == NULL) {
		puts("Too few arguments");
		return 1;
	}
	if (*argv[1] == '+')
		is_independent_shell = 1;
	if (*argv[1] == '-')
		is_independent_shell = 0;
	return 0;
}

int kill_main(int argc, char *argv[])
{
	puts("kill: not implemented yet");
	return 1;
}

int pwd_main(int argc, char *argv[])
{
	int max = pathconf(".", _PC_PATH_MAX);
	char *p;
	char *buf;
	if (max == -1)
		max = 4096;
	buf = alloc_or_die(NULL, max);
	p = getcwd(buf, max);
	if (p == NULL) {
		perror("pwd: ");
		return 1;
	} else {
		printf("%s\n", buf);
	}
	free(buf);
	return 0;
}

int command_main(int argc, char *argv[])
{
	if (argv[1] == NULL) {
		puts("Syntax error");
		return 1;
	}

	execvp(argv[1], argv + 1);
	printf("%s not found\n", argv[1]);
	return 1;
}

int builtin_main(int argc, char *argv[])
{
	int old_is_independent_shell = is_independent_shell;
	int ret;
	if (argv[1] == NULL) {
		puts("Syntax error");
		return 1;
	}

	is_independent_shell = 1;
	ret = find_command_and_call(argc - 1, argv + 1);
	is_independent_shell = old_is_independent_shell;

	if ((ret & 0xff) != 0) {
		printf("%s not found\n", argv[1]);
		return 1;
	}

	return ret >> 8;
}
