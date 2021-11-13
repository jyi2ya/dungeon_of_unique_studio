#!/bin/sh

set -e

time=$(date +%s)
while [ "$(date +%s)" = "$time" ]; do :; done
sleep 0.1

script -c 'ls -a -w 80' testresult/correct.tab >/dev/null 2>&1
expand testresult/correct.tab > testresult/correct
script -c './ls' testresult/result >/dev/null 2>&1

diff testresult/correct testresult/result
