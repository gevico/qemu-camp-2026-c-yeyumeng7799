#include <stdio.h>
#include <limits.h>

#define CHECK_OVERFLOW(carry) \
    carry ? "Overflow" : "Not Overflow"

int check_add_overflow_asm(unsigned int a, unsigned int b) {
    unsigned char carry;
    __asm__ volatile(
        "mov %2, %%eax\n\t"
        "add %3, %%eax\n\t"
        "setc %0"
        : "=r" (carry)
        : "0" (carry), "r" (a), "r" (b)
        : "eax"
    );
    return carry;
}

int check_sub_overflow_asm(unsigned int a, unsigned int b) {
    unsigned char carry;
    __asm__ volatile(
        "mov %2, %%eax\n\t"
        "sub %3, %%eax\n\t"
        "setc %0"
        : "=r" (carry)
        : "0" (carry), "r" (a), "r" (b)
        : "eax"
    );
    return carry;
}

int check_mul_overflow_asm(unsigned int a, unsigned int b) {
    unsigned int high_bits;
    unsigned char overflow;
    __asm__ volatile(
        "mov %3, %%eax\n\t"
        "mul %4\n\t"
        "mov %%edx, %0\n\t"
        "seto %1"
        : "=r" (high_bits), "=r" (overflow)
        : "0" (high_bits), "r" (a), "r" (b)
        : "eax", "edx"
    );
    return overflow || (high_bits != 0);
}

int check_div_overflow_asm(unsigned int a, unsigned int b) {
    unsigned char is_div_zero;
    __asm__ volatile(
        "cmp $0, %2\n\t"
        "sete %0"
        : "=r" (is_div_zero)
        : "0" (is_div_zero), "r" (b)
    );
    return is_div_zero;
}

int main() {
    printf("(UINT_MAX + 1)Add: %s\n", CHECK_OVERFLOW(check_add_overflow_asm(UINT_MAX, 1)));   // 1
    printf("(1, 0)Add: %s\n", CHECK_OVERFLOW(check_add_overflow_asm(1, 0)));  
    printf("(0, 1)Sub: %s\n", CHECK_OVERFLOW(check_sub_overflow_asm(0, 1)));          // 1
    printf("(2, 1)Sub: %s\n", CHECK_OVERFLOW(check_sub_overflow_asm(2, 1)));
    printf("(UINT_MAX, 2)Mul: %s\n", CHECK_OVERFLOW(check_mul_overflow_asm(UINT_MAX, 2)));   // 1
    printf("(1, 2)Mul: %s\n", CHECK_OVERFLOW(check_mul_overflow_asm(1, 2)));
    printf("(10, 0)Div: %s\n", CHECK_OVERFLOW(check_div_overflow_asm(10, 0)));                         // 1
    printf("(2, 1)Div: %s\n", CHECK_OVERFLOW(check_div_overflow_asm(2, 1)));
    return 0;
}