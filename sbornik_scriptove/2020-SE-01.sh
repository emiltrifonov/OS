#!/bin/bash

if [[ $# -ne 2 ]]; then
    echo "Script must take in 2 arguments"
    exit 1
fi

if [[ ! -f "$1" ]]; then
    echo "First agrument must be a file"
    exit 2
fi

if [[ ! -d "$2" ]]; then
    echo "Second argument must be a directory"
    exit 3
fi

> "$1"

echo "hostname,phy,vlans,hosts,failover,VPN-3DES-AES,peers,VLAN Trunk Ports,licence,SN,key" >> "$1"

logs=$(find "$2" -type f -name "*.log")

for log in $logs; do

    info=$(cat < "$log" | sed -E 's/ license/:license/g' | sed -E 's/[[:space:]]*:[[:space:]]*/:/g'\
    | tail -n +3 | sed -E 's/^[[:space:]]*//g' | sed -E '/^[[:space:]]*$/d' |\
    sed -E 's/(a|an)[[:space:]]/:/g')

    echo "$(echo "$info" | awk -F: ' BEGIN { ORS="," } { print $2 } ' | sed 's/,$//')" >> "$1"

done

exit 0
