#!/bin/bash

if [[ $# -ne 1 ]]; then
    exit 1
fi

if [[ ! -d "$1" ]]; then
    exit 2
fi

configs="$(find "${1}/cfg" -type f)"
> "${1}/foo.conf"
while IFS= read -r line; do

    stall=$(./${1}/validate.sh "${line}")
    code="$?"
    if [[ "$code" -eq 1 ]]; then
        edited="$(echo "$stall" | awk -v file=$(basename $line) ' { print file": "$0 } ')"
        echo "$edited" >&2
    elif [[ "$code" -eq 0 ]]; then
        echo "$(cat < "$line")" >> "${1}/foo.conf"
        user="$(echo "$(basename "$line")" | rev | cut -c 5- | rev)"

        existingusers="$(cat < "${1}/foo.pwd" | cut -d: -f1)"

        if [[ -z "$(echo "$existingusers" | egrep "${user}")" ]]; then
            normalpas="$(pwgen 16 1)"
            heshpas="$(mkpasswd "${normalpas}")"
            echo "${user}:${normalpas}"
            echo "${user}:${heshpas}" >> "${1}/foo.pwd"
        fi
    fi

done < <(echo "$configs")

exit 0
