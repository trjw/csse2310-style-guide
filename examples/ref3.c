/* string_comparator()
 * -------------------
 * qsort() comparator function that compares two strings.
 *
 * str1, str2: pointers to strings (char*) that we're comparing.
 *
 * Returns: an integer less then, equal to, or greater than zero if the string
 *    pointed to by  str1 is less than, equal to or greater than the string
 *    pointed to by str2
 * REF: This function is taken from the qsort(3) man page. The name of the
 * REF: function & spacing have been modified to comply with the style guide.
 */
static int string_comparator(const void* str1, const void* str2)
{
    // Compare the strings. Note that we must cast the void* pointers
    // to pointers to strings, and then dereference them
    return strcmp(*(char* const*)str1, *(char* const*)str2);
}
