#!/bin/bash

if [[ $(id -u) -ne 0 ]]; then
    echo "Script must be run by root"
    exit 1
fi

uid_file=$(find /home -type f -printf "%T@ %u %p\n" 2>/dev/null | sort -t' ' -k1,1 -nr | head -n 1 | cut -d' ' -f2-)
user=$(echo "$uid_file" | cut -d' ' -f1)
file=$(echo "$uid_file" | cut -d' ' -f2)
name=$(egrep "$user" /etc/passwd | cut -d: -f5 | cut -d, -f1)

echo "${name} ${file}"

exit 0
