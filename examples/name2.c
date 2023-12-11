int main(int argc, char* argv[]);
void reset_secret_string(void);
// The following is a variable declaration for a function pointer
int (*sortingFunction)(void*, void*) = alpha_sort;
