int physAddress
        = page_table_lookup(vAddress / PAGE_SIZE) + (vAddress % PAGE_SIZE);
int physicalAddress = page_table_lookup(virtualAddress / PAGE_SIZE)
        + (virtualAddress % PAGE_SIZE);
int physicalAddress
        = page_table_lookup(virtualAddressVariableWithLongerName / PAGE_SIZE)
        + (virtualAddress % PAGE_SIZE);
int result1
        = function_that_takes_two_fn_parameters(parameter_one, parameter_two);
int result2 = function_that_takes_three_parameters(
        parameter_one, parameter_two, param3);
int result3 = function_that_takes_even_more_parameters(parameter_one,
        parameter_two, parameter_three, parameter_four, parameter_five,
        parameter_six, parameter_seven, parameter_eight);
int result = function_one(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9,
        arg10, arg11, arg12);
int result = function_two(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9,
        arg10, arg11, arg12, arg13, arg14);
while (!isLoopFinished(argument1) && !isLoopFinished(argument2)
        && !isLoopFinished(argument3)) {
    process(value);
}
