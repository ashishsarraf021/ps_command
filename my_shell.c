#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>

#define MAX_CMD_LEN 1024
#define MAX_ARGS 64

// Function to parse command and its arguments
void parseCommand(char *cmd, char **args) {
    while (*cmd != '\0') {
        while (*cmd == ' ' || *cmd == '\t' || *cmd == '\n') {
            *cmd++ = '\0';  // Replace whitespace with null characters
        }
        *args++ = cmd;  // Save the argument position
        while (*cmd != '\0' && *cmd != ' ' && *cmd != '\t' && *cmd != '\n') {
            cmd++;
        }
    }
    *args = NULL;  // Null terminate the argument list
}

// Function to execute the command
void executeCommand(char **args) {
    pid_t pid = fork();  // Fork a new process
    if (pid < 0) {
        perror("Fork failed");
        exit(1);
    }
    if (pid == 0) {  // Child process
        if (execvp(args[0], args) < 0) {
            perror("Command execution failed");
            exit(1);
        }
    } else {  // Parent process
        wait(NULL);  // Wait for the child process to finish
    }
}

// Function to handle I/O redirection
void handleRedirection(char **args) {
    int fd;
    for (int i = 0; args[i] != NULL; i++) {
        if (strcmp(args[i], ">") == 0) {
            // Output redirection (overwrite)
            args[i] = NULL;
            fd = open(args[i + 1], O_CREAT | O_WRONLY | O_TRUNC, 0644);
            if (fd < 0) {
                perror("Open failed");
                exit(1);
            }
            dup2(fd, STDOUT_FILENO);  // Redirect stdout to file
            close(fd);
            break;
        } else if (strcmp(args[i], ">>") == 0) {
            // Output redirection (append)
            args[i] = NULL;
            fd = open(args[i + 1], O_CREAT | O_WRONLY | O_APPEND, 0644);
            if (fd < 0) {
                perror("Open failed");
                exit(1);
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
            break;
        } else if (strcmp(args[i], "<") == 0) {
            // Input redirection
            args[i] = NULL;
            fd = open(args[i + 1], O_RDONLY);
            if (fd < 0) {
                perror("Open failed");
                exit(1);
            }
            dup2(fd, STDIN_FILENO);  // Redirect stdin to file
            close(fd);
            break;
        }
    }
}

// Main command loop
int main() {
    char cmd[MAX_CMD_LEN];  // Command string
    char *args[MAX_ARGS];   // Argument array

    while (1) {
        printf("myshell> ");  // Prompt
        fgets(cmd, MAX_CMD_LEN, stdin);  // Read input
        cmd[strlen(cmd) - 1] = '\0';  // Remove newline character

        if (strcmp(cmd, "exit") == 0) {
            break;  // Exit the shell
        }

        parseCommand(cmd, args);  // Parse command and arguments
        handleRedirection(args);  // Handle I/O redirection if any
        executeCommand(args);     // Execute the command
    }

    return 0;
}
