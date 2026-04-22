#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STUDENTS 100
#define NAME_LEN     50

typedef struct {
    char name[NAME_LEN];
    int score;
} Student;

Student students[MAX_STUDENTS];

void quick_sort(int left, int right) {
   // 递归终止条件：左边界 >= 右边界
    if (left >= right) {
        return;
    }

    // 选取基准值（这里用最左边元素作为基准）
    int i = left;
    int j = right;
    Student pivot 
= students[left];

    // 分区操作
    while (i < j) {
        // 从右往左找：第一个比基准值大的元素（降序）
        while (i < j && students[j].score <= pivot.score) {
            j
--;
        }
        // 将找到的元素移到左边
        students
[i] = students[j];

        // 从左往右找：第一个比基准值小的元素（降序）
        while (i < j && students[i].score >= pivot.score) {
            i
++;
        }
        // 将找到的元素移到右边
        students
[j] = students[i];
    }

    // 基准值归位
    students
[i] = pivot;

    // 递归排序左半部分和右半部分
    quick_sort(left, i - 1);
    quick_sort(i + 1, right);
}

int main(void) {
    FILE *file = fopen("03_students.txt", "r");
    if (!file) {
        printf("错误：无法打开文件 03_students.txt\n");
        return 1;
    }

    int n;
    fscanf(file, "%d", &n);

    if (n <= 0 || n > MAX_STUDENTS) {
        printf("学生人数无效：%d（应为 1-%d）\n", n, MAX_STUDENTS);
        fclose(file);
        return 1;
    }

    for (int i = 0; i < n; i++) {
        fscanf(file, "%s %d", students[i].name, &students[i].score);
    }
    fclose(file);

    quick_sort(0, n - 1);

    // 输出结果
    printf("\n快速排序后按成绩从高到低排序的学生名单：\n");
    for (int i = 0; i < n; i++) {
        printf("%s %d\n", students[i].name, students[i].score);
    }

    return 0;
}