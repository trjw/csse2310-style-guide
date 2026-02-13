#!/bin/bash
# 2310stylecheck.sh [--globalOK] [file ...]
#
# Script that checks the style of the given files against the requirements
# of the CSSE2310/CSSE7231 style guide. If no files are specified, then the
# script checks all .c and .h files in the current directory

configdir=/local/courses/csse2310/resources
clanglib=/local/courses/csse2310/lib/libClangTidy2310.so
shopt -s nullglob
compilerArgs="-Wall -Wextra -pedantic -std=gnu99 -I/local/courses/csse2310/include"
compileErrorFound=0

function checkfile() {
    if [ -r "$1" ] ; then
        echo "---------- Checking $1 for invalid characters or inappropriate name"
        echo "$1" | grep -E -q '(^[A-Z])|_' && echo "Warning: inappropriate source filename: $1"
        LC_ALL=C grep -n -P '[^\x00-\x7F]' "$1" | sed -e 's/^/Warning: Non ASCII character(s) at /'
        echo "---------- Checking $1 compiles by itself"
        if ! /usr/bin/gcc $compilerArgs -c -o /dev/null "$1" 2>&1
            echo "---------- Found compilation errors - not checking other style aspects" >&2
            compileErrorFound=1
        else
            echo "---------- Running clang-format on $1"
            /usr/bin/clang-format --style=file:${configdir}/.clang-format \
                    --dry-run "$1" 2>&1
            echo "---------- Running clang-tidy on $1"
            /usr/bin/clang-tidy --quiet --load=${clanglib} \
                    --config-file=${configdir}/.clang-tidy $extraClangTidyArg \
                    "$1" -- 2>/dev/null
            /usr/bin/doxygen
    else
	echo "---------- Can not open '$1' - not checking">&2
    fi
}

if [ -x /local/courses/csse2310/bin/csse2310logusage ] ; then
    /local/courses/csse2310/bin/csse2310logusage
fi


# We'll set this if we allow global variables
extraClangTidyArg=""
files=()

# Process the command line arguments
while [ "$#" -gt 0 ] ; do
    case "$1" in
        --globalOK) 
            extraClangTidyArg="--checks=-cppcoreguidelines-avoid-non-const-global-variables"
            ;;
        *)
            files+=("$1")
            ;;
    esac
    shift
done

# Check if any files were given - if not, use all .h and .c files in the
# current directory

if [ ${#files[@]} = 0 ] ; then
    files=(*.h *.c)
fi
if [ ${#files[@]} = 0 ] ; then
    echo "No files to check" >&2
    exit 1
fi

for file in "${files[@]}" ; do
    checkfile "$file"
done

# Doxygen must always be run on all files together
if [ "$compileErrorFound" = 0 ] ; then
    # TODO - should check for the presence of @file commands also - these
    # are a prereq for doxygen working

    echo "---------- Running doxygen on ${files[@]}"
    # Make a temporary directory
    TMP=/tmp/.2310doxy$$
    mkdir -p ${TMP}
    # Copy all the files to a temporary directory
    for i in "${files[@]}" ; do
        /usr/bin/clang-query --use-color=0 -c 'enable output print' \
                -c 'set traversal IgnoreUnlessSpelledInSource' \
                -c 'm varDecl(isExpansionInMainFile(), hasGlobalStorage(), isDefinition(), hasType(isConstQualified()))' \
                -c 'm functionDecl(unless(isDefinition()), isExpansionInMainFile(), isExpansionInFileMatching(".*\\.c"))' \
                "$i" -- |
            gawk 'BEGIN {infile=0; printf("1\n");}
                /^\/.*\/'$1':/ {infile=1; next;}
                /^\// {infile=0; next;}
                /^ +[0-9]+ \|/ {if (infile==1) { print; }}' |
            sed -E -e 's/^ +([0-9]+) +\|.*$/\1s/;2s@$@%^%// %@;$aw '"${TMP}/${i}" -e '$aq' |
            ed -s "${TMP}/${i}"
    done
    cd ${TMP}

    /usr/bin/doxygen ${configDir}/.csse2310.doxyconfig
    # Output is now in warnings.txt - filter and sort - and change the 
    # terminology
    sed -e "s@^${TMP}/@@" warnings.txt | sort -t: -k1,1 -k2,2n | sed '/Member.* of class/s/of class/of struct/;/Member.* of file/s/Member //;s/ of file / in file /;s/parameters of member/parameters of function/'
    rm -f -r ${TMP}
fi
