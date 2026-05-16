#include <stdio.h>
#include <stdlib.h>

/*
 * 19 字符串分割器（类似 strtok_r）
 * 目标：实现线程安全的字符串分割函数，不依赖静态全局状态
 * 原型：char* strtok_r(char *str, const char *delim, char **saveptr)
 * 示例：输入 "hello,world test"，分隔符 ", " → 依次得到 hello、world、test
 */

/* 判断字符 c 是否在分隔符集合 delim 中 */
static int is_delim(char c, const char *delim) {
    while (*delim) {
        if (c == *delim) return 1;
        delim++;
    }
    return 0;
}

char *strtok_r(char *str, const char *delim, char **saveptr) {
    char *p;
    
    if (str) {
        p = str;
    } else {
        p = *saveptr;
    }
    
    while (*p && is_delim(*p, delim)) {
        p++;
    }
    
    if (!*p) {
        *saveptr = p;
        return NULL;
    }
    
    char *token = p;
    while (*p && !is_delim(*p, delim)) {
        p++;
    }
    
    if (*p) {
        *p = '\0';
        *saveptr = p + 1;
    } else {
        *saveptr = p;
    }
    
return token;
}

int main(void) {
    char buf[] = "hello,world test";
    const char *delim = ", ";
    char *save = NULL;

    char *tok = strtok_r(buf, delim, &save);
    while (tok) {
        printf("%s\n", tok);
        tok = strtok_r(NULL, delim, &save);
    }
    return 0;
}
