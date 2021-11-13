#!/bin/sh

set -e

cp test/a.txt testresult/orz
./cp test/a.txt testresult/sto

diff testresult/orz testresult/sto || exit 1

cp test/a.txt testresult/
mv testresult/a.txt testresult/orz
./cp test/a.txt testresult/

diff testresult/a.txt testresult/orz || exit 1

cp test/a.txt testresult
mv testresult/a.txt testresult/orz
./cp test/a.txt testresult

diff testresult/a.txt testresult/orz || exit 1
