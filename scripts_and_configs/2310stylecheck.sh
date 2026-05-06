#!/bin/bash
# 2310stylecheck.sh [--globalOK] [file ...]
#
# Written by Peter Sutton. Updated Feb 2026.
#
# Script that checks the style of the given files against the requirements
# of the CSSE2310/CSSE7231 style guide. If no files are specified, then the
# script checks all .c and .h files in the current directory
#
# If --globalOK is specified then one global bool is permitted without warning

export PATH="/local/courses/csse2310/bin:/usr/bin:$PATH"
configdir=/local/courses/csse2310/resources
doxyconfig=${configdir}/.csse2310.doxyconfig
clanglib=/local/courses/csse2310/lib/libClangTidy2310.so
shopt -s nullglob
includeArg="-I/local/courses/csse2310/include"
compilerArgs="-Wall -Wextra -pedantic -std=gnu99 ${includeArg}"
compileErrorFound=0
clangTidyErrorFound=0
TMPOUT=/tmp/.2310styleout.$$
TMPDIR=/tmp/.2310doxy$$
founderror=0
foundwarning=0
globalBoolsFound=0

function cleanup() {
    rm -f ${TMPOUT}
    rm -f -r ${TMPDIR}
}

trap cleanup EXIT

if test -t 1 ; then
    # stdout is a tty
    normal="$(tput sgr0)"
    bold="$(tput bold)"
    underline="$(tput smul)"
    reverse="$(tput rev)"
    red="$(tput setaf 1)"
    green="$(tput setaf 2)"
    blue="$(tput setaf 4)"
    magenta="$(tput setaf 5)"
else
    normal=""
    bold=""
    underline=""
    reverse=""
    red=""
    green=""
    blue=""
    magenta=""
fi

exec 2>&1

function colorise() {
    sed -E 's@^(.*\.[hc]:[0-9]+(:[0-9]+)?:?) @'${bold}'\1'${normal}' @;s@ [wW]arning:@'${bold}${magenta}' warning:'${normal}'@g;s@ [eE]rror:@'${bold}${red}' error:'${normal}'@g;s@^      \|(.*)$@'${green}'      |\1'${normal}'@;s@([[:space:]]+)\^(.*)$@\1'${green}'^\2'${normal}'@'
}

# Checks file /tmp/.2310styleout.$$ for warnings/errors and prints the given 
# heading, the result (OK/Error(s)/Warning(s)) and the output
function section() {
    heading="$@"
    warnings=$(grep -ci warning: ${TMPOUT})
    errors=$(grep -ci error: ${TMPOUT})
    contents=$(head -1 ${TMPOUT})
    styled_heading=$(echo "$heading" | fmt -w 55 | gawk '{printf "%-55s\n", $0}' | sed -e "1s/^/${reverse} ** /;2,\$s/^/${reverse}    /;s/$/${normal}/")
    if [[ "${contents}" == "Skipped" ]] ; then
        result="${red}Skipped${normal}"
        # Remove contents of temp file
        cp /dev/null ${TMPOUT}
    elif [[ $errors == 1 ]] ; then
        result="${red}Error found${normal}"
        founderror=1
    elif [[ $errors > 1 ]] ; then
        result="${red}${errors} errors found${normal}"
        founderror=1
    elif [[ $warnings == 1 ]] ; then
        result="${magenta}Warning found${normal}"
        foundwarning=1
    elif [[ $warnings > 1 ]] ; then
        result="${magenta}${warnings} warnings found${normal}"
        foundwarning=1
    else
        result="${green}OK${normal}"
    fi
    if [[ $errors > 0 && $warnings == 1 ]] ; then
        result+=" ${magenta}(and warning)${normal}"
    elif [[ $errors > 0 && $warnings > 1 ]] ; then
        result+=" ${magenta}(and ${warnings} warnings)${normal}"
    fi
    echo "${styled_heading}  ${bold}$result"
    cat ${TMPOUT} | colorise
}

