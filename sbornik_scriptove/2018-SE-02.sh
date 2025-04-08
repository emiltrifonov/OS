#!/bin/bash

if [[ $# -ne 2 ]]; then
    echo "Script must take in 2 arguments"
    exit 1
fi

if [[ ! -f $1 ]]; then
    echo "First argument must be a file"
    exit 2
fi

if [[ ! -d $2 ]]; then
    echo "Second argument must be a directory"
    exit 3
fi

namereg="^[a-zA-Z]*[[:space:]]+[a-zA-Z]*"

names1=$(egrep -o "$namereg" "$1" | tr -s ' ' | sort | uniq)

names=$(echo "$names1" | awk ' BEGIN {cnt=1;}  { print $0";"cnt; cnt++; }')

touch "$2"/dict.txt
dict=$2"/dict.txt"

echo "$names" > "$dict"

while IFS= read -r line; do
    echo "$line"

    name1=$(echo "$line" | awk -F';' ' { print $1}')
    num=$(echo "$line" | awk -F';' ' {print $2} ')
    file=$2"/$num"
    echo "$file"
    while IFS= read -r inputline; do
        name2=$(echo "$inputline" | egrep -o "$namereg" | tr -s ' ')

        if [[ "$name2" == "$name1" ]]; then
            echo "$inputline" >> "$file"
        fi

    done < <(cat < "$1")

done < <(cat < "$dict")

exit 0
