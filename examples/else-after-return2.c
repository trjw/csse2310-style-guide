void foo(int value) {
    int local = 0;
    for (int i = 0; i < 42; i++) {
        if (value == 1) {
            return;
        } 
        // This line can only be reached if the 
        // condition above is false anyway
        local++;

        if (value == 2) {
            continue; // Ditto
        }
        local++;
    }
}
