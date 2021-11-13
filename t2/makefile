CC=gcc
CFLAGS=-Wall -Werror -Wshadow -fsanitize=address -O0 -g

all:echo pwd kill ls cat xargs touch mkdir less cp ln mv rm sh

echo: echo.c
	$(CC) $(CFLAGS) echo.c -o echo
pwd: pwd.c util.o
	$(CC) $(CFLAGS) util.o pwd.c -o pwd
kill: kill.c
	$(CC) $(CFLAGS) kill.c -o kill
ls: ls.c util.o
	$(CC) $(CFLAGS) util.o ls.c -o ls
cat: cat.c
	$(CC) $(CFLAGS) cat.c -o cat
xargs: xargs.c util.o
	$(CC) $(CFLAGS) util.o xargs.c -o xargs
touch: touch.c
	$(CC) $(CFLAGS) touch.c -o touch
mkdir: mkdir.c
	$(CC) $(CFLAGS) mkdir.c -o mkdir
less: less.c
	$(CC) $(CFLAGS) less.c -o less
cp: cp.c util.o
	$(CC) $(CFLAGS) util.o cp.c -o cp
ln: ln.c util.o
	$(CC) $(CFLAGS) util.o ln.c -o ln
mv: mv.c util.o
	$(CC) $(CFLAGS) util.o mv.c -o mv
rm: rm.c
	$(CC) $(CFLAGS) rm.c -o rm
sh: sh.c util.o
	$(CC) $(CFLAGS) -lreadline util.o sh.c -o sh
util.o: util.c util.h
	$(CC) $(CFLAGS) -c util.c

clean:
	rm -f echo exit kill ls cat xargs touch mkdir less cp ln mv rm sh util.o
