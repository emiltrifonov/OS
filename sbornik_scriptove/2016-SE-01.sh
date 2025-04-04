#!/bin/bash

if [[ $# -ne 1 ]]; then
    echo "Script takes exactly 1 parameter"
    exit 1
fi

if [[ ! -d "$1" ]]; then
    echo "Parameter has to be a directory"
    exit 2
fi

dir=$1

# type -l for symlinks
symlinks=$(find $dir -type l)
for sl in $symlinks; do
    if [[ -e $sl ]]; then
        echo $sl
    fi
done
