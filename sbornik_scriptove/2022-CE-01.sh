#!/bin/bash

if [[ $# -ne 3 ]]; then
    exit 1
fi

if [[ -z "$(echo "$1" | egrep "^[0-9]*$")" ]]; then
    exit 2
fi

prefix="$(cat < "prefix.csv" | tail -n +2 | cut -d, -f2-3 | egrep "^${2}," | cut -d, -f2)"

if [[ -z "$prefix" ]]; then
    exit 3
fi

base="$(cat < "base.csv" | tail -n +2 | cut -d, -f2-3 | egrep "^${3},")"

if [[ -z "$base" ]]; then
    exit 4
fi

baserow="$(cat < "base.csv" | tail -n +2 | egrep "${base}")"

info="("$(echo "$baserow" | awk -F, ' { print $3", "$1 } ')")"
num="$(echo "${1} * ${prefix}" | bc)"

echo ""${num}" "${2}" "${info}""

exit 0
