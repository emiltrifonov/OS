#!/bin/bash

if [[ $# -ne 1 ]]; then
    echo "Script must take in 1 argument"
    exit 1
fi

if [[ ! -f "$1" ]]; then
    echo "Argument must be a file"
    exit 1
fi

top3sites="$(cat < "$1" | cut -d' ' -f2 | sort | uniq -c | head -n 3 | awk ' { print $2 } ')"

while read -r site; do
    echo "${site}:"
    reg="HTTP/2.0"
    queries="$(cat < "$1" | awk -v site=$site ' $2==site  { print $0  } ')"
    echo "HTTP/2.0 queries: "$(echo "$queries" | egrep -c "${reg}")""
    echo "Non-HTTP/2.0 queries: "$(echo "$queries" | egrep -vc "${reg}")""

    echo "$queries" | egrep -o "^.*HTTP/[12]\.[012][[:space:]][0-9]*" | \
       awk ' { if ( $NF > 302 ) print $1; } ' | sort | uniq -c | \
       awk ' { print $2, $1 } '
    echo ""
done < <(echo "$top3sites")

exit 0
