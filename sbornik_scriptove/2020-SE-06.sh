#!/bin/bash

if [[ $# -ne 3 ]]; then
    echo "Script must take in 3 arguments"
    exit 1
fi

if [[ ! -f "$1" ]]; then
    echo "First argument must be a file"
    exit 2
fi

key="$(cat < "$1" | awk -v k="$2" ' { if ( $1==k ) print $1 } ')"
date=$(date)

temp=$(mktemp)

if [[ -z "$key" ]]; then
    line="$(echo ""$2" = "$3" # added at ${date} by ${USER}")"
    echo "$line" >> "$1"
else
    while IFS= read -r line; do
        thiskey="$(echo "$line" | awk ' { print $1 } ')"

        if [[ "$thiskey" == "$key" ]]; then
            modline="$(echo "# ${line} # edited at "${date}" by "${USER}"")"
            newline="$(echo ""${key}" = "$3" # added at "${date}" by "${USER}"")"
            echo "$modline" >> "$temp"
            echo "$newline" >> "$temp"
        else
            echo "$line" >> "$temp"
        fi

    done < <(cat < "$1")

    echo "$(cat < "$temp")" > "$1"
fi

exit 0
