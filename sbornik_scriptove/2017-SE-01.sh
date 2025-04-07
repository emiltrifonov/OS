#!/bin/bash

if [[ $# -eq 0 ]]; then
    echo "Scripts must take in 1(or 2) arguments"
    exit 1
fi

if [[ ! -d $1 || ! -r $1 ]]; then
    echo "First argument must be a readable directory"
    exit 2
fi

#if there is a second argument
if [[ -n $2 ]]; then

    files=$(find "$1" -type f -printf "%n %p\n" 2>/dev/null | sort -nr)
    echo "$(echo "$files" | awk -v minlinks=$2 ' { if ( $1 < minlinks ) exit; else print $2; } ' )"

#if there isn't
else

    slinks=$(find "$1" -type l -printf "%p\n" 2>/dev/null)

    while IFS= read -r link; do

        #echo "$link"

        if [[ ! -e $link ]]; then
            echo "$link"
        fi

    done < <(echo "$slinks")

fi

exit 0
