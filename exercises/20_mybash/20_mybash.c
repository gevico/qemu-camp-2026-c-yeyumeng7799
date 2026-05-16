#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_LINE 1024
#define MAX_ARGS 64

int main(int argc, char *argv[]) {
    char line[MAX_LINE];
    char *args[MAX_ARGS];
    
    while (1) {
        printf("mybash> ");
        fflush(stdout);
        
        if (fgets(line, MAX_LINE, stdin) == NULL) {
            break;
        }
        
        line[strcspn(line, "\n")] = '\0';
        
        if (strlen(line) == 0) {
            continue;
        }
        
        if (strcmp(line, "exit") == 0) {
            break;
        }
        
        int arg_count = 0;
        char *token = strtok(line, " ");
        while (token != NULL && arg_count < MAX_ARGS - 1) {
            args[arg_count++] = token;
            token = strtok(NULL, " ");
        }
        args[arg_count] = NULL;
        
        if (arg_count == 0) {
            continue;
        }
        
        pid_t pid = fork();
        if (pid == 0) {
            execvp(args[0], args);
            perror("execvp");
            exit(1);
        } else if (pid > 0) {
            wait(NULL);
        } else {
            perror("fork");
        }
    }
    
    return 0;
}