#!/bin/bash

if [[ $(id -u) -ne 0 ]]; then
    echo "Script must be run by root"
    exit 1
fi

# a)
# print total memory used by processes per user

allinfo=$(ps -eo ruser=,rss=,pid= | awk '{

    if ( $2 > max_mem[$1] ) max_mem[$1]=$2; max_pid[$1]=$3
    total_mem[$1]+=$2;
    cnt[$1]++;

}
    END {
        print "user count allmem maxmem pid"
        for (u in total_mem)
            print u" "cnt[u]" "total_mem[u]" "max_mem[u]" "max_pid[u]" "total_mem[u]/cnt[u]

    } ')

# b)
# kill each user's memory heaviest process if it consumes more memory than the average
# of all their processes

while read -r line; do

    pmem=$(echo "$line" | cut -d' ' -f1)
    pid=$(echo "$line" | cut -d' ' -f2)
    avgmem=$(echo "$line" | cut -d' ' -f3)
    maxallowedmem=$(echo "$avgmem * 2" | bc -l)

    if [[ "$(echo "$pmem > $maxallowedmem" | bc)" -eq 1 ]]; then
        kill $pid
    fi

done < <(echo "${allinfo}" | tail -n +2 | awk ' { print $4, $5, $6 } ')

exit 0
