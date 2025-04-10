#!/bin/bash

if [[ $# -ne 2 ]]; then
    exit 1
fi

if [[ ! -f "$1" ]]; then
    exit 2
fi

file="$(cat < "$1" | awk -F, ' { print $1","$4","$5","$7"," } ' | sed 's/,$//' )"

constel="$(echo "$file" | awk -F, -v type="$2" ' BEGIN { max=0;name="" } {

    if ($3==type) con[$2]++;
    if (con[$2]>max) { max=con[$2];name=$2; }

}

END {
    print name;
}

')"

echo "$file" | awk -F, -v type=$2 ' $3==type { print $0 } ' | sort -t, -k4,4 -n | head -n 1 | awk -F, ' { print $1 } '

exit 0
