#!/bin/bash
# Written by Peter Sutton Feb 2026
#
# Script to return a list of types and functions defined in a given source file
#
# 2310getdefns.sh [--with-line-numbers] [--functions] [--types] filename
#
# If --with-line-numbers is present, the names of the types/functions are 
# followed by five fields (tab separated):
# - start line number of definition
# - end line number of definition
# - start line of documentation omment (after comment open, 0 if none)
# - end line of documentation ype comment (before comment close, 0 if none)
# - type of thing (function, union, struct, enum or typedef)
#
# This script lives in the csse2310-style-guide repository

with_line_numbers=0
funcs=0
types=0
both=1
while [[ "$1" =~ ^-- ]] ; do
    case "$1" in
        "--with-line-numbers" )
            with_line_numbers=1
            ;;
        "--functions" )
            both=0
            funcs=1
            ;;
        "--types" )
            both=0
            types=1
            ;;
        * )
            echo "Error: Unknown argument: $1" >&2
            exit 1
    esac
    shift
done
if [[ "$both" == 1 ]]  ; then
    funcs=1
    types=1
fi

file="$1"
if [ ! "$file" -o ! -r "$file" ] ; then
    echo "Error: Can't open '$file' for reading" >&2
    exit 1
fi

clang-query --use-color=0 \
        -c 'set output dump' \
        -c 'set traversal IgnoreUnlessSpelledInSource' \
        -c 'match functionDecl(isDefinition(), isExpansionInMainFile())' \
        -c 'match namedDecl(anyOf(recordDecl(isDefinition(), unless(hasName("(anonymous)"))),enumDecl(isDefinition(), unless(hasName("(anonymous)"))), typedefDecl()), isExpansionInMainFile())' \
        "$file" -- -I/local/courses/csse2310/include |
    FILENAME="$file" \
    WITH_LINE_NUMBERS="$with_line_numbers" \
    FUNCS="$funcs" \
    TYPES="$types" \
    gawk '
        BEGIN {
            printit=0;
            # Indicator to print data when we get a blank line
        }
        /^(Function|Record|Typedef|Enum)Decl/ {
            commentstart=0
            commentend=0
        }
        /^TypedefDecl.*col:[0-9]+>/ {
            startline=gensub(/^.*:([0-9]+):[0-9]+, col.*/, "\\1", 1);
            endline=startline
        }
        /^TypedefDecl.*line:[0-9]+:[0-9]+>/ {
            startline=gensub(/^.*:([0-9]+):[0-9]+, line.*/, "\\1", 1);
            endline=gensub(/^.*, line:([0-9]+):[0-9]+>.*/, "\\1", 1);
        }
        /^(Function|Record|Enum)Decl.*line:/ {
            startline=gensub(/^.*> line:([0-9]+):.*/, "\\1", 1);
            endline=gensub(/^.*:([0-9]+):[0-9]+> line.*/, "\\1", 1);
        }
        ENVIRON["FUNCS"] && /^FunctionDecl/ { 
            name=gensub(/^.*> line:[0-9:]+ (used |referenced )?([a-zA-Z0-9_]+) \x27.*/, 
                    "\\2", "g");
            thing="function"
            printit=1;
            next;
        }
        ENVIRON["TYPES"] && /^RecordDecl/ {
            name=gensub(/^.*> line:[0-9:]+ (used |referenced )?(union|struct) ([a-zA-Z0-9_]+) definition$/, "\\3", "g");
            thing=gensub(/^.*> line:[0-9:]+ (used |referenced )?(union|struct) ([a-zA-Z0-9_]+) definition$/, "\\2", "g");
            printit=1;
            next;
        }
        ENVIRON["TYPES"] && /^EnumDecl/ {
            name=gensub(/^.*> line:[0-9:]+ ([a-zA-Z0-9_]+)$/, "\\1", "g");
            thing="enum";
            printit=1;
            next;
        }
        ENVIRON["TYPES"] && /^TypedefDecl/ {
            name=gensub(/^.*> col:[0-9:]+ (used |referenced )?([a-zA-Z0-9_]+) .*/, "\\2", "g");
            thing="typedef";
            printit=1;
            next;
        }
        /^.-FullComment.*<line:.* col:/ {
            commentstart=gensub(/^.*<line:([0-9]+):.*/, "\\1", 1);
            commentend=commentstart
            next
        }
        /^.-FullComment.*<line:.*, line:/ {
            commentstart=gensub(/^.*<line:([0-9]+):.*/, "\\1", 1);
            commentend=gensub(/^.*<line:[0-9:]+, line:([0-9]+):.*/, "\\1", 1);
            next
        }
        /^.-FullComment.*<.*, col:/ {
            commentstart=gensub(/^.*:([0-9]+):[0-9]+, col.*/, "\\1", 1);
            commentend=commentstart
            next
        }
        /^.-FullComment.*, line:/ {
            commentstart=gensub(/^.*:([0-9]+):[0-9]+, line.*/, "\\1", 1);
            commentend=gensub(/^.*:[0-9]+:[0-9]+, line:([0-9]+):.*/, "\\1", 1);
            next
        }
        printit && /^$/ {
            printf("%s", name);
            if(ENVIRON["WITH_LINE_NUMBERS"]) {
                printf("\t%d\t%d\t%d\t%d\t%s\n", 
                        startline, endline, commentstart, commentend, thing);
            } else {
                printf("\n");
            }
            printit=0
        }
    '
