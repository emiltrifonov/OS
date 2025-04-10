#!/bin/bash

if [[ $# -ne 2 ]]; then
    exit 1
fi

if [[ ! -d "$1" || ! -d "$2" ]]; then
    exit 2
fi

if [[ -n "$(find "$2" -mindepth 1 -printf "%p\n")" ]]; then
    exit 3
fi

#cleaned_name( full_name (if is swp))
files="$(find "$1" -type f -printf "%p\n" | sed -E "s:(.*\/).(.*)\.swp:\1\2 \0:")"
while IFS= read -r line; do
    if [[ -z "$(echo "$line" | awk ' { print $2 } ')" ]]; then
        relpath="$(realpath --relative-to="${1}" "$line")"
        echo "$relpath"
        cd "${1}"
        cp --parents "${relpath}" "../${2}"
        cd ~
    else
        cleaned="$(echo "$line" | awk ' { print $1 } ')"
        full="$(echo "$line" | awk ' { print $2 } ')"

        count="$(echo "$files" | awk -v cleaned=$cleaned ' { if ($1==cleaned) print $1 } ' | uniq -c | awk ' { print $1 } ')"

        if [[ "$count" -eq 1 ]]; then
            relpath="$(realpath --relative-to="${1}" "$full")"
            echo "$relpath"
            cd "${1}"
            cp --parents "${relpath}" "../${2}"
            cd ~
        fi
    fi


done < <(echo "$files")

exit 0
