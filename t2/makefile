CC=gcc
CFLAGS=-lreadline -Wall -Werror -Wshadow -fsanitize=address -O0 -g

all:echo exit cd pwd kill ls cat xargs touch mkdir less cp ln mv rm set sh 

echo: sh.c
	$(CC) $(CFLAGS) sh.c -o echo
exit: sh.c
	$(CC) $(CFLAGS) sh.c -o exit
cd: sh.c
	$(CC) $(CFLAGS) sh.c -o cd
pwd: sh.c
	$(CC) $(CFLAGS) sh.c -o pwd
kill: sh.c
	$(CC) $(CFLAGS) sh.c -o kill
ls: sh.c
	$(CC) $(CFLAGS) sh.c -o ls
cat: sh.c
	$(CC) $(CFLAGS) sh.c -o cat
xargs: sh.c
	$(CC) $(CFLAGS) sh.c -o xargs
touch: sh.c
	$(CC) $(CFLAGS) sh.c -o touch
mkdir: sh.c
	$(CC) $(CFLAGS) sh.c -o mkdir
less: sh.c
	$(CC) $(CFLAGS) sh.c -o less
cp: sh.c
	$(CC) $(CFLAGS) sh.c -o cp
ln: sh.c
	$(CC) $(CFLAGS) sh.c -o ln
mv: sh.c
	$(CC) $(CFLAGS) sh.c -o mv
rm: sh.c
	$(CC) $(CFLAGS) sh.c -o rm
set: sh.c
	$(CC) $(CFLAGS) sh.c -o set
sh: sh.c
	$(CC) $(CFLAGS) sh.c -o sh
clean:
	rm -f echo exit cd pwd kill ls cat xargs touch mkdir less cp ln mv rm set sh 
