#include <stdio.h>
#include <stdlib.h>    // For atoi() function
#include <unistd.h>    // For read() and write() functions
#include <fcntl.h>     // For open() and close() functions
#include <string.h>    // For string handling functions

// Function prototype for the head function
int head(int argc, char *argv[]);

// Buffer to hold file contents (not used in this code but can be used for larger reads)
char buffer[4096];

// Main function: Entry point of the program
int main(int argc, char *argv[]) {
    // Calls the head function with command-line arguments
    head(argc, argv);
    return 0;
}

// Function to display the first few lines of each specified file
int head(int argc, char *argv[]) {
    int fd;                      // File descriptor for file operations
    int i;                       // Index variable for looping through command-line arguments
    int count = 0, num = 10;     // Default number of lines to display is 10

    // Check if there are enough command-line arguments
    if (argc == 1) {
        printf("Wrong number of arguments\nUsage:\n\t%s [<-nlines>] <file(s)>\n", argv[0]);
        return 0;
    }

    // Check if the first argument is an option specifying the number of lines
    if (argv[1][0] == '-') {        // If the first argument starts with '-'
        num = atoi(argv[1] + 1);    // Convert the number after '-' to an integer using atoi
        i = 2;                      // File arguments start after the line number argument
    } else {
        i = 1;                      // If no line number is specified, file arguments start at the first argument
    }

    // Loop through each file provided in the command-line arguments
    for (; i < argc; i++) {
        // Attempt to open the file in read-only mode
        fd = open(argv[i], O_RDONLY);
        if (fd == -1) {             // Check if the file opened successfully
            perror("Error opening file");  // Print error message if file can't be opened
            continue;               // Skip to the next file if there's an error
        }

        int temp_count = 0;         // Counter to keep track of the number of lines read
        char ch;                    // Variable to hold each character read from the file

        // Print the file name header if multiple files are specified
        printf("==> %s <==\n", argv[i]);  

        // Read the file one character at a time until the desired number of lines is reached
        while (read(fd, &ch, 1) == 1) {  // Read one character from the file
            write(STDOUT_FILENO, &ch, 1); // Write the character to the standard output (screen)
            if (ch == '\n') {             // Check if the character is a newline
                temp_count++;             // Increment the line counter
                if (temp_count == num) {  // Stop if the specified number of lines is reached
                    break;
                }
            }
        }

        // Close the file after processing
        close(fd);
    }

    return 0;  // Return 0 to indicate successful execution
}
