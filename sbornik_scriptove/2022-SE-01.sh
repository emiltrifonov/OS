#!/bin/bash

if [[ $# -ne 1 ]]; then
    echo "Script must take in 1 argument"
    exit 1
fi

if [[ ! -f "$1" ]]; then
    echo "Argument must be a file"
    exit 2
fi

alldevices="$(cat < "example-wakeup" | awk ' { print $1 } ' | tail -n +2)"
temp=$(mktemp)
while IFS= read -r line; do

    if [[ -n "$(echo "$line" | egrep "^#")" ]]; then
        echo "$line" >> "$temp"
        continue
    fi

    dev="$(echo "$line" | awk ' { print $1 } ')"

    if [[ -z "$(echo "$alldevices" | egrep "$dev")" ]]; then
        echo "${line} # Non-existent device" >> "$temp"
    else
        echo "$line" >> "$temp"
    fi

done < <(cat < "$1")

cat < "$temp"

exit 0
