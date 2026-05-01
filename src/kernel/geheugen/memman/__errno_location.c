
int* __errno_location(void) {
    static int _errno;
    return &_errno;
}
