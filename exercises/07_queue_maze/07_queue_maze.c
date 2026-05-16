#include <stdio.h>

#define MAX_ROW 5
#define MAX_COL 5

int maze[MAX_ROW][MAX_COL] = {
	0, 1, 0, 0, 0,
	0, 1, 0, 1, 0,
	0, 0, 0, 0, 0,
	0, 1, 1, 1, 0,
	0, 0, 0, 1, 0,
};

typedef struct {
    int row;
    int col;
} Position;

Position queue[MAX_ROW * MAX_COL];
int front = 0, rear = 0;

void enqueue(int row, int col) {
    queue[rear++] = (Position){row, col};
}

Position dequeue(void) {
    return queue[front++];
}

int is_empty(void) {
    return front == rear;
}

int visited[MAX_ROW][MAX_COL] = {0};

int main(void)
{
    int directions[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
    enqueue(0, 0);
    visited[0][0] = 1;

    while (!is_empty()) {
        Position current = dequeue();
        printf("(%d, %d)\n", current.row, current.col);

        if (current.row == MAX_ROW - 1 && current.col == MAX_COL - 1) {
            printf("到达终点!\n");
            break;
        }

        for (int i = 0; i < 4; i++) {
            int new_row = current.row + directions[i][0];
            int new_col = current.col + directions[i][1];

            if (new_row >= 0 && new_row < MAX_ROW &&
                new_col >= 0 && new_col < MAX_COL &&
                maze[new_row][new_col] == 0 &&
                visited[new_row][new_col] == 0) {
                visited[new_row][new_col] = 1;
                enqueue(new_row, new_col);
            }
        }
    }

    return 0;
}