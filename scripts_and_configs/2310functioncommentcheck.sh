#!/bin/bash
# 2310functioncommentcheck.sh [file ...]
#
# Written by Peter Sutton. Updated Apr 2026.
#
# Checks that the function comments of all files are correct (using clang-tidy)
#

export PATH="/local/courses/csse2310/bin:/usr/bin:$PATH"
configdir=/local/courses/csse2310/resources
clanglib=/local/courses/csse2310/lib/libClangTidy2310.so
shopt -s nullglob
includeArg="-I/local/courses/csse2310/include"
clangTidyErrorFound=0
TMPOUT=/tmp/.2310filecommentcheck.out.$$

function cleanup() {
    rm -f ${TMPOUT}
}

trap cleanup EXIT

exec 2>&1

function checkfile() {
    clang-tidy --quiet --use-color=0 --load=${clanglib} \
            --config-file=${configdir}/.clang-tidy \
            "$1" -- 2>/dev/null > ${TMPOUT}
    if grep '\[csse2310-file-comment' ${TMPOUT} > ${TMPOUT}.2 ; then
        clangTidyErrorFound=1
        cat ${TMPOUT}.2 | sed -E 's@^/.*/([^/]+\.[hc]:)@\1@'
    else
        echo "$1: OK"
    fi
    rm -f ${TMPOUT} ${TMPOUT}.2
}

if [ -x /local/courses/csse2310/bin/csse2310logusage ] ; then
    /local/courses/csse2310/bin/csse2310logusage
fi


files=("$@")

# Check if any files were given - if not, use all .h and .c files in the
# current directory

if [ ${#files[@]} = 0 ] ; then
    files=(*.h *.c)
fi
if [ ${#files[@]} = 0 ] ; then
    echo "No files to check" >&2
    exit 1
fi

echo "Checking file comments"
for file in "${files[@]}" ; do
    checkfile "$file"
done

exit $clangTidyErrorFound
