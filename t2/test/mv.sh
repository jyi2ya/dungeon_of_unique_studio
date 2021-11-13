#!/bin/sh

set -e

cp test/a.txt testresult/

./mv testresult/a.txt testresult/orz
[ ! -f testresult/orz ] && exit 1
[ -f testresult/a.txt ] && exit 1

mkdir testresult/o
./mv testresult/orz testresult/o/
[ ! -f testresult/o/orz ] && exit 1
[ -f testresult/o ] && exit 1

./mv testresult/o/orz testresult
[ ! -f testresult/orz ] && exit 1
[ -f testresult/o/orz ] && exit 1

exit 0
