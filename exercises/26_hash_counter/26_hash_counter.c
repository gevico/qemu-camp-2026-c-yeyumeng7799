#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TABLE_SIZE 1024  // 哈希表大小

// 哈希表节点结构
typedef struct HashNode {
    char *word;
    int count;
    struct HashNode *next;
} HashNode;

// 哈希表结构
typedef struct {
    HashNode **table;
    int size;
} HashTable;

// djb2哈希函数
unsigned long djb2_hash(const char *str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}

HashTable *create_hash_table(int size) {
    HashTable *ht = malloc(sizeof(HashTable));
    ht->table = calloc(size, sizeof(HashNode *));
    ht->size = size;
    return ht;
}

void hash_table_insert(HashTable *ht, const char *word) {
    unsigned long hash = djb2_hash(word) % ht->size;

    HashNode *node = ht->table[hash];
    while (node) {
        if (strcmp(node->word, word) == 0) {
            node->count++;
            return;
        }
        node = node->next;
    }

    HashNode *new_node = malloc(sizeof(HashNode));
    new_node->word = strdup(word);
    new_node->count = 1;
    new_node->next = ht->table[hash];
    ht->table[hash] = new_node;
}

void get_all_words(HashTable *ht, HashNode **nodes, int *count) {
    *count = 0;
    for (int i = 0; i < ht->size; i++) {
        HashNode *node = ht->table[i];
        while (node) {
            nodes[(*count)++] = node;
            node = node->next;
        }
    }
}

int compare_nodes(const void *a, const void *b) {
    HashNode *node_a = *(HashNode **)a;
    HashNode *node_b = *(HashNode **)b;
    
    if (node_a->count != node_b->count) {
        return node_b->count - node_a->count;
    }
    return strcmp(node_a->word, node_b->word);
}

char *get_next_word(const char **text) {
    const char *p = *text;
    
    while (*p && !isalpha(*p)) p++;
    
    if (!*p) return NULL;
    
    const char *start = p;
    while (*p && isalpha(*p)) p++;
    
    int len = p - start;
    char *word = malloc(len + 1);
    strncpy(word, start, len);
    word[len] = '\0';
    
    for (int i = 0; word[i]; i++) {
        word[i] = tolower(word[i]);
    }
    
    *text = p;
    return word;
}

void free_hash_table(HashTable *ht) {
    for (int i = 0; i < ht->size; i++) {
        HashNode *node = ht->table[i];
        while (node != NULL) {
            HashNode *temp = node;
            node = node->next;
            free(temp->word);
            free(temp);
        }
    }
    free(ht->table);
    free(ht);
}

int main(int argc, char *argv[]) {
    const char* file_path = "paper.txt";

    FILE *file = fopen(file_path, "r");
    if (file == NULL) {
        perror("无法打开文件");
        return 1;
    }

    HashTable *ht = create_hash_table(TABLE_SIZE);
    char buffer[4096];
    
    printf("正在读取文件: %s\n", file_path);
    
    // 从文件读取直到EOF
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        const char *ptr = buffer;
        char *word;
        
        while ((word = get_next_word(&ptr)) != NULL) {
            hash_table_insert(ht, word);
            free(word);
        }
    }
    
    fclose(file);
    
    // 收集所有单词节点
    HashNode **nodes = malloc(TABLE_SIZE * sizeof(HashNode *));
    int node_count = 0;
    get_all_words(ht, nodes, &node_count);
    
    // 排序
    qsort(nodes, node_count, sizeof(HashNode *), compare_nodes);
    
    // 输出结果
    printf("\n单词统计结果（按频率降序排列）:\n");
    printf("%-20s %s\n", "单词", "出现次数");
    printf("-------------------- ----------\n");
    for (int i = 0; i < node_count; i++) {
        printf("%s:%d\n", nodes[i]->word, nodes[i]->count);
    }
    
    // 释放内存
    free(nodes);
    free_hash_table(ht);
    
    return 0;
}