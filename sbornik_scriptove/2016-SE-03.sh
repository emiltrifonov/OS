#!/bin/bash

if [[ $(id -u) -ne 0 ]]; then
    echo "Script must be run by root"
    exit 1
fi

while read -r user home; do

    if [[ -z "$user" || -z "$home" ]]; then
        continue
    fi

    #bits=$(stat -c %A "${home}")
    #echo "${bits}"
    if [[ ! -d "$home" ]]; then
        echo "${user}'s home directory ${home} doesn't exist"
        continue
    fi

    bits=$(stat -c %A "${home}")

    if [[ ! "${bits}" =~ ^.{2}w.*$ ]]; then
        echo "${user} cannot write in their home directory ${home}"
    fi

done < <( awk -F: '{print $1,  $6}' /etc/passwd)

exit 0
