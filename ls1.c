// #include <stdio.h>
// #include <stdlib.h>
// #include <dirent.h>
// #include <sys/stat.h>
// #include <time.h>
// #include <string.h>
// #include <unistd.h>
// #include <pwd.h>
// #include <grp.h>

// void print_permissions(mode_t mode) {
//     printf( (S_ISDIR(mode)) ? "d" : "-");
//     printf( (mode & S_IRUSR) ? "r" : "-");
//     printf( (mode & S_IWUSR) ? "w" : "-");
//     printf( (mode & S_IXUSR) ? "x" : "-");
//     printf( (mode & S_IRGRP) ? "r" : "-");
//     printf( (mode & S_IWGRP) ? "w" : "-");
//     printf( (mode & S_IXGRP) ? "x" : "-");
//     printf( (mode & S_IROTH) ? "r" : "-");
//     printf( (mode & S_IWOTH) ? "w" : "-");
//     printf( (mode & S_IXOTH) ? "x" : "-");
// }

// void print_details(struct dirent *entry, const char *path) {
//     struct stat file_stat;
//     char fullpath[512];
    
//     snprintf(fullpath, sizeof(fullpath), "%s/%s", path, entry->d_name);
//     if (stat(fullpath, &file_stat) == -1) {
//         perror("stat");
//         return;
//     }

//     print_permissions(file_stat.st_mode);
//     printf(" %ld", (long)file_stat.st_nlink);

//     struct passwd *pw = getpwuid(file_stat.st_uid);
//     struct group *gr = getgrgid(file_stat.st_gid);
//     printf(" %s %s", pw->pw_name, gr->gr_name);
    
//     printf(" %5ld", (long)file_stat.st_size);

//     char timebuf[80];
//     struct tm *timeinfo = localtime(&file_stat.st_mtime);
//     strftime(timebuf, sizeof(timebuf), "%b %d %H:%M", timeinfo);
//     printf(" %s", timebuf);
    
//     printf(" %s\n", entry->d_name);
// }

// void list_files(const char *path, int show_all, int show_long) {
//     DIR *dir = opendir(path);
//     if (dir == NULL) {
//         perror("opendir");
//         return;
//     }
    
//     struct dirent *entry;
//     while ((entry = readdir(dir)) != NULL) {
//         // Skip hidden files unless -a is provided
//         if (!show_all && entry->d_name[0] == '.') continue;
        
//         if (show_long) {
//             print_details(entry, path);
//         } else {
//             printf("%s  ", entry->d_name);
//         }
//     }
    
//     if (!show_long) printf("\n");
    
//     closedir(dir);
// }

// int main(int argc, char *argv[]) {
//     int show_all = 0, show_long = 0;
//     const char *path = ".";
    
//     // Parse command line options
//     for (int i = 1; i < argc; i++) {
//         if (strcmp(argv[i], "-a") == 0) {
//             show_all = 1;
//         } else if (strcmp(argv[i], "-l") == 0) {
//             show_long = 1;
//         } else {
//             path = argv[i]; // Assume it's a directory path
//         }
//     }

//     list_files(path, show_all, show_long);
    
//     return 0;
// }

#include <stdio.h>       // Standard input-output library for printf()
#include <stdlib.h>      // Standard library for general functions like perror()
#include <dirent.h>      // Directory entry library for handling directories
#include <sys/stat.h>    // Library for retrieving file status
#include <time.h>        // Time library for handling file modification times
#include <string.h>      // String library for handling string functions like strcmp()
#include <unistd.h>      // POSIX operating system API for access() function
#include <pwd.h>         // Library for handling user names
#include <grp.h>         // Library for handling group names

