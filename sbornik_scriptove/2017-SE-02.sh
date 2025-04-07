#!/bin/bash

if [[ $(id -u) -ne 0 ]]; then
    echo "Script must be run by root"
    exit 1
fi

if [[ $# -ne 3 ]]; then
    echo "Script must take in exactly 3 parameters"
    exit 2
fi

if [[ ! -d $1 || ! -r $1 ]]; then
    echo "First agrument must be a readable directory"
    exit 3
fi

if [[ ! -d $2 ]]; then
    echo "Second argument must be a directory"
    echo 4
fi

if [[ -n "$(find "$2" -mindepth 1 -print)" ]]; then
    echo "Second argument must be an empty directory"
    exit 5
fi

files=$(find "$1" -type f -name "*${3}*")

while IFS= read -r file; do

    cp --parents "$file" "$2"
    rm "$file"

done < <(echo "$files")

exit 0
