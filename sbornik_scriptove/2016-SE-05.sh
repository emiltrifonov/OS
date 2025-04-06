#!/bin/bash

if [[ $# -ne 2 ]]; then
    echo "Script must take in exactly 2 arguments"
    exit 1
fi


if [[ ! -f $1 || ! -r $1 || ! -f $2 || ! -r $2 ]]; then
    echo "Both arguments must be readable regular files"
    exit 2
fi

file="$(basename $1)"

matches1=$(egrep -c "$(basename $1)" $1)
matches2=$(egrep -c "$(basename $2)" $2)

if [[ $matches1 -eq $matches2 ]]; then
    echo "Kofti failove brato"
    exit 3
elif [[ $matches2 -gt $matches1 ]]; then
    file="$(basename $2)"
fi

echo "$(cut -d' ' -f4- "$file" | sort)"

exit 0
