#!/bin/sh

for i in a b c d e f g; do
	touch testresult/$i
done

for i in a b c d e f g; do
	./rm testresult/$i
done

for i in a b c d e f g; do
	[ -f testresult/$i ] && exit 1
done

exit 0
