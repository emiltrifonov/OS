#!/bin/bash

# b)
# print smallest number with the maximum sum of its digits

maxsum=-1
temp=$(mktemp)

while read -r line; do

    rline=$(echo "$line" | egrep -o "(|-)(0|[1-9][0-9]*)")

    while read -r num; do

        digits=$(echo "$num" | fold -w 1 | sed '/^-$/d')
        #echo "$digits"

        cursum=$(echo "$digits" | awk ' { sum+=$1 } END {print sum} ')

        #echo "$cursum"

        if [[ "$cursum" -gt "$maxsum" ]]; then
            maxsum="$cursum"
        fi

        echo "${num} ${cursum}" >> "$temp"

    done < <(echo "$rline")

done

cat < "$temp" | awk -v maxsum=$maxsum ' { if ( $2==maxsum ) print $1 } ' | sort -n | head -n 1

exit 0

# a)
# print all unique numbers with maximum absolute value

maxabs=-1
temp=$(mktemp)

while read -r line; do

    rline=$(echo "$line" | egrep -o "(|-)(0|[1-9][0-9]*)")

    echo "$rline" >> "$temp"

    curmaxabs=$(echo "$rline" | awk -v maxabs=$maxabs '
        { if ( $1>maxabs ) maxabs=$1; if ( -$1>maxabs ) maxabs=-$1; } END { print maxabs; } ')

    if [[ "$curmaxabs" -gt "$maxabs" ]]; then
        maxabs=$curmaxabs
    fi

done

cat < "$temp" | sort | uniq | awk -v maxabs=$maxabs ' { if ($1==maxabs || -$1==maxabs) print $1 } '

exit 0