function checkfile() {
    printf "${reverse}${bold}%-75s${normal}\n" "$1"
    if [ -r "$1" ] ; then

        echo "$1" | grep -E -q '(^[A-Z])|_' && echo "Warning: inappropriate source filename: $1" > ${TMPOUT}
        LC_ALL=C grep -n -P '[^\x00-\x7F]' "$1" | sed -e 's/^/Warning: Non ASCII character(s) at /' >> ${TMPOUT}
        section "Checking $1 for invalid characters or inappropriate name"

        gcc -fno-diagnostics-color $compilerArgs -c -o /dev/null "$1" > ${TMPOUT} 2>&1
        result=$?
        section "Checking $1 compiles by itself"
        if grep -sqi error: ${TMPOUT} ; then
            echo "${red}${bold}Found compilation error(s) - not checking other style aspects of $1${normal}" >&2
            compileErrorFound=1
            echo Skipped > ${TMPOUT}
            section "Running clang-format on $1"
            echo Skipped > ${TMPOUT}
            section "Running clang-tidy on $1"
            echo Skipped > ${TMPOUT}
            section "Checking comments in $1"
        else
            clang-format -fno-color-diagnostics --style=file:${configdir}/.clang-format \
                    --dry-run "$1" >${TMPOUT} 2>&1
            section "Running clang-format on $1"

            clang-tidy --quiet --use-color=0 --load=${clanglib} \
                    --config-file=${configdir}/.clang-tidy \
                    "$1" -- 2>/dev/null |
                sed -E 's@^/[^:*]*/@@' > ${TMPOUT}
            if [ "$globalBoolOK" = 1 ] ; then
                gawk 'BEGIN {foundBool = 0; found=0; skip=0; }
                    /cppcoreguidelines-avoid-non-const-global-variables/ {found=1; line=$0; next;}
                    skip {
                        skip=0;
                        next;
                    }
                    (found) {
                        found = 0;
                        if (match($0, "\\s*extern\\s+(volatile\\s+)?bool\\s+")) {
                            # We dont print anything for global extern bools
                            # skip printing next line also (with ^)
                            skip=1;
                            next;
                        } else if (match($0, "\\s*((static\\s+)|(volatile\\s+))*bool\\s+")) {
                            if(foundBool) {
                                modline=gensub(/(^.*:) warning:.*$/, "\\1", "g", line);
                                print modline " warning: additional global bool found - only one is permitted in this assignment"
                            } else {
                                foundBool = 1;
                                modline=gensub(/(^.*:) warning:.*$/, "\\1", "g", line);
                                print modline " note: global bool found - one is permitted in this assignment"
                            }
                        } else {
                            print line;
                        }
                    }
                    {
                        print
                    }
                ' < ${TMPOUT} > ${TMPOUT}.2
                if grep -sqi "note: global bool found" ${TMPOUT}.2 ; then
                    ((globalBoolsFound++))
                fi
                if [[ $globalBoolsFound -gt 1 ]] ; then
                    # Change our note back in to a warning
                    sed -e 's/note: global bool found - one/warning: additional global bool found - only one/' < ${TMPOUT}.2 > ${TMPOUT}
                else
                    cp ${TMPOUT}.2 ${TMPOUT}
                fi
                rm ${TMPOUT}.2
            fi
            section "Running clang-tidy on $1"
            if grep -sqi error: ${TMPOUT} ; then
                clangTidyErrorFound=1
                echo Skipped > ${TMPOUT}
                section "Checking comments in $1"
                echo "${red}${bold}Not running checks on comments for $1${normal}"
                echo "${red}${bold}clang-tidy must report no errors for this to be checked${normal}"
            else
                2310checkcomments.sh "$1" > ${TMPOUT}
                2310checkfunctioninternalcomments.sh "$1" >> ${TMPOUT}
                section "Checking comments in $1"
            fi
        fi
        rm -f ${TMPOUT}
    else
	echo "${red}Error: Can not open '$1' - not checking${normal}"
        founderror=1
    fi
    echo
}

