#!/bin/bash
# 2310reformat.sh
#
# Script that runs clang-format with the CSSE2310/7231 specific
# .clang-format configuration and fixes the format of the file.
#	

# Check if any arguments are given
if [ ! "$1" ] ; then 
    echo "Usage: 2310format.sh filename ..." >&2
    exit 1
fi

if [ -x /local/courses/csse2310/bin/csse2310logusage ] ; then
    /local/courses/csse2310/bin/csse2310logusage
fi

# Iterate over all the arguments. If readable then reformat
while [ -r "$1" ] ; do
    if ! /usr/bin/clang-format \
	    --style=file:/local/courses/csse2310/resources/.clang-format \
	    -i "$1" ; then
	echo "Reformatting failed on file '$1' - aborting" >&2
	exit 2
    fi
    shift
done
# Check if any files left
if [ "$1" ] ; then 
    # At least one file left - we must not have been able to open it
    echo "Unable to open file '$1' - aborting" >&2
    exit 3
fi
