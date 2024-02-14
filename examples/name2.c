int main(int argc, char* argv[]);
void reset_secret_string(void);
// The following is a variable declaration for a function pointer
// (sortingFunction) that is initialised to point to the function alpha_sort()
int (*sortingFunction)(void*, void*) = alpha_sort;
