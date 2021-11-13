#!/bin/sh

set -e

echo never gonna give you up >> testresult/correct
./echo never gonna give you up >> testresult/result

diff testresult/correct testresult/result
