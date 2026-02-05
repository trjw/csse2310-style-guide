void fn(int a, int b)
{
    int sum, diff;

    // Don't use a comma where you can use a semicolon and multiple lines:
    // Not this: sum = a + b, diff = a - b;
    // Do this:
    sum = a + b;
    diff = a - b;
    for (i = 0, j = 0; i < sum; i++, j++) { // Comma operators OK here
        // code ...
    }
    while (i = 0, j < b) { // Comma operators OK here also
        // code ...
    }
}
