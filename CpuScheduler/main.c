#include <stdio.h>
#include "scheduler.h"

extern ProcessQueue high_priority_queue;
extern ProcessQueue medium_priority_queue;
extern ProcessQueue low_priority_queue;

int main() {
    Process processes[MAX_PROCESSES];
    int num_processes = 0;

    read_input(processes, &num_processes);
    perform_scheduling(processes, num_processes);

    return 0;
}
