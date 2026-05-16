#include "singly_linked_list.h"

#include <stdio.h>
#include <stdlib.h>

// 全局头指针
static link head = NULL;

// 创建新节点
link make_node(unsigned char item) {
    link p = malloc(sizeof(struct node));
    p->item = item;
    p->next = NULL;
    return p;
}

link search(unsigned char key) {
    link p = head;
    while (p) {
        if (p->item == key) return p;
        p = p->next;
    }
    return NULL;
}

void insert(link p) {
    p->next = head;
    head = p;
}

void delete(link p) {
    if (p == head) {
        head = head->next;
        free_node(p);
        return;
    }
    link prev = head;
    while (prev && prev->next != p) prev = prev->next;
    if (prev) {
        prev->next = p->next;
        free_node(p);
    }
}

void traverse(void (*visit)(link)) {
    link p = head;
    while (p) {
        visit(p);
        p = p->next;
    }
}

void destroy(void) {
    link p = head;
    while (p) {
        link next = p->next;
        free_node(p);
        p = next;
    }
    head = NULL;
}

void push(link p) { 
    p->next = head;
    head = p;
}

link pop(void) {
    if (!head) return NULL;
    link p = head;
    head = head->next;
    p->next = NULL;
    return p;
}

void free_list(link list_head) {
    link p = list_head;
    while (p) {
        link next = p->next;
        free_node(p);
        p = next;
    }
}
