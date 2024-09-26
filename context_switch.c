#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <setjmp.h>

// Jump buffers to save the process states
jmp_buf env1, env2;
int process = 1; // Variable to keep track of which process is running

// Function to handle the SIGALRM signal
void context_switch(int signum) {
    // Switch between processes
    if (process == 1) {
        process = 2;
        printf("Switching to Process 2...\n");
        alarm(1);  // Set an alarm for 1 second
        longjmp(env2, 1);  // Switch to Process 2
    } else {
        process = 1;
        printf("Switching to Process 1...\n");
        alarm(1);  // Set an alarm for 1 second
        longjmp(env1, 1);  // Switch to Process 1
    }
}

// Simulate Process 1
void process1() {
    while (1) {
        printf("Process 1 is running...\n");
        sleep(2);  // Simulate some work
    }
}

// Simulate Process 2
void process2() {
    while (1) {
        printf("Process 2 is running...\n");
        sleep(2);  // Simulate some work
    }
}

int main() {
    // Set the signal handler for SIGALRM
    signal(SIGALRM, context_switch);

    // Set the alarm to go off in 1 second
    alarm(1);

    // Save the context of Process 1
    if (setjmp(env1) == 0) {
        // Save the context of Process 2
        if (setjmp(env2) == 0) {
            // Start running Process 1
            process1();
        } else {
            // Start running Process 2
            process2();
        }
    } else {
        // The program will resume here after switching
        process1();
    }

    return 0;
}
