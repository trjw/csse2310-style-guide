if (month >= 1 && month <= 3) {
    // comments on a line by themselves must have the same indenting level
    // as code would have
    quarter = 1;
} else if (month >= 4 && month <= 6) {
    quarter = 2; // multi-line comments can be aligned like this - i.e. the
                 // comment continuation lines up with the start of the comment
                 // in the line above
} else if (month >= 7 && month <= 9) {
    quarter = 3; // multi-line comments can also be aligned like this - i.e
    // the continuation is lined up with the start of the previous
    // statement
} else {
    quarter = 4;
    /* The starting position for Comments that start with /* is the same as
       for comments that start with //, but
                continuation lines
                        can use any level
                                of indenting */
}
