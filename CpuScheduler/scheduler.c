#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scheduler.h"

// Definition of queues for different priorities
ProcessQueue high_priority_queue = { .front = 0, .rear = -1, .size = 0 };
ProcessQueue medium_priority_queue = { .front = 0, .rear = -1, .size = 0 };
ProcessQueue low_priority_queue = { .front = 0, .rear = -1, .size = 0 };

// Sort processes by priority
void sort_processes_by_priority(Process *processes, int num_processes) {
    for (int i = 0; i < num_processes - 1; i++) {
        for (int j = 0; j < num_processes - i - 1; j++) {
            if (processes[j].priority > processes[j + 1].priority) {
                Process temp = processes[j];
                processes[j] = processes[j + 1];
                processes[j + 1] = temp;
            }
        }
    }
}

// Check if resources are available
int are_resources_available(int RAM, int CPU_rate, int *cpu_ram, int *cpu_rate) {
    if (cpu_ram && *cpu_ram < RAM) {
        return 0;
    }
    if (cpu_rate && *cpu_rate < CPU_rate) {
        return 0;
    }
    if (cpu_ram) {
        *cpu_ram -= RAM;
    }
    if (cpu_rate) {
        *cpu_rate -= CPU_rate;
    }
    return 1;
}

// Enqueue process into queue
void enqueue_process(ProcessQueue *queue, Process process) {
    if (queue->size == MAX_PROCESSES) {
        printf("Queue is full, process cannot be added!\n");
        return;
    }
    queue->rear = (queue->rear + 1) % MAX_PROCESSES;
    queue->processes[queue->rear] = process;
    queue->size++;
}

// Function definition to print the contents of a queue to a file
void print_queue_contents(ProcessQueue *queue, FILE *output_file) {
    // Iterate over the content of the queue and print each element to the file
    for (int i = 0; i < queue->size; i++) {
        int index = (queue->front + i) % MAX_PROCESSES;
        fprintf(output_file, "%s ", queue->processes[index].process_number);
    }
    fprintf(output_file, "\n"); // Add a new line at the end of the queue
}

// Read input from file
void read_input(Process *processes, int *num_processes) {
    FILE *input_file;
    input_file = fopen("input.txt", "r");
    if (input_file == NULL) {
        printf("Input file not found!");
        return;
    }

    while (fscanf(input_file, "%[^,],%d,%d,%d,%d,%d\n", 
        processes[*num_processes].process_number, 
        &processes[*num_processes].arrival_time, 
        &processes[*num_processes].priority, 
        &processes[*num_processes].burst_time, 
        &processes[*num_processes].RAM, 
        &processes[*num_processes].CPU_rate) == 6) {
        (*num_processes)++;
    }

    fclose(input_file);
}

// Perform scheduling of processes
void perform_scheduling(Process *processes, int num_processes) {
    FILE *output_file = fopen("output.txt", "w");
    if (output_file == NULL) {
        printf("Output file could not be created!");
        return;
    }

    // Sort processes by priority
    sort_processes_by_priority(processes, num_processes);

    // Check RAM and CPU usage for CPUs
    int cpu1_ram = TOTAL_RAM * 0.25; // RAM allocated for CPU-1
    int cpu2_ram = TOTAL_RAM * 0.75; // RAM allocated for CPU-2

    // Assign jobs to CPU-1 for FCFS
    for (int i = 0; i < num_processes; i++) {
        if (processes[i].priority == 0) {
            // Assign job to CPU-1
            if (are_resources_available(processes[i].RAM, processes[i].CPU_rate, &cpu1_ram, NULL)) {
                fprintf(output_file, "Process %s is queued to be assigned to CPU-1.\n", processes[i].process_number);
                fprintf(output_file, "Process %s is assigned to CPU-1.\n", processes[i].process_number);
                // Perform job assignment to CPU-1
                fprintf(output_file, "Process %s is completed and terminated.\n", processes[i].process_number);
            }
        }
    }

    // Assign other jobs to CPU-2
    for (int i = 0; i < num_processes; i++) {
        if (processes[i].priority != 0) {
            // Assign job to CPU-2
            if (are_resources_available(processes[i].RAM, processes[i].CPU_rate, &cpu2_ram, NULL)) {
                fprintf(output_file, "Process %s is placed in the queue to be assigned to CPU-2.\n", processes[i].process_number);
                // Enqueue job to the appropriate queue based on priority
                if (processes[i].priority == 1) {
                    // SJF algorithm for high priority jobs
                    enqueue_process(&high_priority_queue, processes[i]);
                } else if (processes[i].priority == 2) {
                    // Round Robin algorithm for medium priority jobs (quantum time: 8)
                    enqueue_process(&medium_priority_queue, processes[i]);
                } else if (processes[i].priority == 3) {
                    // Round Robin algorithm for low priority jobs (quantum time: 16)
                    enqueue_process(&low_priority_queue, processes[i]);
                }
                fprintf(output_file, "Process %s is assigned to CPU-2.\n", processes[i].process_number);
                // Perform job assignment to CPU-2
                fprintf(output_file, "The operation of process %s is completed and terminated.\n", processes[i].process_number);
            }
        }
    }
    // Print the contents of queues
    fprintf(output_file, "CPU-1 queue (priority-0) (FCFS)→ ");
    print_queue_contents(&high_priority_queue, output_file);

    fprintf(output_file, "CPU-2 queue (priority-1) (SJF)→ ");
    print_queue_contents(&medium_priority_queue, output_file);

    fprintf(output_file, "CPU-2 queue (priority-2) (RR-q8)→ ");
    print_queue_contents(&medium_priority_queue, output_file);

    fprintf(output_file, "CPU-2 queue (priority-3) (RR-q16)→ ");
    print_queue_contents(&low_priority_queue, output_file);

    fclose(output_file);
}
