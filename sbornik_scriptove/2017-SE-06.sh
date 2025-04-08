#!/bin/bash

if [[ $(id -u) -eq 0 ]]; then
    echo "Script must be run by root"
    exit 1
fi

procs=$(ps -eo pid=,ruser=,rss= | awk ' {

    if ( $2 != "root" ) print $1" "$2" "$3;

} ')

users=$(echo "$procs" | awk ' { print $2; } ' | sort | uniq)

finalprocs=$(while IFS= read -r u; do

    homedir=$(getent passwd "${u}" | cut -d: -f6)

    if [[ -d "$homedir" && "${u}" == "$(stat -c %U "${homedir}")" && "$(stat -c %A "${homedir}" | cut -c 3)" == "w" ]]; then
        continue
    fi

    echo "$procs" | awk -v u=$u '$2 == u'

done < <(echo "${users}"))

rootmem=$(ps -eo user=,rss= | egrep "^[[:space:]]*root[[:space:]]+" | awk ' { sum+=$2 } END { print sum; } ')

# pid,ruser,rss
finalusers=$(echo "$finalprocs" | awk -v rootmem=$rootmem ' {

    mem[$2]+=$3;

}

END {

    for (u in mem) {
        if ( mem[u] > rootmem ) {
            print u
        }
    }

} ')

while IFS= read -r process; do
    pid=$(echo "$process" | awk ' { print $1 } ')
    user=$(echo "$process" | awk ' { print $2 } ' )

    for fuser in $finalusers; do
        if [[ "$user" == "$fuser" ]]; then
            kill $pid
            break
        fi
    done

done < <(echo "$finalprocs")

exit 0
