/** ***************************************************************************
 * @fn print_usage_and_exit(const char *, int)
 *
 * Print usage error message to stderr and exit.
 *
 * @param program_name  The name of the program to include in the error message.
 * @param exit_code     The exit code to use when exiting the program.
 *
 * @ai Wrote Code
 * @aidetails ChatGPT wrote the whole function.
 *
 * @returns never
 *****************************************************************************/
void print_usage_and_exit(const char *program_name, int exit_code) {
    fprintf(stderr, "Usage: %s <integer> <string>\n", program_name);
    exit(exit_code);
}
