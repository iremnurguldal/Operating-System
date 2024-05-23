#include <stdio.h>
#include "scheduler.h"

void read_input(Process *processes, int *num_processes) {
    FILE *input_file;
    input_file = fopen("input.txt", "r");
    if (input_file == NULL) {
        printf("Girdi dosyası bulunamadı!");
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


void perform_scheduling(Process *processes, int num_processes) {
    FILE *output_file = fopen("output.txt", "w");
    if (output_file == NULL) {
        printf("Output dosyası oluşturulamadı!");
        return;
    }



    // Öncelik sırasına göre işlemleri sırala
    sort_processes_by_priority(processes, num_processes);

    // CPU'lar için RAM ve CPU kullanımını kontrol et
    int cpu1_ram = TOTAL_RAM * 0.25; // CPU-1 için ayrılan RAM
    int cpu2_ram = TOTAL_RAM * 0.75; // CPU-2 için ayrılan RAM

    // FCFS için CPU-1'e iş atama
    for (int i = 0; i < num_processes; i++) {
        if (processes[i].priority == 0) {
            // CPU-1'e iş atama
            if (are_resources_available(processes[i].RAM, processes[i].CPU_rate, &cpu1_ram, NULL)) {
                fprintf(output_file, "Process %s is queued to be assigned to CPU-1.\n", processes[i].process_number);
                fprintf(output_file, "Process %s is assigned to CPU-1.\n", processes[i].process_number);
                // İşlemleri CPU-1'e atama işlemi
                // Örneğin: assign_to_cpu1(&processes[i]);
                fprintf(output_file, "Process %s is completed and terminated.\n", processes[i].process_number);
            }
        }
    }

    // Diğer işlemleri CPU-2'ye atama
    for (int i = 0; i < num_processes; i++) {
        if (processes[i].priority != 0) {
            // CPU-2'ye iş atama
            if (are_resources_available(processes[i].RAM, processes[i].CPU_rate, &cpu2_ram, NULL)) {
                fprintf(output_file, "Process %s is placed in the queue to be assigned to CPU-2.\n", processes[i].process_number);
                // İşlem öncelik seviyesine göre uygun kuyruğa ekleme
                if (processes[i].priority == 1) {
                    // Yüksek öncelikli işlemler için SJF algoritması
                    enqueue_process(&high_priority_queue, processes[i]);
                } else if (processes[i].priority == 2) {
                    // Orta öncelikli işlemler için Round Robin algoritması (quantum time: 8)
                    enqueue_process(&medium_priority_queue, processes[i]);
                } else if (processes[i].priority == 3) {
                    // Düşük öncelikli işlemler için Round Robin algoritması (quantum time: 16)
                    enqueue_process(&low_priority_queue, processes[i]);
                }
                fprintf(output_file, "Process %s is assigned to CPU-2.\n", processes[i].process_number);
                // İşlemleri CPU-2'ye atama işlemi
                // Örneğin: assign_to_cpu2(&processes[i]);
                fprintf(output_file, "The operation of process %s is completed and terminated.\n", processes[i].process_number);
            }
        }
    }

   
    // Kuyrukların içeriğini yazdırmak için
    fprintf(output_file, "CPU-1 que1(priority-0) (FCFS)→ ");
    print_queue_contents(&high_priority_queue, output_file);

    fprintf(output_file, "CPU-2 que2(priority-1) (SJF)→ ");
    print_queue_contents(&medium_priority_queue, output_file);

    fprintf(output_file, "CPU-2 que3(priority-2) (RR-q8)→ ");
    print_queue_contents(&low_priority_queue, output_file);


    fclose(output_file);
}
// Kuyruğun içeriğini dosyaya yazdırmak için fonksiyon tanımı
void print_queue_contents(ProcessQueue *queue, FILE *output_file) {
    // Kuyruğun içeriğini dolaşarak her bir elemanı dosyaya yazdırma
    for (int i = queue->front; i <= queue->rear; i++) {
        fprintf(output_file, "%s ", queue->processes[i].process_number);
    }
    fprintf(output_file, "\n"); // Kuyruğun sonuna yeni satır ekleme
}
