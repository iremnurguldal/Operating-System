#include <stdio.h>
#include "scheduler.h"

// Declare external references to the process queues
extern ProcessQueue high_priority_queue;
extern ProcessQueue medium_priority_queue;
extern ProcessQueue low_priority_queue;

int main() {
    // Array to store processes and variable to hold the number of processes
    Process processes[MAX_PROCESSES];
    int num_processes = 0;

    // Read input processes from file
    read_input(processes, &num_processes);
    
    // Perform scheduling of processes
    perform_scheduling(processes, num_processes);

    return 0;
}
