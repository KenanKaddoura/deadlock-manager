#include <stdio.h>
#include <stdbool.h>
#include "banker.h"



// Banker's Algorithm (Safety Check) - is_safe() Implementation
bool is_safe() {
    int work[NUMBER_OF_RESOURCES];
    bool finish[NUMBER_OF_CUSTOMERS] = {false};

    // Initialize Work = Available
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
        work[i] = available[i];
    }

    // Find a process that can finish
    int count = 0;
    while (count < NUMBER_OF_CUSTOMERS) {
        bool found = false;
        for (int p = 0; p < NUMBER_OF_CUSTOMERS; p++) {
            if (finish[p] == false) {
                // Check if Need_p <= Work
                bool can_finish = true;
                for (int j = 0; j < NUMBER_OF_RESOURCES; j++) {
                    if (need[p][j] > work[j]) {
                        can_finish = false;
                        break;
                    }
                }

                // If it can finish, simulate execution and release its resources
                if (can_finish) {
                    for (int j = 0; j < NUMBER_OF_RESOURCES; j++) {
                        work[j] += allocation[p][j];
                    }
                    finish[p] = true;
                    found = true;
                    count++;
                }
            }
        }

        // If we loop through all processes and find none that can finish, system is unsafe
        if (found == false) {
            return false;
        }
    }

    // If all processes finished, system is in a safe state
    return true;
}


// Request Resources Implementation
int request_resources(int customer_num, int request[]) {

    // Lock the mutex to prevent other threads from modifying state concurrently
    pthread_mutex_lock(&resource_lock);

    // Check if Request <= Need
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
        if (request[i] > need[customer_num][i]) {
            printf("[DENIED] Customer %d requested more than needed.\n", customer_num);
            pthread_mutex_unlock(&resource_lock);

            return -1;
        }
    }

    // Check if Request <= Available
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
        if (request[i] > available[i]) {
            pthread_mutex_unlock(&resource_lock);

            return -1; 
        }
    }

    // Pretend to allocate resources
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
        available[i] -= request[i];
        allocation[customer_num][i] += request[i];
        need[customer_num][i] -= request[i];
    }

    // Run the safety algorithm
    if (is_safe()) { // Granted Case
        printf(COLOR_GREEN "[GRANTED] Customer %d request approved." COLOR_RESET "\n", customer_num);
        pthread_mutex_unlock(&resource_lock);

        return 0; 

    } else { // Denied Case
        printf(COLOR_RED "[DENIED] Customer %d request leads to UNSAFE state." COLOR_RESET "\n", customer_num);
        for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
            available[i] += request[i];
            allocation[customer_num][i] -= request[i];
            need[customer_num][i] += request[i];
        }
        pthread_mutex_unlock(&resource_lock);

        return -1; 
    }
}

// Release Resources Implementation
int release_resources(int customer_num, int release[]) {
    pthread_mutex_lock(&resource_lock);

    // Validate the release (cannot release more than currently allocated)
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
        if (release[i] > allocation[customer_num][i]) {
            printf("[ERROR] Customer %d tried to release more than allocated.\n", customer_num);
            pthread_mutex_unlock(&resource_lock);

            return -1;
        }
    }

    // Process the release
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
        allocation[customer_num][i] -= release[i];
        available[i] += release[i];
        // Note: 'need' generally does not increase upon release in Banker's algorithm, 
        // it strictly tracks the remaining maximum claim.
    }

    printf(COLOR_YELLOW "[RELEASED] Customer %d released resources." COLOR_RESET "\n", customer_num);
    
    pthread_mutex_unlock(&resource_lock);

    return 0;
}