// Function to print the file permissions in 'ls -l' style
void print_file_permissions(mode_t mode) {
    // File type and permissions
    printf((S_ISDIR(mode)) ? "d" : "-");  // Directory or regular file
    printf((mode & S_IRUSR) ? "r" : "-"); // Owner has read permission
    printf((mode & S_IWUSR) ? "w" : "-"); // Owner has write permission
    printf((mode & S_IXUSR) ? "x" : "-"); // Owner has execute permission
    printf((mode & S_IRGRP) ? "r" : "-"); // Group has read permission
    printf((mode & S_IWGRP) ? "w" : "-"); // Group has write permission
    printf((mode & S_IXGRP) ? "x" : "-"); // Group has execute permission
    printf((mode & S_IROTH) ? "r" : "-"); // Others have read permission
    printf((mode & S_IWOTH) ? "w" : "-"); // Others have write permission
    printf((mode & S_IXOTH) ? "x" : "-"); // Others have execute permission
}

// Function to print detailed information about each file (like 'ls -l')
void display_file_info(struct dirent *entry, const char *dir_path) {
    struct stat file_stat;
    char complete_path[512];  // Buffer to hold the full path of the file

    // Combine directory path and file name to create full path
    // snprintf(complete_path, sizeof(complete_path), "%s/%s", dir_path, entry->d_name);

    if (strlen(dir_path) + strlen(entry->d_name) + 1 < 512) {
    strcpy(complete_path, dir_path);
    strcat(complete_path, "/");
    strcat(complete_path, entry->d_name);
} else {
    fprintf(stderr, "Error: Path length exceeds buffer size.\n");
    // Handle the error appropriately (e.g., return an error code)
}

    // Get file status information
    if (stat(complete_path, &file_stat) == -1) {
        perror("stat");  // Print error if stat fails
        return;
    }

    // Print file permissions, number of links, owner, group, size, and last modification time
    print_file_permissions(file_stat.st_mode); // Print file type and permissions
    printf(" %ld", (long)file_stat.st_nlink);  // Print number of hard links

    // Get and print owner and group names
    struct passwd *pw = getpwuid(file_stat.st_uid); // Owner info
    struct group *gr = getgrgid(file_stat.st_gid);  // Group info
    printf(" %s %s", pw->pw_name, gr->gr_name);     // Print owner and group names

    printf(" %5ld", (long)file_stat.st_size);       // Print file size in bytes

    // Convert last modification time to a readable format and print it
    char time_buffer[80];
    struct tm *time_info = localtime(&file_stat.st_mtime);
    strftime(time_buffer, sizeof(time_buffer), "%b %d %H:%M", time_info);
    printf(" %s", time_buffer);

    // Print file name
    printf(" %s\n", entry->d_name);
}

// Function to list all files in a directory based on provided options
void list_directory_files(const char *directory_path, int show_hidden, int show_detailed) {
    DIR *dir = opendir(directory_path);  // Open the specified directory
    if (dir == NULL) {
        perror("opendir");  // Print error if unable to open directory
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {  // Read directory entries
        // Skip hidden files unless the -a option is provided
        if (!show_hidden && entry->d_name[0] == '.') continue;

        // Print detailed information if -l option is provided, otherwise just print file names
        if (show_detailed) {
            display_file_info(entry, directory_path);
        } else {
            printf("%s  ", entry->d_name);
        }
    }

    if (!show_detailed) printf("\n");  // Print newline if not using -l option

    closedir(dir);  // Close the directory stream
}

// Main function: entry point of the program
int main(int argc, char *argv[]) {
    int show_hidden = 0, show_detailed = 0;  // Flags for -a and -l options
    const char *directory_path = ".";  // Default to current directory

    // Parse command line arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-a") == 0) {  // Check for -a option
            show_hidden = 1;
        } else if (strcmp(argv[i], "-l") == 0) {  // Check for -l option
            show_detailed = 1;
        } else {
            directory_path = argv[i];  // If not an option, assume it's a directory path
        }
    }

    // List files in the directory based on the options provided
    list_directory_files(directory_path, show_hidden, show_detailed);

    return 0;  // Exit the program
}
