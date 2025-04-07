#!/bin/bash

if [[ $# -ne 3 ]]; then
    echo "Script must take in 3 arguments"
    exit 1
fi

if [[ ! -f $1 || ! -r $1 ]]; then
    echo "First argument must be a readable file"
    exit 2
fi

#entire file
file=$(awk '{print $0}' "$1")

#first row with matching key
row1=$(awk -F '=' -v key1="$2" ' { if ( $1 == key1 ) { print $0 } } ' "$1")

#potentially second row with matching key
row2=$(awk -F '=' -v key2="$3" ' { if ( $1 == key2 ) { print $0 } } ' "$1")

#test in case row with key2 doesn't exist
if [[ -z "$row2" ]]; then
    echo "$file"
    exit 3
fi

newrow=$(echo "$row2" | cut -d'=' -f1)"="

for term2 in $(echo "$row2" | cut -d'=' -f2); do
    dupl=0
    for term1 in $(echo "$row1" | cut -d'=' -f2); do
        if [[ $term1 == $term2 ]]; then
            dupl=1
            break
        fi
    done

    if [[ $dupl -eq 0 ]]; then
        newrow=$newrow$term2
        if [[ $term2 != $(echo "$row2" | awk '{print $NF}') ]]; then
        newrow=$newrow" "
        fi
    fi

done

while IFS= read -r line; do
    key=$(echo "$line" | cut -d'=' -f1)

    if [[ "$key" == $(echo "$row2" | cut -d'=' -f1) ]]; then
        echo "$newrow"
    else
        echo "$line"
    fi

done < <(echo "$file")

exit 0
