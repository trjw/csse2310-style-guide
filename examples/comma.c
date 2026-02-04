void fn(int a, int b)
{
    int sum, diff; // Commas in variable declarations are fine provided
                   // the variables aren't being initialised with a non
                   // constant expression at the same time
    int product = a * b; // Declaring one variable and initialising it is fine
    int x = 0, y = 1; // Initialisations to constants can be put on one line

    // Don't use a comma where you can use a semicolon and multiple lines:
    // Not this: sum = a + b, diff = a - b;
    // Do this:
    sum = a + b;
    diff = a - b;
    for (i = 0, j = 0; i < sum; i++, j++) { // Comma operators OK here
        // code ...
    }
}
