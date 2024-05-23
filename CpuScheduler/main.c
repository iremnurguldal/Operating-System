#include <stdio.h>
#include "scheduler.h"


int main() {
    Process processes[MAX_PROCESSES];
    int num_processes = 0;

    read_input(processes, &num_processes);
    perform_scheduling(processes, num_processes);

    return 0;
}
