// For loop where parenthesised expressions fit on one line
for (i = 0; i < count; i++) {
    // Statements within the loop are indented by four
    do_stuff();
}
// For loop where expressions do not fit on one line. The start of the next
// expression is indented by five characters to align with the first expression.
for (loopCount = 0; loopCount < numberOfElementsToCheck && !isStopCondition;
     loopCount++) {
    // Statements within the loop are still indented by four
    process(item[loopCount]);
}
// If the initialisation expression (an assignment expression) overflows
// then it is indented by 9. An overflowing test expression is not further
// indented if it is not nested
for (var = initial_value_of_loop_counter(argument1, argument2)
         + adjustment_factor_to_apply();
     var < numberOfElementsToCheck && haveNotFinishedYet()
     && definitelyHaveNotFinishedYet();
     var++) {
    process();
}
// If the initialisation expression uses the comma operator then
// indentation is by 4 characters.
for (loopCountVariableWithVeryLongName = 0,
    anotherVariableWithLongNameInitialisedHere = 2;
     // Test expression indented by 5 to line up with initialisation expression.
     // The subexpression is further indented by 4.
     loopCount < numberOfElementsToCheck
     && !(isStopConditionA || isStopConditionB || isStopConditionC
         || isStopConditionC);
     loopCount++) {
    process();
}
