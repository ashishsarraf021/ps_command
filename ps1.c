#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

// Function to check if a string is numeric (used for PID checking)
int is_numeric(const char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isdigit(str[i])) {
            return 0;
        }
    }
    return 1;
}

// Function to display the process details for a given PID
void display_process_info(const char *pid) {
    char path[40], line[100], *p;
    FILE *statusf;
    
    // Construct the path to the process's status file
    snprintf(path, 40, "/proc/%s/stat", pid);
    
    statusf = fopen(path, "r");
    if (!statusf) {
        return;
    }
    
    // Read the file contents and print process details
    if (fgets(line, 100, statusf)) {
        // Process ID, name, and state are the first three fields
        int pid;
        char comm[256], state;
        sscanf(line, "%d %s %c", &pid, comm, &state);
        printf("%5d %s %c\n", pid, comm, state);
    }
    
    fclose(statusf);
}

// Function to implement ps -e
void ps_e() {
    struct dirent *entry;
    DIR *dir = opendir("/proc");
    
    if (!dir) {
        perror("opendir");
        return;
    }
    
    printf("%5s %20s %s\n", "PID", "COMMAND", "STATE");
    
    while ((entry = readdir(dir)) != NULL) {
        if (is_numeric(entry->d_name)) {
            display_process_info(entry->d_name);
        }
    }
    
    closedir(dir);
}

// Function to implement ps -a
void ps_a() {
    struct dirent *entry;
    DIR *dir = opendir("/proc");
    
    if (!dir) {
        perror("opendir");
        return;
    }
    
    printf("%5s %20s %s\n", "PID", "COMMAND", "STATE");
    
    while ((entry = readdir(dir)) != NULL) {
        if (is_numeric(entry->d_name)) {
            // Only display processes associated with a terminal
            char tty_path[256];
            snprintf(tty_path, sizeof(tty_path), "/proc/%s/fd/0", entry->d_name);
            if (access(tty_path, F_OK) != -1) {
                display_process_info(entry->d_name);
            }
        }
    }
    
    closedir(dir);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s -a or %s -e\n", argv[0], argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "-a") == 0) {
        ps_a();
    } else if (strcmp(argv[1], "-e") == 0) {
        ps_e();
    } else {
        fprintf(stderr, "Invalid option. Use -a or -e.\n");
        return 1;
    }

    return 0;
}