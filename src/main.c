#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "banker.h"
#include "simulator.h"

// Memory Allocation for Globals
int available[NUMBER_OF_RESOURCES];
int maximum[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
int allocation[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
int need[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
pthread_mutex_t resource_lock;

int main(int argc, char *argv[]) {
    if (argc < NUMBER_OF_RESOURCES + 1) {
        printf("Usage: ./deadlock_manager [recovery] <res1> <res2> <res3>\n");
        return -1;
    }

    // Parse Mode
    int offset = 1;
    if (argc > NUMBER_OF_RESOURCES + 1 && strcmp(argv[1], "recovery") == 0) {
        operating_mode = 1; // Enable Phase 3 Recovery Mode
        offset = 2;
    }

    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) available[i] = atoi(argv[i + offset]);

    int temp_max[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES] = {
        {7, 5, 3}, {3, 2, 2}, {9, 0, 2}, {2, 2, 2}, {4, 3, 3}
    };

    for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
        for (int j = 0; j < NUMBER_OF_RESOURCES; j++) {
            maximum[i][j] = temp_max[i][j];
            allocation[i][j] = 0;       
            need[i][j] = maximum[i][j]; 
        }
    }

    pthread_mutex_init(&resource_lock, NULL);

    printf("\nSystem Initialized in %s MODE.\n", operating_mode == 0 ? "BANKER" : "RECOVERY");
    printf("Initial Available Resources: %d %d %d\n\n", available[0], available[1], available[2]);

    // Spawn Phase 3 Monitor Thread
    pthread_t monitor;
    pthread_create(&monitor, NULL, deadlock_monitor_thread, NULL);

    pthread_t threads[NUMBER_OF_CUSTOMERS];
    int customer_ids[NUMBER_OF_CUSTOMERS]; 

    for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
        customer_ids[i] = i; 
        pthread_create(&threads[i], NULL, client_thread, &customer_ids[i]);
    }

    // Wait for clients to finish
    for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) pthread_join(threads[i], NULL);

    printf("\n--- Simulation Complete ---\n");
    pthread_mutex_destroy(&resource_lock);
    return 0;
}