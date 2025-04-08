#!/bin/bash

if [[ $# -eq 0 ]]; then
    echo "There must be at least 1 argument"
    exit 1
fi

if [[ ! -d $1 ]]; then
    echo "First agrument must be a directory"
    exit 2
fi

links=$(find "$1" -type l -printf "%p\n" 2>/dev/null)

output=""
broken=0

while IFS= read -r link; do

    if [[ ! -e "$link" ]]; then
        broken=$(($broken+1))
    else
        linkpath=$(realpath -s $link)
        realpath=$(realpath $link)
        output=${output}$(realpath -s $link)" -> "$(realpath $link)$'\n'
    fi

done < <(echo "$links")

output=${output}${broken}' broken links'

if [[ $# -eq 2 && ! -f $2 ]]; then
    echo "$output" > "$2"
else
    echo "$output"
fi

exit 0
