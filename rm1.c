#include <stdio.h>      // Standard Input/Output library for functions like printf()
#include <unistd.h>     // POSIX API for system calls, including unlink() used for file removal
#include <string.h>     // Library for string manipulation functions such as strcmp()
#include <stdlib.h>     // Standard library functions including exit()

// Function prototypes for rdir and rm commands
int rdir(int argc, char *argv[]);
int rm(int argc, char *argv[]);

// Buffer defined for potential error messages
char error_buffer[512];

// Main function: Entry point of the program
int main(int argc, char *argv[]) {
    // Call the rm function to handle the removal of files passed via command-line arguments
    return rm(argc, argv);
}

// Function to handle file removal based on the command-line arguments provided
int rm(int argc, char *argv[]) {
    // Variables for loop and user input
    int i;
    char confirm;

    // Check if the command was used with the correct number of arguments
    if (argc < 2) {
        printf("Error: No file specified for removal.\n");
        printf("Usage:\n\trm <filename1> <filename2> ...\n\n");  // Display usage instructions
        return 0;  // Exit if no files are provided
    }

    // Iterate over the list of arguments, starting from 1 (since 0 is the program name)
    for (i = 1; i < argc; i++) {
        // Skip any empty string arguments
        if (strlen(argv[i]) > 0) {
            // Ask the user for confirmation before deleting the file
            printf("Are you sure you want to delete the file '%s'? (Enter y/Y to confirm): ", argv[i]);
            confirm = getchar();  // Capture the user's response

            // Check if the user confirmed deletion by entering 'y' or 'Y'
            if (confirm == 'y' || confirm == 'Y') {
                // Attempt to delete the specified file
                if (unlink(argv[i]) == -1) {
                    // If unlink() fails, print an error message and exit the program
                    perror("Error: Failed to delete the file.");
                    exit(EXIT_FAILURE);
                }
            }

            // Clear the input buffer to handle any leftover input
            while (getchar() != '\n'); 
        }
    }

    // Indicate successful completion
    return 0;
}
