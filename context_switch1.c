#include <stdio.h>      // Standard input/output library for printf
#include <unistd.h>     // UNIX standard functions for sleep
#include <signal.h>     // Signal handling functions for SIGALRM
#include <setjmp.h>     // Functions for non-local jumps (setjmp and longjmp)

// Jump buffers to save the process states
jmp_buf env1, env2;  // These buffers will hold the states for processes 1 and 2
int process = 1;     // Variable to keep track of which process is currently running

// Function to handle the SIGALRM signal
void context_switch(int signum) {
    // Switch between processes based on the current process
    if (process == 1) {
        process = 2;  // Switch to Process 2
        printf("Switching to Process 2...\n");
        alarm(1);  // Set an alarm for 1 second
        longjmp(env2, 1);  // Switch to the state saved for Process 2
    } else {
        process = 1;  // Switch to Process 1
        printf("Switching to Process 1...\n");
        alarm(1);  // Set an alarm for 1 second
        longjmp(env1, 1);  // Switch to the state saved for Process 1
    }
}

// Simulate Process 1
void process1() {
    while (1) {  // Infinite loop to keep Process 1 running
        printf("Process 1 is running...\n");
        sleep(2);  // Simulate work being done for 2 seconds
    }
}

// Simulate Process 2
void process2() {
    while (1) {  // Infinite loop to keep Process 2 running
        printf("Process 2 is running...\n");
        sleep(2);  // Simulate work being done for 2 seconds
    }
}

int main() {
    // Set the signal handler for SIGALRM to call context_switch when the alarm goes off
    signal(SIGALRM, context_switch);

    // Set the initial alarm to go off in 1 second
    alarm(1);

    // Save the context for Process 1
    if (setjmp(env1) == 0) {  // If this is the first time setjmp is called
        // Save the context for Process 2
        if (setjmp(env2) == 0) {  // If this is the first time for Process 2
            // Start running Process 1
            process1();
        } else {
            // Start running Process 2
            process2();  // This will execute if switching back to Process 2
        }
    } else {
        // The program will resume here after switching back to Process 1
        process1();  // This will execute if switching back to Process 1
    }

    return 0;  // Return 0 on successful termination of the program (although this won't be reached)
}
// Summary of Key Concepts:
// Jump Buffers: jmp_buf env1, env2; are used to save the execution state of each simulated process. These buffers hold information such as the stack pointer and program counter.

// Process Switching: The context_switch function is registered as a signal handler for SIGALRM. When the alarm goes off, it switches between Process 1 and Process 2 using longjmp. This allows the program to simulate multitasking by saving and restoring the execution context.

// Simulated Processes: The functions process1 and process2 represent the two simulated processes that run indefinitely. They print their status and simulate doing work with sleep.

// Main Function Flow:

// The main function sets up the signal handler and initializes the alarm.
// It uses setjmp to save the context of Process 1 and Process 2. When setjmp is first called, it returns 0. On subsequent calls from longjmp, it returns a non-zero value.
// The program starts by executing Process 1. Upon receiving the alarm signal after 1 second, it will switch to Process 2, and the cycle continues indefinitely.
// Important Notes:
// Infinite Loops: Both process1 and process2 have infinite loops that would run continuously, simulating active processes. In a real multitasking environment, this behavior would be managed by the operating system.

// Alarm Mechanism: The alarm is set to trigger every second, allowing for context switching at regular intervals, effectively simulating a time-slicing scheduling mechanism.

// No Exit Condition: Since both processes run indefinitely, the program does not have a natural exit point unless interrupted externally (e.g., by pressing Ctrl+C).

// This implementation provides a basic demonstration of cooperative multitasking using C, highlighting how process states can be managed using setjmp and longjmp along with signal handling.
