#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/wait.h>

#define MAXCOMMANDLEN	4096
#define MAXARGLEN	128
#define	MAXARGCOUNT	256

char *com;
char args[MAXARGCOUNT][MAXARGLEN];
char *argv[MAXARGCOUNT];
char path[MAXCOMMANDLEN], spath[MAXCOMMANDLEN];
char fullpath[MAXCOMMANDLEN];
char* dirs[MAXARGCOUNT];
// The command line is in com[]. Number of characeters in the
// command line is len. It is parsed into separate arguments which
// are copied into args[][] array. The count of arguments 
// including the command, is returned.
int parse_args(char com[], int len) {
	int i, k, argcount = 0;
	while(com[i]==' ' && i<=len)
	i++;
	k = 0;
	while (i <= len) {
		if (com[i]==' ' || com[i] == '\t' || com[i] == 0) {
			// End of the current argument
			argcount++;
			// skip additional spaces
			while(com[i]==' ' && i<=len)
			i++;
		}
		else args[argcount][k++]= com[i++];
	}
	return argcount;
}

int main() {
	unsigned long int len = MAXCOMMANDLEN, alen; 
				// Command line maximum length and actual length
	int countargs;		// Count of arguments
	int pid, status;	// Child process id and its termination status
	char *dirname;		// Pointer to directory name component
				// of path environment variable.
	int aflag, rflag;	// Absolute path or relative path
	int i, j;
	// Get the path envronment variable
	    for(i=0; i<MAXARGCOUNT; i++)
		dirs[i] = NULL;
		path = (char *) getenv("PATH");
		fullpath=path;
		i=0
		for(;; path++) 
    {
	if((*path == ':') || (*path == '\0')) 
        {
            dirs[i] = fullpath;
            i++;
            if(*path == '\0') break;
            *path = '\0';
            fullpath = path + 1;
        }
    }
	// Allocate memory for command line
	for(i =0 ; i<MAXARGCOUNT;i++)
	{
		argv[i] = (char*)malloc(MAXARGLEN);
	}
	// Accept commands in an infinite loop
	while (1) {
		// Display the prompt and read the command. 
		printf("SHELL $ ");
		alen = getline(&com, &len, stdin);
		// Replace the last newline with null character
		com[alen-1]='\0';
		// If some command entered ...
		if (alen != 0) {
			// Parse the arguments
			countargs = parse_args(com, alen);	
			// If the line is empty read next command
			if(countargs == 0)
			break;
			// If exit command is entered, terminate the program
			if(strcmp(args[0],"exit"))
				exit(0);
			// Point to those args, 
			// skipping first one (base command)
			// and point to last NULL argument
			for(i=0;i<countargs;i++)
			argv[i]=args[i];
			argv[countargs]='\0';
			// Determine whether the command is an absolute or 
			// relative path name
			aflag = 0;
			rflag = 0;
			if(args[1][0]=='/')
			aflag=1;
			else
			for(i=0;i<strlen(args[1];i++))
			{
				if(args[1][i]=='/')
				rflag=1;
			}
			// Copy the saved path environment variable
			fullpath = path;
			// Not an absolute or relative path,
			// then search for the command in the
			// directories of the path 
			if ((aflag == 0) && (rflag == 0)) {
				// Search for the command in the path
				for(i = 0; i < MAX_PATHS; i++) 
					{
						if(dir[i] == NULL) break;
						strcpy(path, dir[i]);
						strcat(path, "/");
						// Append base command name to directory name
						strcat(path, argv[0]);
						// Check whether it is executable file
						if(access(pName, X_OK | F_OK) != -1) 
						{
							// File found
							fullpath = (char *) malloc(strlen(pName)+1);
							strcpy(result, pName);
						}
						// Otherwise check in the next directory
					}
				}	
			}
			else {
				// Otherwise path of the command is given
				// either as absolute path or relative path
				       fullpath = (char *) malloc(strlen(argv[0])+1);
        				strcpy(fullpath, argv[0]);
			}
			// Check if base command exists	
			if ((argv[0] == NULL) || (access(argv[0], X_OK) != 0)) 
				printf("Error : No such command : '%s'\n", args[0]);
			else {
				// Create a child process and execute the 
				// specified command.
				if ((pid = fork()) == 0) {
					// Child will execute the command
					execvp(args[0],args);
				}
				else {
					// parent will wait for the completion 
					// of the command/child
					printf("Parent waiting");
				 	status = wait(&pid);
				}
			}
		}
	}
	return 0;
}
