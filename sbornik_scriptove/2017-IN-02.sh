#!/bin/bash

if [[ $# -ne 1 ]]; then
    echo "Script must take in exactly 1 argument"
    exit 1
fi

if [[ $(id -u) -ne 0 ]]; then
    echo "Script must be run by root"
    exit 2
fi

if ! id "$1" &>/dev/null; then
    echo "User doesn't exist"
    exit 3
fi

# a)
# print users with more processes than user given as argument
echo "Users with more processes than $1($(ps -U $1 -o pid= | wc -l)):"
ps -eo ruser | sort | uniq -c | sort -nr | awk -v usr=$1 ' { if ( $2 == usr ) exit; else { print $2"("$1")" } } '
echo ""

# b)
# print average runtime for all processes in seconds (in format HH:MM:SS)
avgsec=$(ps -eo etimes= | awk ' BEGIN {c=0;}
{
    sum+=$1;
    c++;
}
END { if (c > 0) print sum/c; else print 0 }
')
avgtime=$avgsec
hours=$(echo "$avgsec / 3600" | bc)
avgsec=$(echo "$avgsec-$(echo "3600 * $hours" | bc)" | bc)
mins=$(echo "$avgsec / 60" | bc)
avgsec=$(echo "$avgsec-$(echo "60 * $mins" | bc)" | bc)

echo "Average time every current process has been running for: ${hours}:${mins}:${avgsec}"
echo ""

# c)
# kill all of $1's processes which have worked at least double the average time
while IFS= read -r line; do

    runtime=$(echo "$line" | awk ' { print $2 } ')

    if [[ $runtime -gt $(($avgtime * 2)) ]]; then
        kill $(echo "$line" | awk ' { print $1 } ')
    fi

done < <(ps -U "$1" -o pid=,etimes=)


exit 0
