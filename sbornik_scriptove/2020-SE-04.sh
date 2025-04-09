#!/bin/bash

if [[ $# -ne 2 ]]; then
    echo "Script must take in 2 arguments"
    exit 1
fi

if [[ ! -d "$1" ]]; then
    echo "First argument must be an existing directory"
    exit 2
fi

if [[ -e "$2" ]]; then
    echo "Second argument must be the name of a nonexisting (and to be created) directory"
    exit 3
fi

names=$(find "$1" -type f -printf "%f\n")

mkdir "$2"

mkdir -p "$2"/"images"
images="$2"/"images"

mkdir -p "$2"/"by-date"
bydate="$2"/"by-date"

mkdir -p "$2"/"by-album"
byalbum="$2"/"by-album"

mkdir -p "$2"/"by-title"
bytitle="$2"/"by-title"

# name,title,album,date,hash
while IFS= read -r line; do

    title="$(echo "$line" | sed 's/([^()]*)//g' | tr -s ' ' | \
        awk -F. ' { print $1 } ' | sed 's/^[[:space:]]*//g' | sed 's/[[:space:]]*$//g')"

    album=""

    if [[ -z "$(echo "$line" | egrep "(\(|\))")" ]]; then
        album="misc"
    else
        album="$(echo "$line" | egrep -o "[\(][^\(]*\)[^\(\)]*$" |\
            awk -F')' ' { print $1 } ' | sed 's/^(//')"
    fi

    date="$(find "$1" -type f -name "*${line}*" -printf "%TF\n")"

    hesh="$(sha256sum "$1"/"$line" | cut -c 1-16)"

    hjpg="$images"/"$hesh".jpg

    cp "$1"/"$line" "$hjpg"

    # ln -s TARGET LINK

    ln -s "$hjpg" "$bytitle"/"$title"

    d1="$bydate"/"$date"/"by-album"/"$album"/"by-title"
    mkdir -p "$d1"
    ln -s "$hjpg" "$d1"/"$title".jpg

    d2="$bydate"/"$date"/"by-title"
    mkdir -p "$d2"
    ln -s "$hjpg" "$d2"/"$title".jpg

    d3="$byalbum"/"$album"/"by-date"/"$date"/"by-title"
    mkdir -p "$d3"
    ln -s "$hjpg" "$d3"/"$title".jpg

    d4="$byalbum"/"$album"/"by-title"
    mkdir -p "$d4"
    ln -s "$hjpg" "$d4"/"$title".jpg

done < <(echo "$names")

exit 0
