#!/bin/sh

set -e

cat test/a.txt - test/a.txt < test/b.txt >> testresult/correct
./cat test/a.txt - test/a.txt < test/b.txt >> testresult/result

diff testresult/correct testresult/result
