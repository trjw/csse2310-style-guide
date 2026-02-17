#!/bin/bash

while [ "$1" ] ; do
    if [ ! -r "$1" ] ; then
        echo "Can't open '$1' for reading" >&2
        exit 1
    fi

    # Awk states are
    # 0 - looking for "Match"
    # 1 - found "Match - looking for "root" binds here
    # 2 - found function header
    # 3 - inside {} within function - we can count lines
    # 4 - inside /* */ comment within function
    clang-query --use-color=0 -c 'set output diag' \
            -extra-arg="-fcaret-diagnostics-max-lines=200" \
            -c 'enable output dump' \
            -c 'set traversal IgnoreUnlessSpelledInSource' \
            -c 'match functionDecl(isDefinition(), isExpansionInMainFile())' \
            "$1" -- -I/local/courses/csse2310/include |
        FILENAME="$1" \
        gawk 'BEGIN {state=0;}
            /^Match #.*:$/ { 
                state=1; 
                chars_with = 0; 
                comment_chars = 0;
                startline = 0;
                lines=-1;
                next; 
            }
            /"root" binds here/ {
                state=2; 
                next; 
            }
            /^FunctionDecl/ { 
                fnname=gensub(/^.*> line(:[0-9]+):.* ([a-zA-Z0-9_]+) \x27.*/, 
                        "\\1 Function \\2()", "g");
                if(lines > 20 && comment_chars == 0) {
                    printf("Warning: %s%s has over 20 lines (%d) " \
                            "but no internal comments\n", 
                            ENVIRON["FILENAME"], fnname, lines);
                }
                state = 0;
            }
            # line does not include leading line number - skip it
            !/^ *[0-9]+ \|/ {
                next;
            }
            # Look for line that starts function
            /^ *[0-9]+ \|.*{/ {
                if(state==2) {
                    state = 3;
                    lines=-1;
                }
            }
            /^ *[0-9]+ \|/ {
                if(startline == 0) {
                    match($0, "^ *([0-9]+) ", a);
                    startline=a[1];
                }
                if(state>=3) {
                    lines++;
                }
                # Remove line number prefix
                sub("^ *[0-9]+ \\| ","");
                chars_with += length($0);
                # Work out if we are inside /* */ comment 
                if(state == 3 && match($0,"/\\*(.*)\\*/", a)) {
                    # Comment starts and ends on same line
                    comment_chars += gsub("\\S","@", a[1]);
                } else if (state == 3 && match($0, "/\\*(.*)$", a)) {
                    # Comment starts
                    comment_chars += gsub("\\S", "@", a[1]);
                    state = 4;
                } else if (state == 4 && match($0, "^(.*)\\*/", a)) {
                    # Comment ends
                    comment_chars += gsub("\\S", "@", a[1]);
                    state = 3;
                } else if (state == 4) {
                    # Line inside comment
                    comment_chars += gsub("\\S", "@", $0);
                } else if (state == 3 && match($0, "//(.*)$", a)) {
                    # // style comment
                    comment_chars += gsub("\\S", "@", a[1]);
                }
            }
        '
    shift
done




