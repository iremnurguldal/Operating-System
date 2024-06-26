#ifndef CPU_SCHEDULER_H
#define CPU_SCHEDULER_H

#define MAX_PROCESSES 100
#define TOTAL_RAM 2048

// Structure to represent a process
typedef struct {
    char process_number[10];
    int arrival_time;
    int priority;
    int burst_time;
    int RAM;
    int CPU_rate;
} Process;

// Structure to represent a queue of processes
typedef struct {
    Process processes[MAX_PROCESSES];
    int front, rear;
    int size;
} ProcessQueue;

// Function prototypes
void sort_processes_by_priority(Process *processes, int num_processes);
int are_resources_available(int RAM, int CPU_rate, int *cpu_ram, int *cpu_rate);
void enqueue_process(ProcessQueue *queue, Process process);
void print_queue_contents(ProcessQueue *queue, FILE *output_file);

// Extern declarations for the process queues
extern ProcessQueue high_priority_queue;
extern ProcessQueue medium_priority_queue;
extern ProcessQueue low_priority_queue;

// Function prototypes for reading input and performing scheduling
void read_input(Process *processes, int *num_processes);
void perform_scheduling(Process *processes, int num_processes);

#endif
