#!/bin/bash

if [[ $# -ne 3 ]]; then
    exit 1
fi

if [[ ! -f "$1" || ! -f "$2" ]]; then
    exit 2
fi

info1="$(egrep "${3}" "${1}")"
if [[ -z "$info1" ]]; then
    exit 3
fi

info2="$(egrep "${3}" "${2}")"
if [[ -z "$info2" ]]; then
    exit 4
fi

dist1="$(echo "$info1" | cut -d: -f2 | cut -d' ' -f2)"
dist2="$(echo "$info2" | cut -d: -f2 | cut -d' ' -f2)"

if [[ "$dist1" -gt "$dist2" ]]; then
    echo "$1"
elif [[ "$dist2" -gt "$dist1" ]]; then
    echo "$2"
else
    echo "${1} = ${2}"
fi

exit 0
