#!/bin/bash
# 2310stylecheck.sh [--globalOK] [file ...]
#
# Script that runs clang-format and clang-tidy to report any issues. If no
# filenames are given then all .c and .h files in the current directory are
# checked.

if [ -x /local/courses/csse2310/bin/csse2310logusage ] ; then
    /local/courses/csse2310/bin/csse2310logusage
fi

configdir=/local/courses/csse2310/resources
shopt -s nullglob

if [ "$1" = "--globalOK" ] ; then
    extraArg="--checks=-cppcoreguidelines-avoid-non-const-global-variables"
    shift
else
    extraArg=""
fi
function checkfile() {
    if [ -r "$1" ] ; then
	echo "---------- Running clang-format on $1"
	/usr/bin/clang-format --style=file:${configdir}/.clang-format \
		--dry-run "$1" 2>&1
	echo "---------- Running clang-tidy on $1"
	/usr/bin/clang-tidy -p /tmp --quiet \
		--config-file=${configdir}/.clang-tidy $extraArg "$1" 2>/dev/null
    else
	echo "---------- Can not open '$1' - not checking">&2
    fi
}

# Check if any arguments are given
if [ "$#" = 0 ] ; then
    # No arguments given - get the .c and .h files
    for i in *.[hc] ; do
        checkfile "$i"
    done
else
    while [ "$1" ] ; do
        checkfile "$1"
        shift
    done
fi
