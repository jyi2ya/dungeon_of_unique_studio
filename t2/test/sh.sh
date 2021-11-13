#!/bin/sh

./sh <<EOF
echo testing buitlin commands
echo export:
export PATH=
ls
export PATH=/bin:/usr/bin:/usr/local/bin
ls
export PATH=$PWD
ls
echo testing cd
cd testresult
pwd
export PATH=/bin:/usr/bin:/usr/local/bin
echo testing pipe and redirection
cat<../echo.c|tr \\& \\>|tr '>' '<' >'|'
cat './|'
cat < ../echo.c | tr \\& \\> | tr '>' '<' > '\\'
diff '|' '\\'
rm \\| \\\\
EOF
