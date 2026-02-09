void fn(int a, int b)
{
    int i, j; // Multiple declarations on one line are fine provided
              // the variables aren't being initialised with non-
              // constant expressions
    int x = 0, y = 1; // Initialisations to constants can be put on one line
    int product = a * b; // Declaring one variable with a non-constant initialiser is fine
    // Don't do this: int sum = a + b, diff = a-b;
    // Do this (or declare and initialise separately):
    int sum = a + b;
    int diff = a - b;
}
