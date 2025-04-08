#!/bin/bash

if [[ $# -ne 2 ]]; then
    echo "Script must take in exactly 2 parameters"
    exit 1
fi

if [[ ! -d $1 ]]; then
    echo "First argument must be a directory"
    exit 2
fi

if [[ -z $2 ]]; then
    echo "Second argument must be a non-empty string"
fi

files=$(find "$1" -maxdepth 1 -type f -printf "%f\n" | egrep "^vmlinuz-[0-9]*\.[0-9]*\.[0-9]*-${2}")

echo "$files" | sort -nr | head -n 1

exit 0
