#!/bin/sh

get_cmd_list()
{
	sed -n '/define X_MORE_BUILTIN_COMMANDS/,/END_OF_X_MORE_BUILTIN_COMMANDS/p' \
		< huge_sh.c | awk 'NR > 1' | sed 's#/\*.*\*/##g;s#\\##g' \
		| tr -d ' \n' | sed 's/X(\([^)]*\))/\1 /g'
}

cat > makefile <<EOF
CC=gcc
CFLAGS=-lreadline -Wall -Werror -Wshadow -fsanitize=address -O0 -g

all:`get_cmd_list`

EOF

get_cmd_list | tr ' ' '\n' | while read -r cmd; do
		cat >> makefile <<EOF
$cmd: $cmd.c
	\$(CC) \$(CFLAGS) $cmd.c -o $cmd
EOF
done

cat >> makefile <<EOF
clean:
	rm -f `get_cmd_list`
EOF
