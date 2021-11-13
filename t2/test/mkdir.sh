#!/bin/sh

set -e

./mkdir testresult/sto
[ -d testresult/sto ] || exit 1
rmdir testresult/sto
./mkdir testresult/sto/
[ -d testresult/sto ] || exit 1
