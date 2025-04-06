#!/bin/bash

if [[ $# -ne 2 ]]; then
    echo "Script must take in exactly 2 arguments"
    exit 1
fi

reg=^[1-9][0-9]*$

if [[ ! $1 =~ $reg || ! $2 =~ $reg ]]; then
    echo "Both arguments must be valid positive numbers"
    exit 2
fi

if [[ $1 -gt $2 ]]; then
    echo "First agrument must be smaller than the second"
    exit 3
fi

mkdir -p ./a ./b ./c
a="./a"; b="./b"; c="./c"

find . -type f ! -name "$(basename $0)" | while IFS= read -r file; do

    lines=$(wc -l < "$file")

    if [[ $lines -lt $1 ]]; then
        mv "$file" "${a}/$(basename "${file}")"
    elif [[ $lines -le $2 ]]; then
        mv "$file" "${b}/$(basename "${file}")"
    else
        mv "$file" "${c}/$(basename "${file}")"
    fi

done

exit 0
