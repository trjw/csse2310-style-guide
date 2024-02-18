/* Calculates and returns the sum of integers from 1 to n. If n is less than
 * or equal to 0 then 0 is returned. It is assumed that the sum does not
 * overflow - the result will be incorrect if it does.
 */
int sum(int n)
{
    int i, s = 0;
    for (i = 1; i <= n; i++) {
        s += i;
    }
    return s;
}
