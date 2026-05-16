#include <stdio.h>
#include <stdlib.h>
#include <execinfo.h>

#ifdef DEBUG

#define DEBUG_PRINT(fmt, ...) \
    fprintf(stderr, "[DEBUG] %s:%d: " fmt "\n", __FILE__, __LINE__, __VA_ARGS__)

#else

#define DEBUG_PRINT(fmt, ...) do {} while (0)

#endif



























//! MUST BE ENSURE THE DEBUG_PRINT("x=%d", x) AT THE 48 LINE

// 测试代码
void test() {
    int x = 42;
    DEBUG_PRINT("x=%d", x);
}

int main() {
    test();
    return 0;
}