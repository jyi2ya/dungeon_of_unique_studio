#!/bin/sh

set -e

./ln test/a.txt testresult/a.txt
[ "$(stat -c '%i' test/a.txt)" = "$(stat -c '%i' testresult/a.txt)" ] || exit 1
rm -f testresult/a.txt

./ln test/a.txt testresult/
[ "$(stat -c '%i' test/a.txt)" = "$(stat -c '%i' testresult/a.txt)" ] || exit 1
rm -f testresult/a.txt

./ln test/a.txt testresult
[ "$(stat -c '%i' test/a.txt)" = "$(stat -c '%i' testresult/a.txt)" ] || exit 1
