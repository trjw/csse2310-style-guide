// The function:
char first_char(char* p)
{
    return *p;
}

// would be safer written as the following. It makes it clearer that the
// string pointed to by p will not be modified by the function.
char first_char(const char* p)
{
    return *p;
}
