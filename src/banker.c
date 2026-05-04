#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include "banker.h"

// Define the new globals for Phase 3
int operating_mode = 0; 
int active_request[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES] = {0};

bool is_safe() {
    int work[NUMBER_OF_RESOURCES];
    bool finish[NUMBER_OF_CUSTOMERS] = {false};

    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) work[i] = available[i];

    int count = 0;
    while (count < NUMBER_OF_CUSTOMERS) {
        bool found = false;
        for (int p = 0; p < NUMBER_OF_CUSTOMERS; p++) {
            if (finish[p] == false) {
                bool can_finish = true;
                for (int j = 0; j < NUMBER_OF_RESOURCES; j++) {
                    if (need[p][j] > work[j]) {
                        can_finish = false;
                        break;
                    }
                }
                if (can_finish) {
                    for (int j = 0; j < NUMBER_OF_RESOURCES; j++) work[j] += allocation[p][j];
                    finish[p] = true;
                    found = true;
                    count++;
                }
            }
        }
        if (found == false) return false;
    }
    return true;
}

int request_resources(int customer_num, int request[]) {
    pthread_mutex_lock(&resource_lock);

    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
        if (request[i] > need[customer_num][i]) {
            printf(COLOR_RED "[DENIED] Customer %d requested more than needed." COLOR_RESET "\n", customer_num);
            pthread_mutex_unlock(&resource_lock);
            return -1;
        }
    }

    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
        if (request[i] > available[i]) {
            // Must wait! Record this active request for the Deadlock Monitor
            for(int j=0; j<NUMBER_OF_RESOURCES; j++) active_request[customer_num][j] = request[j];
            pthread_mutex_unlock(&resource_lock);
            return -1; 
        }
    }

    // Pretend to allocate
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
        available[i] -= request[i];
        allocation[customer_num][i] += request[i];
        need[customer_num][i] -= request[i];
    }

    // MODE CHECK (Phase 3 Logic)
    if (operating_mode == 0) { 
        // BANKER MODE
        if (is_safe()) {
            for(int j=0; j<NUMBER_OF_RESOURCES; j++) active_request[customer_num][j] = 0;
            printf(COLOR_GREEN "[GRANTED] Customer %d request approved." COLOR_RESET "\n", customer_num);
            pthread_mutex_unlock(&resource_lock);
            return 0;
        } else {
            // Rollback
            printf(COLOR_RED "[DENIED] Customer %d request leads to UNSAFE state." COLOR_RESET "\n", customer_num);
            for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
                available[i] += request[i];
                allocation[customer_num][i] -= request[i];
                need[customer_num][i] += request[i];
            }
            pthread_mutex_unlock(&resource_lock);
            return -1;
        }
    } else {
        // RECOVERY MODE (Blind Allocation)
        for(int j=0; j<NUMBER_OF_RESOURCES; j++) active_request[customer_num][j] = 0;
        printf(COLOR_GREEN "[GRANTED] Customer %d request approved (Blind Allocation)." COLOR_RESET "\n", customer_num);
        pthread_mutex_unlock(&resource_lock);
        return 0;
    }
}

int release_resources(int customer_num, int release[]) {
    pthread_mutex_lock(&resource_lock);
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
        allocation[customer_num][i] -= release[i];
        available[i] += release[i];
    }
    printf(COLOR_YELLOW "[RELEASED] Customer %d released resources." COLOR_RESET "\n", customer_num);
    pthread_mutex_unlock(&resource_lock);
    return 0;
}

// ---------------------------------------------------------
// PHASE 3: Deadlock Monitor Background Thread
// ---------------------------------------------------------
void* deadlock_monitor_thread(void* arg) {
    (void)arg; // Ignore unused parameter warning
    while(1) {
        sleep(2); // Check every 2 seconds
        if (operating_mode == 0) continue; // Only check if in Recovery Mode

        pthread_mutex_lock(&resource_lock);
        
        int work[NUMBER_OF_RESOURCES];
        bool finish[NUMBER_OF_CUSTOMERS] = {false};
        for (int i = 0; i < NUMBER_OF_RESOURCES; i++) work[i] = available[i];
        
        for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
            bool has_alloc = false;
            for(int j=0; j<NUMBER_OF_RESOURCES; j++) {
                if (allocation[i][j] > 0) has_alloc = true;
            }
            if (!has_alloc) finish[i] = true;
        }

        bool progress = true;
        while (progress) {
            progress = false;
            for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
                if (!finish[i]) {
                    bool can_satisfy = true;
                    for (int j = 0; j < NUMBER_OF_RESOURCES; j++) {
                        if (active_request[i][j] > work[j]) can_satisfy = false;
                    }
                    if (can_satisfy) {
                        for (int j = 0; j < NUMBER_OF_RESOURCES; j++) work[j] += allocation[i][j];
                        finish[i] = true;
                        progress = true;
                    }
                }
            }
        }

        // Check if any process is deadlocked and recover
        for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
            if (!finish[i]) {
                printf(COLOR_RED "\n!!! [DEADLOCK DETECTED] Customer %d is stuck! Initiating Recovery !!!" COLOR_RESET "\n", i);
                
                // Victim selection: Terminate Customer 'i' and release its resources
                for (int j = 0; j < NUMBER_OF_RESOURCES; j++) {
                    available[j] += allocation[i][j];
                    allocation[i][j] = 0;
                    active_request[i][j] = 0; // Clear its active request
                }
                printf(COLOR_YELLOW ">>> [RECOVERY] Terminated Customer %d and reclaimed resources. <<<" COLOR_RESET "\n\n", i);
                break; // Recover one victim at a time
            }
        }
        pthread_mutex_unlock(&resource_lock);
    }
    return NULL;
}