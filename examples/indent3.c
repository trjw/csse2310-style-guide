// If the expression fits on one line then split it like this:
int physAddress
        = page_table_lookup(vAddress / PAGE_SIZE) + (vAddress % PAGE_SIZE);
// rather than like this:
// int physAddress = page_table_lookup(vAddress / PAGE_SIZE)
//         + (vAddress % PAGE_SIZE);
// But if the whole expression doesn't fit on one line then split it like this:
int physicalAddress = page_table_lookup(virtualAddress / PAGE_SIZE)
        + (virtualAddress % PAGE_SIZE);
// Expressions should be split so that operators are at the start of the
// continuation line, not at the end:
int physicalAddress
        = page_table_lookup(virtualAddressVariableWithLongerName / PAGE_SIZE)
        + (virtualAddress % PAGE_SIZE);
// If function parameters can be grouped with the function name then do so
// rather than split them:
int result1
        = function_that_takes_two_fn_parameters(parameter_one, parameter_two);
// i.e. not like this:
// int result1 = function_that_takes_two_fn_parameters(
//         parameter_one, parameter_two);
// But if the parameters can't be grouped with the function name on one line
// but can be grouped with each other, then do that:
int result2 = function_that_takes_three_parameters(
        parameter_one, parameter_two, param3);
// If they can't be grouped either way then don't try:
int result3 = function_that_takes_even_more_parameters(parameter_one,
        parameter_two, parameter_three, parameter_four, parameter_five,
        parameter_six, parameter_seven, parameter_eight);
// Continuation lines for loop expressions are indented by 8 also:
while (!isLoopFinished(argument1) && !isLoopFinished(argument2)
        && !isLoopFinished(argument3)) {
    // but the statements within the loop are indented by 4:
    process(value);
}
