#include <stdio.h>      // Standard input/output library for printf and fgets
#include <stdlib.h>     // Standard library for memory allocation, process control, etc.
#include <unistd.h>     // UNIX standard functions, including fork and exec
#include <fcntl.h>      // File control options for opening files
#include <string.h>     // String manipulation functions like strcmp
#include <sys/wait.h>   // For wait function to wait for child processes

#define MAX_CMD_LEN 1024  // Maximum length for command input
#define MAX_ARGS 64        // Maximum number of arguments for a command

// Function to parse command and its arguments
void parseCommand(char *cmd, char **args) {
    // Loop through the command string until the end
    while (*cmd != '\0') {
        // Skip whitespace characters and replace them with null characters
        while (*cmd == ' ' || *cmd == '\t' || *cmd == '\n') {
            *cmd++ = '\0';  // Replace whitespace with null terminators
        }
        *args++ = cmd;  // Save the position of the current argument
        // Move to the next argument
        while (*cmd != '\0' && *cmd != ' ' && *cmd != '\t' && *cmd != '\n') {
            cmd++;
        }
    }
    *args = NULL;  // Null terminate the argument list to mark the end
}

// Function to execute the command
void executeCommand(char **args) {
    pid_t pid = fork();  // Create a new process
    if (pid < 0) {
        perror("Fork failed");  // Print error if fork fails
        exit(1);                // Exit if fork fails
    }
    if (pid == 0) {  // Child process
        if (execvp(args[0], args) < 0) {  // Execute the command
            perror("Command execution failed");  // Print error if execution fails
            exit(1);  // Exit child process if execution fails
        }
    } else {  // Parent process
        wait(NULL);  // Wait for the child process to finish executing
    }
}

// Function to handle I/O redirection
void handleRedirection(char **args) {
    int fd;  // File descriptor for opening files
    // Loop through arguments to find redirection symbols
    for (int i = 0; args[i] != NULL; i++) {
        if (strcmp(args[i], ">") == 0) {  // Output redirection (overwrite)
            args[i] = NULL;  // Null terminate the args list
            fd = open(args[i + 1], O_CREAT | O_WRONLY | O_TRUNC, 0644);  // Open the file
            if (fd < 0) {  // Check for errors in opening the file
                perror("Open failed");  // Print error
                exit(1);  // Exit if open fails
            }
            dup2(fd, STDOUT_FILENO);  // Redirect standard output to the file
            close(fd);  // Close the file descriptor
            break;  // Break out of the loop after redirection
        } else if (strcmp(args[i], ">>") == 0) {  // Output redirection (append)
            args[i] = NULL;  // Null terminate the args list
            fd = open(args[i + 1], O_CREAT | O_WRONLY | O_APPEND, 0644);  // Open the file in append mode
            if (fd < 0) {  // Check for errors
                perror("Open failed");  // Print error
                exit(1);  // Exit if open fails
            }
            dup2(fd, STDOUT_FILENO);  // Redirect standard output to the file
            close(fd);  // Close the file descriptor
            break;  // Break out of the loop after redirection
        } else if (strcmp(args[i], "<") == 0) {  // Input redirection
            args[i] = NULL;  // Null terminate the args list
            fd = open(args[i + 1], O_RDONLY);  // Open the file for reading
            if (fd < 0) {  // Check for errors
                perror("Open failed");  // Print error
                exit(1);  // Exit if open fails
            }
            dup2(fd, STDIN_FILENO);  // Redirect standard input from the file
            close(fd);  // Close the file descriptor
            break;  // Break out of the loop after redirection
        }
    }
}

// Main command loop
int main() {
    char cmd[MAX_CMD_LEN];  // Command string buffer
    char *args[MAX_ARGS];   // Argument array for command arguments

    while (1) {  // Infinite loop to keep the shell running
        printf("myshell> ");  // Display shell prompt
        fgets(cmd, MAX_CMD_LEN, stdin);  // Read input from user
        cmd[strlen(cmd) - 1] = '\0';  // Remove the newline character from input

        if (strcmp(cmd, "exit") == 0) {  // Check for exit command
            break;  // Exit the shell loop
        }

        parseCommand(cmd, args);  // Parse the command and arguments
        handleRedirection(args);  // Handle any I/O redirection
        executeCommand(args);     // Execute the parsed command
    }

    return 0;  // Return 0 on successful termination of the program
}
// Summary of Key Functions:
// parseCommand: This function splits the command string into individual arguments and handles whitespace.
// executeCommand: This function creates a new process to execute the command using execvp.
// handleRedirection: This function processes input/output redirection based on the specified symbols (>, >>, and <).
// main: The main loop displays the shell prompt, reads user input, and executes commands until the user types "exit".
// This implementation allows for simple command execution along with basic I/O redirection in a shell-like environment.
