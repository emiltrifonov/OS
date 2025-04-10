#!/bin/bash

if [[ $# -ne 1 ]]; then
    exit 1
fi

dev="$1"

status="$(cat < "file" | egrep "${dev}" | awk ' { print $3 } ')"

if [[ "$status" == "*enabled" ]]; then
    sed -Ei "s/^([[:space:]]*${dev})(.*)\*enabled(.*)$/\1\2*disabled\3/g" file
else
    sed -Ei "s/^([[:space:]]*${dev})(.*)\*disabled(.*)$/\1\2*enabled\3/g" file
fi

exit 0
