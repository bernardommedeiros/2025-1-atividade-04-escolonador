#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/wait.h>
#include <math.h>

// Thread CPU-bound (cálculo de π usando série de Leibniz)
void* cpu_thread(void* arg) {
    long thread_id = (long)arg;
    printf("Thread CPU-bound %ld iniciada (PID: %d)\n", thread_id, getpid());
    
    double pi = 0.0;
    long iterations = 100000000L; // 100 milhões de iterações
    int sign = 1;
    
    for(long i = 0; i < iterations; i++) {
        pi += sign * (4.0 / (2.0 * i + 1.0));
        sign *= -1;
    }
    
    printf("Thread CPU-bound %ld terminou (resultado: %.8f)\n", thread_id, pi);
    return NULL;
}

// Thread I/O-bound (simula operações de E/S)
void* io_thread(void* arg) {
    long thread_id = (long)arg;
    printf("Thread I/O-bound %ld iniciada (PID: %d)\n", thread_id, getpid());
    
    sleep(2);  // Simula operação I/O (2 segundos)
    
    printf("Thread I/O-bound %ld terminou\n", thread_id);
    return NULL;
}

int main() {
    pid_t pid;
    pthread_t thread_cpu[3], thread_io[3];

    printf("Processo pai (PID: %d)\n", getpid());
    pid = fork();

    if (pid == 0) { // Processo filho
        printf("\nProcesso filho (PID: %d)\n", getpid());
        
        // Cria 3 threads CPU-bound
        for(long i = 0; i < 3; i++) {
            pthread_create(&thread_cpu[i], NULL, cpu_thread, (void*)(i+1));
        }
        
        // Cria 3 threads I/O-bound
        for(long i = 0; i < 3; i++) {
            pthread_create(&thread_io[i], NULL, io_thread, (void*)(i+1));
        }
        
        // Espera threads I/O primeiro
        for(int i = 0; i < 3; i++) {
            pthread_join(thread_io[i], NULL);
        }
        
        printf("\n"); // Linha em branco como no exemplo
        
        // Depois espera threads CPU
        for(int i = 0; i < 3; i++) {
            pthread_join(thread_cpu[i], NULL);
        }
        
    } else if (pid > 0) { // Processo pai
        wait(NULL); // Espera filho terminar
        printf("\nProcesso pai terminou\n");
    }

    return 0;
}