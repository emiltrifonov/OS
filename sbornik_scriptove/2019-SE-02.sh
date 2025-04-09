#!/bin/bash

N=10
ind=1

if [[ -z "$(echo "$1" | egrep "^.*\.log$")" ]]; then
    N=$2
    ind=3
fi

args=$(echo "$@" | tr ' ' '\n' | awk -F' ' -v ind=$ind ' { cnt++; if ( cnt >= ind ) print $0 } ' )
temp=$(mktemp)
while read -r arg; do

    idf=$(echo "$arg" | cut -d. -f1)
    file=$(cat "$arg")

    echo "$(echo "$file" | tail -n "${N}" | awk -F' ' -v idf=$idf ' { $2=$2" "idf; print $0 } ')" >> "$temp"

done < <(echo "${args}")

cat "$temp" | sort

exit 0
