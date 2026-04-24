#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "banker.h"
#include "simulator.h"

// Global Variables Memory Allocation
// (This actually reserves the RAM for the externs we declared in banker.h)
int available[NUMBER_OF_RESOURCES];
int maximum[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
int allocation[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
int need[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
pthread_mutex_t resource_lock;

int main(int argc, char *argv[]) {
    // Validate Command Line Arguments
    // We expect exactly 3 resource counts based on NUMBER_OF_RESOURCES
    if (argc != NUMBER_OF_RESOURCES + 1) {
        printf("Error: Please provide exactly %d resource limits.\n", NUMBER_OF_RESOURCES);
        printf("Usage: ./deadlock_manager <res1> <res2> <res3>\n");
        return -1;
    }

    // Initialize Available Resources from CLI
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
        available[i] = atoi(argv[i + 1]);
    }

    // Initialize Matrices (Hardcoded for simulation testing)
    // !!! We can modify these values to test different deadlock/safe scenarios 
    int temp_max[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES] = {
        {7, 5, 3},
        {3, 2, 2},
        {9, 0, 2},
        {2, 2, 2},
        {4, 3, 3}
    };

    for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
        for (int j = 0; j < NUMBER_OF_RESOURCES; j++) {
            maximum[i][j] = temp_max[i][j];
            allocation[i][j] = 0;       // Starts with 0 resources allocated
            need[i][j] = maximum[i][j]; // Initial need is the maximum they might want
        }
    }

    // Initialize the Mutex Lock
    if (pthread_mutex_init(&resource_lock, NULL) != 0) {
        printf("Error: Mutex initialization failed.\n");
        return -1;
    }

    printf("System Initialized.\n");
    printf("Initial Available Resources: %d %d %d\n\n", available[0], available[1], available[2]);

    // Spawn Client Threads
    pthread_t threads[NUMBER_OF_CUSTOMERS];
    int customer_ids[NUMBER_OF_CUSTOMERS]; // Array to hold IDs to pass safely to threads

    for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
        customer_ids[i] = i; // Assign ID
        if (pthread_create(&threads[i], NULL, client_thread, &customer_ids[i]) != 0) {
            printf("Error: Failed to create thread for Customer %d\n", i);
            return -1;
        }
        printf("Customer %d thread started.\n", i);
    }

    printf("\n--- Starting Simulation ---\n\n");

    // Wait for all threads to finish executing
    for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("\n--- Simulation Complete ---\n");

    // Cleanup
    pthread_mutex_destroy(&resource_lock);

    return 0;
}