if [ -x /local/courses/csse2310/bin/csse2310logusage ] ; then
    /local/courses/csse2310/bin/csse2310logusage
fi


# We'll set this if we allow global variables
files=()

# Process the command line arguments
globalBoolOK=0
while [ "$#" -gt 0 ] ; do
    case "$1" in
        --globalOK) 
            globalBoolOK=1
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
printf "${reverse}${bold}%-75s${normal}\n" "DOXYGEN CHECK"
if [[ ${compileErrorFound} == 0 && ${clangTidyErrorFound} == 0 ]] ; then
    # Make a temporary directory
    mkdir -p ${TMPDIR}
    # Copy all the files to a temporary directory
    for i in "${files[@]}" ; do
        # Comment out function declarations and const variables - no comments
        # are expected for these
        cp "${i}" "${TMPDIR}/tmp.${i}"
        clang-query --use-color=0 -c 'enable output print' \
                -extra-arg="-fcaret-diagnostics-max-lines=1000" \
                -c 'set traversal IgnoreUnlessSpelledInSource' \
                -c 'm varDecl(isExpansionInMainFile(), hasGlobalStorage(), isDefinition(), hasType(isConstQualified()))' \
                -c 'm varDecl(isExpansionInMainFile(), isExternC(), unless(hasInitializer(anything())))' \
                -c 'm functionDecl(unless(isDefinition()), isExpansionInMainFile())' \
                -c 'm functionDecl(hasName("main"), isDefinition())' \
                "$i" -- "${includeArg}" 2>/dev/null |
            gawk 'BEGIN {infile=0; printf("1\n");}
                /^\/.*\/'${i}':/ {infile=1; next;}
                /^\// {infile=0; next;}
                /^ +[0-9]+ \|/ {if (infile==1) { print; }}' |
            sed -E -e 's/^ +([0-9]+) +\|.*$/\1s/;2s@$@%^%// %@;$aw '"${TMPDIR}/tmp.${i}" -e '$aq' |
            ed -s "${TMPDIR}/tmp.${i}" >&/dev/null
            # Remove #define constants (other than those with parameters)
            # Remove __attribute__((unused))
            gawk 'BEGIN {indefine=0; continued=0;}
                /__attribute__\(\(unused\)\)/ {gsub(/__attribute__\(\(unused\)\)/, "");}
                /[[:space:]]*#[[:space:]]*define[[:space:]]+[A-Za-z_0-9]+([[:space:]]|$)/ {indefine=1; printf("// %s\n", $0); }
                indefine && continued {printf("// %s\n", $0); }
                !indefine {print}
                /\\$/ {continued=1;}
                ! /\\$/ {continued=0;}
                indefine && !continued {indefine=0;}
            ' "${TMPDIR}/tmp.${i}" > "${TMPDIR}/${i}"
            rm -f "${TMPDIR}/tmp.${i}"
    done
    cd ${TMPDIR}

    # Run doxygen to check the files
    doxygen "${doxyconfig}"
    # terminology. We update the file names to just show the base name.
    sed -e "s@^${TMPDIR}/@@" warnings.txt | sort -t: -k1,1 -k2,2n | sed '/Member.* of class/s/of class/of struct/;/Member.* of file/s/Member //;s/ of file / in file /;s/parameters of member/parameters of function/' > ${TMPOUT}
    section "Running doxygen on ${files[@]}"
else
    echo Skipped > ${TMPOUT}
    section "Running doxygen on ${files[@]}"
    echo "${red}${bold}Found error(s) that prevent doxygen checks${normal}"
    echo "${red}${bold}All files must compile and contain @file comments${normal}"
fi
exit $(($founderror * 2 + $foundwarning))
