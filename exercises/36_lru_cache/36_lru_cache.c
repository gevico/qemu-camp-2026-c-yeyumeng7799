#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * 16 LRU 缓存淘汰算法（哈希表 + 双向链表）
 *  - put(k,v)、get(k) 均为 O(1)
 *  - 容量满时淘汰最久未使用（LRU）的元素
 */

typedef struct LRUNode {
    int key;
    int value;
    struct LRUNode* prev;
    struct LRUNode* next;
} LRUNode;

typedef struct HashEntry {
    int key;
    LRUNode* node;
    struct HashEntry* next;
} HashEntry;

typedef struct {
    int capacity;
    int size;
    LRUNode* head; /* 最近使用（MRU） */
    LRUNode* tail; /* 最久未使用（LRU） */
    /* 简单链式哈希表 */
    size_t bucket_count;
    HashEntry** buckets;
} LRUCache;

static unsigned hash_int(int key) {
    return (unsigned)(key ^ (key >> 16));
}

static HashEntry* hash_find(LRUCache* c, int key, HashEntry*** pprev_next) {
    unsigned idx = hash_int(key) % c->bucket_count;
    HashEntry** pprev = &c->buckets[idx];
    HashEntry* entry = *pprev;
    while (entry) {
        if (entry->key == key) {
            if (pprev_next) *pprev_next = pprev;
            return entry;
        }
        pprev = &entry->next;
        entry = *pprev;
    }
    if (pprev_next) *pprev_next = pprev;
    return NULL;
}

static void list_add_to_head(LRUCache* c, LRUNode* node) {
    node->prev = NULL;
    node->next = c->head;
    if (c->head) c->head->prev = node;
    else c->tail = node;
    c->head = node;
}

static void list_remove(LRUCache* c, LRUNode* node) {
    if (node->prev) node->prev->next = node->next;
    else c->head = node->next;
    if (node->next) node->next->prev = node->prev;
    else c->tail = node->prev;
    node->prev = node->next = NULL;
}

static void list_move_to_head(LRUCache* c, LRUNode* node) {
    list_remove(c, node);
    list_add_to_head(c, node);
}

static LRUNode* list_pop_tail(LRUCache* c) {
    if (!c->tail) return NULL;
    LRUNode* node = c->tail;
    list_remove(c, node);
    return node;
}

static LRUCache* lru_create(int capacity) {
    LRUCache* c = malloc(sizeof(LRUCache));
    c->capacity = capacity;
    c->size = 0;
    c->head = c->tail = NULL;
    c->bucket_count = (size_t)(capacity * 2);
    c->buckets = calloc(c->bucket_count, sizeof(HashEntry*));
    return c;
}

static void lru_free(LRUCache* c) {
    LRUNode* p = c->head;
    while (p) {
        LRUNode* next = p->next;
        free(p);
        p = next;
    }
    for (size_t i = 0; i < c->bucket_count; i++) {
        HashEntry* e = c->buckets[i];
        while (e) {
            HashEntry* next = e->next;
            free(e);
            e = next;
        }
    }
    free(c->buckets);
    free(c);
}

static int lru_get(LRUCache* c, int key, int* out_value) {
    HashEntry* entry = hash_find(c, key, NULL);
    if (!entry) return 0;
    list_move_to_head(c, entry->node);
    if (out_value) *out_value = entry->node->value;
    return 1;
}

static void lru_put(LRUCache* c, int key, int value) {
    HashEntry* entry = hash_find(c, key, NULL);
    if (entry) {
        entry->node->value = value;
        list_move_to_head(c, entry->node);
        return;
    }
    
    if (c->size >= c->capacity) {
        LRUNode* tail = list_pop_tail(c);
        if (tail) {
            HashEntry** pprev;
            hash_find(c, tail->key, &pprev);
            HashEntry* old = *pprev;
            *pprev = old->next;
            free(old);
            free(tail);
            c->size--;
        }
    }
    
    LRUNode* node = malloc(sizeof(LRUNode));
    node->key = key;
    node->value = value;
    list_add_to_head(c, node);
    
    HashEntry* new_entry = malloc(sizeof(HashEntry));
    new_entry->key = key;
    new_entry->node = node;
    unsigned idx = hash_int(key) % c->bucket_count;
    new_entry->next = c->buckets[idx];
    c->buckets[idx] = new_entry;
    c->size++;
}

static void lru_print(LRUCache* c) {
    LRUNode* p = c->head;
    int first = 1;
    while (p) {
        if (!first) printf(", ");
        first = 0;
        printf("%d:%d", p->key, p->value);
        p = p->next;
    }
    printf("\n");
}

int main(void) {
    /* 容量 3：put(1,1), put(2,2), put(3,3), put(4,4), get(2), put(5,5) */
    LRUCache* c = lru_create(3);
    if (!c) {
        fprintf(stderr, "创建 LRU 失败\n");
        return 1;
    }

    lru_put(c, 1, 1); /* 缓存：1 */
    lru_put(c, 2, 2); /* 缓存：2,1 */
    lru_put(c, 3, 3); /* 缓存：3,2,1 (满) */
    lru_put(c, 4, 4); /* 淘汰 LRU(1)，缓存：4,3,2 */

    int val;
    if (lru_get(c, 2, &val)) {
        /* 访问 2：缓存：2,4,3 */
        (void)val; /* 演示无需使用 */
    }

    lru_put(c, 5, 5); /* 淘汰 LRU(3)，缓存：5,2,4 */

    /* 期望最终键集合：{2,4,5}，顺序无关。此处按最近->最久打印：5:5, 2:2, 4:4 */
    lru_print(c);

    lru_free(c);
    return 0;
}
