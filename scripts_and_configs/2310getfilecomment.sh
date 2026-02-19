#!/bin/bash
# Written by Peter Sutton Feb 2026
#
# Script to extract the file comment from a source file
#
# Usage: 2310getfilecomment.sh [--with-line-numbers] filename
#
# This script lives in the csse2310-style-guide repository

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
# States
# 0 - haven't found start of file comment
# 1 - inside /* style comment
# 2 - in /// style comment
WITH_LINE_NUMBERS=${with_line_numbers} \
    gawk 'BEGIN {
        # Have not found file comment
        state = 0;
        skip=0;
    }
    {
        # Replace all multiple spaces by single spaces
        gsub(/[[:space:]]+/, " ");
        # Remove all leading and trailing spaces on the line
        gsub(/^[[:space:]]+|[[:space:]]+$/, "");
    }
    (state == 2) && !/\s*\/\/(\/|!)/ {
        # Line that is not a // style comment - if we were in one, we
        # are now out
        exit;
    }
    (state == 0) && /(\/\*\*)|(\/\*!)/ {
        state = 1;
    }
    (state == 0) && /(\/\/\/)|(\/\/!)/ {
        state = 2;
        #next;
    }
    (state == 0) && !/^$/ { 
        # Looking for start of file comment but found something else
        printf("%s: error: - did not find expected documentation comment at start of file\n", FILENAME);
        exit
    }
    (state == 1) && /\*\// {
        # End of /* */ style comment
        # Strip off comment start and end characters
        remainder=gensub(/^[[:space:]*/!]+|[[:space:]*/]+$/, "", "g");
        if(length(remainder)) {
            printf("%s\n", remainder);
        }
        exit;
    }
    (state > 0) {
        # Inside comment - strip off comment start characters
        remainder=gensub(/^[[:space:]*/!]+/, "", "g");
        # Strip off spaces at end
        remainder=gensub(/\s+$/, "", "g", remainder);
        if(length(remainder)) {
            if(ENVIRON["WITH_LINE_NUMBERS"]) {
                printf("%d ", FNR);
            }
            printf("%s\n", remainder);
        }
    }
' "$file"
