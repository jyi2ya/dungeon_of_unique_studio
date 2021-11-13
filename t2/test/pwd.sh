#!/bin/sh

pwd="$(pwd)/pwd"
mypwd()
{
	eval "$pwd"
}

while read -r path; do
	cd "$path"
	lines="$(printf '%s\n%s\n' "$(mypwd)" "$(pwd)" | sort | uniq | wc -l)"
	[ "$lines" -ne 1 ] && exit 1
done <<'EOF'
/
/usr/bin/
/usr/local/bin
EOF

exit 0
