#!/bin/bash

if [[ $# -ne 1 ]]; then
    echo "Script must take in exactly 1 argument"
    exit 1
fi

if [[ ! -r $1 || ! -f $1 ]]; then
    echo "Argument must be a readable file"
    exit 2
fi

awk '

    BEGIN { line=1; }

    {
        $3=line".";
        output=$3;
        for (f=4;f<=NF;f++) {
            output=output" "$f;
        }
        print output;
        line++;
    }

'  "$1" | sort -t' ' -k2,2

exit 0
