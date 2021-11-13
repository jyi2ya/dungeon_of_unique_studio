#!/bin/sh

for i in a b c d e f; do
	touch testresult/$i
done

for i in a b c d e f; do
	[ ! -f testresult/$i ] && exit 1
done

exit 0
