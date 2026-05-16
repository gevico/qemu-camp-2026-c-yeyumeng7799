#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

/**
 * URL参数解析器
 * 输入：包含http/https超链接的字符串
 * 输出：解析出所有的key-value键值对，每行显示一个
 */

int parse_url(const char* url) {
    int err = 0;

    const char *query = strchr(url, '?');
    if (!query) {
        err = 1;
        goto exit;
    }
    query++;

    const char *p = query;
    while (*p) {
        const char *key_start = p;
        while (*p && *p != '=' && *p != '&') p++;
        if (*p != '=') break;
        
        int key_len = p - key_start;
        char key[256];
        strncpy(key, key_start, key_len);
        key[key_len] = '\0';
        p++;

        const char *val_start = p;
        while (*p && *p != '&') p++;
        int val_len = p - val_start;
        char val[256];
        strncpy(val, val_start, val_len);
        val[val_len] = '\0';

        printf("%s: %s\n", key, val);

        if (*p == '&') p++;
    }

exit:
    return err;
}

int main() {
    const char* test_url = "https://cn.bing.com/search?name=John&age=30&city=New+York";

    printf("Parsing URL: %s\n", test_url);
    printf("Parameters:\n");

    parse_url(test_url);

    return 0;
}