#!/bin/bash

if [[ $# -eq 0 ]]; then
    exit 1
fi

file="$(echo "$@" | egrep -o "[[:space:]][^-][^[:space:]]*" | tr -d ' ')"
cat < "$file"
temp=$(mktemp)
echo "$(cat < "$file")" > "$temp"
for arg in "$@"; do
    if [[ "$arg" == "$file" ]]; then
        continue
    fi

    old="$(echo "$arg" | cut -c 3- | cut -d'=' -f1)"
    new="$(echo "$arg" | cut -c 3- | cut -d'=' -f2)"

    sed -Ei "s/(^| )${old}( |$)/\1${new}$(pwgen 64 1)\2/g" "$temp"

done

cat <"$temp" | sed -E "s/(^| )([^[:space:]]*)[[:alnum:]]{64}( |$)/\1\2\3/g"

exit 0
