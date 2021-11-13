#!/bin/sh

set -e

for t in test/*.sh; do
	rm -rf testresult
	mkdir -p testresult

	[ "$t" = "test/test.sh" ] && continue
	if ! sh "$t"; then
	       	echo "$t failed"
		exit 1
	fi
done
