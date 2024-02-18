void foo(int value)
{
    int local = 0;
    for (int i = 0; i < 42; i++) {
        if (value == 1) {
            return;
        } else {
            local++;
        }

        if (value == 2) {
            continue;
        } else {
            local++;
        }
    }
}
