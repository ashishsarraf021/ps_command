#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <ctype.h>
#include <unistd.h>
#include <time.h>

// Function to check if a string is numeric
int is_numeric(const char *str) {
    while (*str) {
        if (!isdigit(*str)) {
            return 0;
        }
        str++;
    }
    return 1;
}

// Function to get the TTY (terminal) associated with the process
void get_tty(const char *pid, char *tty) {
    char path[256];
    snprintf(path, sizeof(path), "/proc/%s/fd/0", pid);

    if (access(path, F_OK) != -1) {
        snprintf(tty, 32, "tty%s", pid); // For simplicity, using "tty" + pid as a placeholder
    } else {
        strcpy(tty, "?");
    }
}

// Function to calculate total CPU time (user + system) in the format mm:ss
void get_cpu_time(const char *pid, char *time_str) {
    char path[256], buffer[256];
    unsigned long utime, stime;
    FILE *file;

    snprintf(path, sizeof(path), "/proc/%s/stat", pid);
    if ((file = fopen(path, "r")) != NULL) {
        fscanf(file, "%*d %*s %*c %*d %*d %*d %*d %*d %*u %*u %*u %*u %*u %*u %lu %lu", &utime, &stime);
        fclose(file);
    } else {
        strcpy(time_str, "00:00");
        return;
    }

    unsigned long total_time = (utime + stime) / sysconf(_SC_CLK_TCK);  // Convert to seconds
    snprintf(time_str, 16, "%02lu:%02lu", total_time / 60, total_time % 60);
}

// Function to display process information in the desired format
void display_process_info(const char *pid) {
    char path[256], cmd[256], tty[32], time_str[16];
    FILE *file;

    // Construct the path to the process status file
    snprintf(path, sizeof(path), "/proc/%s/stat", pid);

    // Open the file to read process information
    if ((file = fopen(path, "r")) != NULL) {
        fscanf(file, "%*d %255s", cmd);  // Read command name
        cmd[strlen(cmd) - 1] = '\0'; // Remove the trailing parenthesis
        memmove(cmd, cmd + 1, strlen(cmd)); // Remove the leading parenthesis
        fclose(file);
    } else {
        strcpy(cmd, "?");
    }

    get_tty(pid, tty);  // Get TTY information
    get_cpu_time(pid, time_str);  // Get CPU time information

    printf("%5s %10s %8s %s\n", pid, tty, time_str, cmd);  // Print PID, TTY, TIME, CMD
}

// Function to implement `ps -a` behavior
void ps_a() {
    DIR *dir;
    struct dirent *entry;

    // Open the /proc directory
    if ((dir = opendir("/proc")) == NULL) {
        perror("opendir");
        return;
    }

    printf("%5s %10s %8s %s\n", "PID", "TTY", "TIME", "CMD");

    // Read each entry in the /proc directory
    while ((entry = readdir(dir)) != NULL) {
        // Check if the entry name is numeric (indicating a process)
        if (is_numeric(entry->d_name)) {
            // Check if the process is associated with a terminal
            char tty_path[256];
            snprintf(tty_path, sizeof(tty_path), "/proc/%s/fd/0", entry->d_name);
            if (access(tty_path, F_OK) != -1) {
                display_process_info(entry->d_name);
            }
        }
    }

    closedir(dir);
}

// Function to implement `ps -e` behavior
void ps_e() {
    DIR *dir;
    struct dirent *entry;

    // Open the /proc directory
    if ((dir = opendir("/proc")) == NULL) {
        perror("opendir");
        return;
    }

    printf("%5s %10s %8s %s\n", "PID", "TTY", "TIME", "CMD");

    // Read each entry in the /proc directory
    while ((entry = readdir(dir)) != NULL) {
        // Check if the entry name is numeric (indicating a process)
        if (is_numeric(entry->d_name)) {
            display_process_info(entry->d_name);
        }
    }

    closedir(dir);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s -a or %s -e\n", argv[0], argv[0]);
        return 1;
    }

    // Handle command-line arguments
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
