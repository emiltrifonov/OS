#!/bin/bash

count=0
temp=$(mktemp)

while true; do
    valid=0

    prcs="$(ps -eo rss=,comm=)"

    #comm,sum_rss
    sums="$(echo "$prcs" | awk ' { mem[$2]+=$1 } END { for (com in mem) print com,mem[com] } ')"

    maxrss="$(echo "$sums" | sort -t' ' -k2,2 -nr | head -n 1 | awk ' { print $2 } ')"

    if [[ "$maxrss" -le 65536 ]]; then
        valid=1
    else
        invalid_processes="$(echo "$sums" | awk ' { if ($2>65536) print $1 } ')"
        echo "$invalid_processes" >> "$temp"
    fi

    count="$((${count}+1))"
    if [[ "$valid" -eq 1 ]]; then
        break
    fi

    sleep 1
done

half=$((${count}/2))
cat < "$temp" | sort | uniq -c | awk -v half="${half}" ' { if ($1>=half) print $2 } '

exit 0
