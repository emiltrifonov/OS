#!/bin/bash

touch "lastx"
lastx="$(cat < "lastx")"
echo "${EPOCHSECONDS}" > "lastx"

if [[ -z "$lastx" ]]; then
    lastx=0
fi

if [[ $# -ne 1 ]]; then
    exit 1
fi

if [[ ! -d "$1" ]]; then
    exit 2
fi

archs="$(find "$1" -type f -name "*.tgz" -printf "%p\n")"

while IFS= read -r arch; do
    #filebirth, lastmod
    data="$(stat -c "%W %Y" "$arch")"
    fbirth="$(echo "$data" | cut -d' ' -f1)"
    lastmod="$(echo "$data" | cut -d' ' -f2)"
    mkdir -p "temp"
    if [[ 0 -lt 1 || "$fbirth" -gt "$lastx" || "$lastmod" -gt "$lastx" ]]; then
        cd "temp"
        tar -xzf "../$arch" >&2
        if [[ $? -eq 0 ]]; then
            NAME="$(echo "$arch" | cut -d'-' -f1 | cut -d'/' -f2)"
            TIMESTAMP="$(echo "$arch" | rev | cut -c 5- | cut -d'-' -f1 | rev)"

            mkdir -p "../extracted"
            find . -type f -name "meow.txt" -exec cp {} "../extracted/${NAME}_${TIMESTAMP}.txt" \;
        fi
    fi
    cd ~
    rm -r "temp"

done < <(echo "$archs")

exit 0
