#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFFER_SIZE 50

// Function to read from a file and write to a specified file descriptor
void cat_file(const char *filename, int output_fd)
{
    int file;
    char buffer[BUFFER_SIZE];
    int read_size;

    file = open(filename, O_RDONLY);
    if (file == -1)  
    {
        fprintf(stderr, "Error: %s: file not found\n", filename);
        return;
    }

    // Read from the file and write to the specified output_fd until end of file
    while ((read_size = read(file, buffer, BUFFER_SIZE)) > 0)
    {
        write(output_fd, buffer, read_size);
    }
    close(file);
}

void cat_files_to_output(int argc, char **argv, const char *output_filename)
{
    int output_fd = open(output_filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (output_fd == -1)
    {
        fprintf(stderr, "Error: Could not open or create %s\n", output_filename);
        return;
    }

    for (int i = 1; i < argc - 1; i++)  // Don't include the last argument (output filename)
    {
        cat_file(argv[i], output_fd);
    }

    close(output_fd);
}

int main(int argc, char **argv)
{
    if (argc < 2)  
    {
        fprintf(stderr, "Error: usage: ./cat filename [filename2 ...] > outputfile\n");
        return -1;
    }

    // Check if redirection is needed (e.g., `> outputfile`)
    if (argv[argc - 2][0] == '>')
    {
        const char *output_filename = argv[argc - 1];
        cat_files_to_output(argc, argv, output_filename);
    }
    else
    {
        for (int i = 1; i < argc; i++)
        {
            cat_file(argv[i], STDOUT_FILENO);  // Write to stdout if no redirection
        }
    }

    return 0;
}
