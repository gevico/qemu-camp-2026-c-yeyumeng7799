#include <stdio.h>

unsigned int gcd_asm(unsigned int a, unsigned int b) {
    unsigned int result;
    
    __asm__ volatile (
        "mov %1, %%eax\n\t"
        "mov %2, %%ebx\n\t"
        "jmp .L_check\n\t"
        
        ".L_loop:\n\t"
        "   xor %%edx, %%edx\n\t"
        "   div %%ebx\n\t"
        "   mov %%ebx, %%eax\n\t"
        "   mov %%edx, %%ebx\n\t"
        
        ".L_check:\n\t"
        "   test %%ebx, %%ebx\n\t"
        "   jne .L_loop\n\t"
        
        "mov %%eax, %0"
        : "=r" (result)
        : "r" (a), "r" (b)
        : "eax", "ebx", "edx"
    );
    
    return result;
}

int main(int argc, char* argv[]) {
    printf("%d\n", gcd_asm(12, 8));
    printf("%d\n", gcd_asm(7, 5));
    return 0;
}