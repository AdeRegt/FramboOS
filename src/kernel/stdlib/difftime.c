#include "lualib.h"

long long difftime(time_t time1, time_t time0) {
    return (long long)(time1 - time0);
}