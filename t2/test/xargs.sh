#!/bin/sh

cd testresult
seq 1 9 | ../xargs touch
for i in `seq 1 9`; do
	[ ! -f $i ] && exit
done

exit 0
