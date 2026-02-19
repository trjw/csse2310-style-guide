#!/bin/bash
# Written by Peter Sutton Feb 2026
#
# Usage: 2310checkcomments.sh filename ...
#
# Script to check whether the expected documentation comments 
# have the required @ai, @aitool and @aidetails fields.
#
# This script lives in the csse2310-style-guide repository

export PATH="/local/courses/csse2310/bin:/usr/bin:$PATH"

while [ "$1" ] ; do
    file="$1"
    shift
    if [ ! -r "$file" ] ; then
        echo "Can't open '$file' for reading" >&2
        exit 1
    fi
    # Get the file comment for this file
    basename=$(basename $file)
    if ! 2310getfilecomment.sh --with-line-numbers "$file" |
        BASENAME="${basename}" \
        gawk 'BEGIN {
                filename=ENVIRON["BASENAME"]
                # Have not found file comment
                state = 0;
                ainotused=0;
                countaicommands = 0;
                countaitools = 0;
                error = 0;
                exitstatus = 1;
                skip=0;
            }
            {
                linenum=gensub(/^([0-9]+) .*$/, "\\1", "g");
                gsub(/^[0-9]+ /, "");
            }
            (FNR == 1) && !/^[@\\]file '${basename}'$/ {
                printf("%s:1 Error: file comment does not begin with @file command\n", filename);
                error=1
                exit
            }
            (FNR > 1) && /^[@\\]file / {
                printf("%s:%d Error: file comment has more than one @file command\n", filename, linenum);
                exitstatus=2
            }
            /^[@\\]ai Not Used$/ {
                exitstatus=0;
                ainotused=1;
                skip=1;
            }
            /^[@\\]ai (Wrote Code|Wrote Comments|Inspiration|Debugging)$/ {
                exitstatus=1;
                countaicommands++;
                skip=1;
            }
            !skip && /^[@\\]ai( |$)/ {
                # Invalid @ai command 
                printf("%s:%d warning: invalid @ai command:\"%s\"\n", 
                        filename, linenum, $0);
                error=1
                next
            }
            /^[@\\]aitool \S+/ {
                exitstatus=1;
                countaitools++;
                skip=1;
            }
            !skip && /^[@\\]aitool$/ {
                printf("%s:%d warning: invalid @aitool command:\"%s\"\n", 
                        filename, linenum, $0);
                error=1;
            }
            END {
                if(ainotused && countaicommands) {
                    printf("%s:%d warning: found @ai command(s) in addition to @ai Not Used\n", filename, linenum);
                    error=1
                }
                if(ainotused && countaitools) {
                    error=1
                    printf("%s:%d warning: found @aitool command(s) in addition to @ai Not Used\n", filename, linenum);
                }
                if(countaicommands > 0 && countaitools == 0) {
                    printf("%s:%d warning: found @ai command(s) (besides @ai Not Used) without required @aitool command(s)\n", filename, linenum);
                    error=1
                }
                if(error) {
                    printf("Note: assuming that function and type @ai commands are needed\n");
                    exitstatus=1
                }
                exit exitstatus;
            }
        ' ; then
        # Now need to check the comments within the file
        echo "Note: checking comments within file"
        2310getcomments.sh --with-line-numbers "$file" |
            FILENAME="$file" \
            gawk '
                BEGIN {
                    filename=ENVIRON["FILENAME"];
                }
                {
                    linenum=gensub(/^([0-9]+) .*$/, "\\1", "g");
                    gsub(/^[0-9]+ /, "");
                }
                /^\/\/\// {
                    # New function/type - grab the name and type
                    name=gensub(/^\/\/\/([a-zA-Z0-9_]+) .*/, "\\1", "g");
                    type=gensub(/^[^ ]+ ([a-z]+)$/, "\\1", "g");
                    startline=linenum
                    ainotused = 0;
                    countaicommands = 0;
                    countaidetails = 0;
                    next;
                }
                /^[@\\]ai Not Used$/ {
                    ainotused=1;
                    next;
                }
                /^[@\\]ai (Wrote Code|Wrote Comments|Inspiration|Debugging)$/ {
                    countaicommands++;
                    next;
                }
                !skip && /^[@\\]ai( |$)/ {
                    # Invalid @ai command 
                    printf("%s:%d warning: invalid @ai command:\"%s\"\n", 
                            filename, linenum, $0);
                    error=1
                    next;
                }
                /^[@\\]aidetails \S+/ {
                    countaidetails++;
                    next;
                }
                !skip && /^[@\\]aidetails$/ {
                    printf("%s:%d warning: invalid @aidetails command:\"%s\"\n", 
                            filename, linenum, $0);
                    error=1;
                    next;
                }
                /^$/ {
                    # Blank line - indicates end of comment for this object
                    # End of function decl
                    if(!ainotused && !countaicommands) {
                        printf("%s:%d %s %s: warning: @ai command not found\n",
                                filename, startline, type, name);
                    }
                    if(ainotused && countaicommands) {
                        printf("%s:%d %s %s: warning: found @ai command(s) in addition to @ai Not Used\n", 
                                filename, startline, type, name);
                    }
                    if(ainotused && countaidetails) {
                        printf("%s:%d %s %s: warning: found @aidetails command(s) in addition to @ai Not Used\n", 
                                filename, startline, type, name);
                    }
                    if(countaicommands > 0 && countaidetails == 0) {
                        printf("%s:%d %s %s: warning: found @ai command(s) (besides @ai Not Used) without required @aidetails command(s)\n", 
                                filename, startline, type, name);
                    }
                    next;
                }

            '
    fi
done
