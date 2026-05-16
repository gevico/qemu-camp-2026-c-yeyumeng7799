#include <stdio.h>
#include <stdbool.h>

#define MAX_PEOPLE 50

typedef struct {
    int id;
} People;

typedef struct {
    People data[MAX_PEOPLE];
    int head;
    int tail;
    int count;
} Queue;

int main() {
    Queue q;
    int total_people=50;
    int report_interval=5;

    q.head = 0;
    q.tail = 0;
    q.count = 0;

    for (int i = 0; i < total_people; i++) {
        q.data[i].id = i + 1;
        q.count++;
        q.tail = i;
    }

    int current = 0;
    while (q.count > 1) {
        current++;
        if (current == report_interval) {
            current = 0;
            q.head = (q.head + 1) % MAX_PEOPLE;
            q.count--;
        } else {
            People temp = q.data[q.head];
            q.head = (q.head + 1) % MAX_PEOPLE;
            q.tail = (q.tail + 1) % MAX_PEOPLE;
            q.data[q.tail] = temp;
        }
    }
    
    printf("最后剩下的人是: %d\n", q.data[q.head].id);

    return 0;
}