#!/bin/bash
# Written by Peter Sutton Feb 2026
#
# Usage: 2310getcomments.sh [--with-line-numbers] filename
#
# Returns the lower level documentation comments from the given file
#
# Output is ///element-name type
# followed by the comment 
# followed by a blank line
#
# This script lives in the csse2310-style-guide repository

PATH="/local/courses/csse2310/bin:/usr/bin:$PATH"

with_line_numbers=0
if [[ "$1" == "--with-line-numbers" ]] ; then
    with_line_numbers=1
    shift
fi

file="$1"
if [ ! "$file" -o ! -r "$file" ] ; then
    echo "Can't open '$file' for reading" >&2
    exit 1
fi
shift

# Get the function names from the file
2310getdefns.sh --with-line-numbers "$file" |
    while read name start end comstart comend type ; do
        if [[ "$with_line_numbers" == 1 ]] ; then
            echo "${start} ///${name} ${type}"
        else
            echo "///${name} ${type}"
        fi
        if [[ "$comstart" != 0 ]] ; then
            # Extract the lines from the file and strip off leading comment
            # characters and trailing spaces
            if [[ "$with_line_numbers" == 1 ]] ; then
                cat -n "$file" | sed -n -e "${comstart},${comend}p" |
                    sed -E 's/^[[:space:]]+([0-9]+)\t[[:space:]*/!]+/\1\t/;s/[[:space:]]+$//;/^[0-9]+$/d;s/[[:space:]]+/ /g'
            else
                sed -n -e "${comstart},${comend}p" "$file" |
                    sed -E 's/^[[:space:]*/!]+//;s/[[:space:]]+$//;s/[[:space:]]+/ /g;/^$/d'
            fi
        fi 
        echo
    done
exit
