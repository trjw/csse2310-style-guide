#!/bin/bash

export PATH=/local/courses/csse2310/bin:/usr/bin:$PATH

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

trap cleanup EXIT

function cleanup() {
    rm -f -r /tmp/.csse2310.check.commit.*
}

if ! repo_url=$(svn info --show-item url) ; then
    exit 1
fi
if ! [[ ${repo_url} =~ ^svn\+ssh://source\.eait\.uq\.edu\.au/csse2310.*/trunk/a[1234]$ ]] ; then
    # Repo URL is not the top level of an assignment checkout
    echo "`pwd` is not a CSSE2310/7231 assignment working directory" >&2
    exit 1
fi

# Get the head version number and the working copy current version
# If the working copy version reported does not end in an "M" then no 
# changes have been made
headversion=$(svn info -r HEAD --show-item revision || exit 1)
myversion=$(svnversion || exit 1)
if [[ "$headversion" == "$myversion" ]]  ; then
    echo "No changes to version controlled files - nothing would be committed" >&2
    exit 1
elif [[ ${myversion} =~ .*:${headversion} ]] ; then 
    echo "Working copy may not have all of the files from your latest version (r${headversion}) - run 'svn update' first" >&2
    exit 1
elif ! [[ "$headversion"M == "$myversion" ]]  ; then
    echo "Working copy not at latest version - run 'svn update' first" >&2
    exit 1
fi

# Make a temporary directory that we'll do the style checking in
cleanup
tmp=/tmp/.csse2310.check.commit.$$
mkdir -p ${tmp}

# Checkout the head version into a temporary directory
svn checkout --depth files "${repo_url}" ${tmp}/repo >/dev/null || exit 1

# Count lines of code
loc=$(cd ${tmp}/repo; gcc -fpreprocessed -dD -E -P *.[hc] 2>/dev/null | wc -l)

echo "Starting files:"
(cd $tmp/repo; ls *.[hc])
echo "Lines of code in previous revision (r${headversion}): $loc"

# Get the list of modified or added files (there must be some)
svn status > $tmp/.status || exit 1
echo "Changes in this commit:"
cat $tmp/.status
while read status file ; do
    case $status in 
        A ) # File added
            cp "$file" "${tmp}/repo"
            ;;
        M ) # File modified - already copied
            ;;
        R ) # File replaced - already copied?
            ;;
        D ) # File deleted - remove it if it is there
            rm -f "${tmp}/repo/${file}"
            ;;
        ? ) # Unknown file - ignore
            ;;
        * )
            echo "Unexpected svn status (${status}) for file ${file}"
            exit 1
            ;;
    esac
done < $tmp/.status
rm -f $tmp/.status

cd ${tmp}/repo
newloc=$(gcc -fpreprocessed -dD -E -P *.[hc] 2>/dev/null | wc -l)
echo "Lines of code after this commit: $newloc"

echo "${bold}Checking style on the following files:${normal}"
ls
2310stylecheck.sh
status=$?
linesadded=$((newloc - loc))
if [[ $linesadded -gt 150 ]] ; then
    echo "${red}${bold}More than 150 lines of code added in this commit${normal}" >&2
    ((status++))
fi
exit $status
