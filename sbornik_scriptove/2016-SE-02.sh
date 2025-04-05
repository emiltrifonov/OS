#!/bin/bash

if [[ $# -ne 1 ]]; then
    echo "Script takes exactly 1 argument"
    exit 1
fi

if [[ $(id -u) -ne 0 ]]; then
    echo "Script must be executed by root"
    exit 2
fi

processes=$(ps -eo ruser,pid,rss)

#echo "$processes"

userToMem=$(echo "$processes" | tail -n +2 | awk '
    { mems[$1]+=$3; }

    END {

        for (u in mems) {

            { print u, mems[u] }

        }

    }
')

echo "$userToMem"

while read line; do

    usrMem=$(echo $line | cut -d' ' -f2)
    #echo "$usrMem"

    if [[ "$usrMem" -gt "$1" ]]; then
        usr=$(echo $line | cut -d' ' -f1)
        #echo "$usr"
        id=$(ps -eo rss,ruser,pid | awk -v user=$usr ' $2 == user  { print $0 } ' | sort -nr | head -n 1 | awk ' {print $3} ')
        kill "$id"
    fi

done < <(echo "${userToMem}")

exit 0
