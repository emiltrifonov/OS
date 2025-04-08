#!/bin/bash

if [[ $# -ne 2 ]]; then
    echo "Script must take in exactly 2 arguments"
    exit 1
fi

if [[ ! -f $1 ]]; then
    echo "First argument must be a file"
    exit 2
fi

if [[ ! -f $2 ]]; then
    echo "Second argument must be a file"
    exit 3
fi

data=$(cat < "$1")
> "$2"
touch temp
temp=temp

while IFS= read -r line; do

    num1=$(echo "$line" | awk -F',' ' {print $1}')
    other1=$(echo "$line" | cut -d, -f2-)
    duplicate=0
    minnum=$num1

    while IFS= read -r line2; do

        num2=$(echo "$line2" | cut -d, -f1)

        if [[ "$num1" -eq "$num2" ]]; then
            continue
        fi

        other2=$(echo "$line2" | cut -d, -f2-)

        if [[ "$other1" == "$other2" ]]; then
            duplicate=1
            if [[ "$num2" -lt "$minnum" ]]; then
                minnum=$num2
            fi
        fi

    done < <(echo "$data")

    if [[ "$duplicate" -eq 1 ]]; then
        echo "${minnum},${other1}" >> "$temp"
    else
        echo "$line" >> "$2"
    fi

done < <(echo "$data")

echo "$(cat < "$temp" | sort | uniq)" > "$temp"

echo "$(cat < "$temp")" > "$1"

rm "$temp"

exit 0
