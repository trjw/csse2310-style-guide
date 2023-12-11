// Initialiser list fits on the same line as the declaration:
double d1[10] = { 1.11111111, 2.222222, 3.333333, 4.4444444, 5.555555 };
struct Parameters1 param1 = { .initial = NULL, .end = NULL, .runningCount = 0 };
// Initialiser list fits by itself on a continuation line:
double d2[10]
    = { 1.11111111, 2.222222, 3.333333, 4.4444444, 5.555555, 6.666666 };
struct Parameters2 param2
    = { .initial = NULL, .end = NULL, .runningCount = 0, .total = 0 };
// Initialiser list doesn't fit on a single line (or continuation line):
double d3[10] = { 1.11111111, 2.222222, 3.333333, 4.4444444, 5.555555, 6.666666,
    7.777777, 8.888888, 9.9999999 };
struct Parameters3 param3 = { NULL, NULL, 0, NULL, 0, false, true, 0, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL };
// Named lists are formatted differently if they don't fit on one line:
struct Parameters4 param4 = { .initial = NULL,
    .end = NULL,
    .runningCount = 0,
    .fileName = NULL,
    .limit = 0,
    .upper = false,
    .lower = true,
    .length = 0,
    .userName = NULL };
