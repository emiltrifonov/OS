#!/bin/bash

if [[ $# -ne 3 ]]; then
    echo "Script must take in 3 arguments"
    exit 1
fi

if [[ ! -f "$1" ]]; then
    echo "First argument must be a file"
    exit 2
fi

if [[ ! -d "$3" ]]; then
    echo "Third argument must be a directory"
    exit 3
fi

if [[ ! -f "$2" ]]; then
    echo "Second argument must be a file"
    exit 4
fi

files=$(find "$3" -type f)

reg0="^Line [0-9]*:"
reg1="${reg0}#"
reg2="${reg0}\{[[:space:]]{1}[^[:space:]]*(;|)[[:space:]]{1}\};"
allusers="$(cat < "$1" | awk -F: ' {print $1} ')"
while read -r file; do
    content="$(cat < "$file" | awk ' { cnt++; print "Line "cnt":"$0; } ')"
    invalid="$(echo "$content" | egrep -v "(${reg1}|${reg2}|${reg0}$)")"

    if [[ -n "$invalid" ]]; then
        echo "Error in $(basename "$file"):"
        echo "$invalid"
    else
        user="$(echo "$(basename "$file")" | rev | cut -c 5- | rev)"

        if [[ -z "$(echo "$allusers" | egrep "$user")" ]]; then
            pas=$(pwgen 16 1)
            echo "${user} ${pas}"
            echo "${user}:${pas}" >> "$1"
        fi

    fi

done < <(echo "$files")

exit 0
