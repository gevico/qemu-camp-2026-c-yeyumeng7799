#include "circular_linked_list.h"

#include <stdio.h>
#include <stdlib.h>

Node* create_circular_list(int n) {
    if (n <= 0) return NULL;
    
    Node* head = malloc(sizeof(Node));
    head->id = 1;
    head->next = head;
    
    Node* tail = head;
    for (int i = 2; i <= n; i++) {
        Node* node = malloc(sizeof(Node));
        node->id = i;
        node->next = head;
        tail->next = node;
        tail = node;
    }
    return head;
}

void free_list(Node* head) {
    if (!head) return;
    
    Node* current = head;
    Node* next;
    do {
        next = current->next;
        free(current);
        current = next;
    } while (current != head);
}
