long long pow(long long a, long long b) { return 0; }        // Stub
long long frexp(long long x, int *e) { *e = 0; return 0; } // Stub
// In je kernel math_stubs.c
long long floor(long long x) {
    if (x >= 0) return (long long)((long)x);
    else return (long long)((long)x - 1);
}

long long fmod(long long a, long long b) {
    return a - (floor(a / b) * b);
}