#!/bin/bash

if [[ $# -ne 1 ]]; then
    echo "Script must take in exactly one argument"
    exit 1
fi

if [[ ! -d $1 ]]; then
    echo "Argument must be a directory"
    exit 2
fi


# dir file
friends1=$(find logs -mindepth 3 -maxdepth 3 -type d -printf "%p %f\n")
friends=""
#echo "$friends1"

logs=""

while IFS= read -r line; do
    logs=$logs$'\n'
    dir=$(echo "$line"| cut -d' ' -f1)
    file=$(echo "$line" | cut -d' ' -f2)
    lines=$(find "$dir" -mindepth 1 -maxdepth 1 -type f -exec wc -l {} \; )
    logs=${logs}$(echo "$lines" | tr ' ' '/')

done < <(echo "$friends1")

logs=$(echo "$logs" | tail -n +2)

echo "$logs" | awk -F/ ' { print $5" "$1; } ' |  awk ' {

    lines[$1]+=$2

}

END {

    for (u in lines)
        print u" "lines[u]

} ' | sort -t' ' -k2,2 -nr | head -n 10

exit 0
