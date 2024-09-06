#include <stdio.h>      // For standard input/output functions
#include <stdlib.h>     // For exit() and EXIT_FAILURE
#include <sys/stat.h>   // For mkdir() function and mode constants
#include <errno.h>      // For error handling using errno
#include <string.h>     // For strerror() function

// Function prototype
int make_directory(int argc, char *argv[]);

int main(int argc, char *argv[]) {
    // Call the function to create directories with command-line arguments
    return make_directory(argc, argv);
}

// Function to create directories based on provided command-line arguments
int make_directory(int argc, char *argv[]) {
    // Check if at least one directory name is provided
    if (argc < 2) {
        printf("Error: No directory name provided.\n");
        printf("Usage:\n\tmkdir <directory1> <directory2> ...\n");
        return EXIT_FAILURE;
    }

    // Iterate over each argument (directory name) starting from index 1
    for (int i = 1; i < argc; i++) {
        // Attempt to create the directory with default permissions (0755)
        if (mkdir(argv[i], 0755) == -1) {
            // Print error message if mkdir fails
            fprintf(stderr, "Error: Unable to create directory '%s': %s\n", argv[i], strerror(errno));
            // Continue to the next argument instead of exiting, allowing multiple errors to be shown
        } else {
            // Success message for each created directory
            printf("Directory '%s' created successfully.\n", argv[i]);
        }
    }

    return EXIT_SUCCESS;
}
