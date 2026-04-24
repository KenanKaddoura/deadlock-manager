#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "simulator.h" // Inludes client_thread signature
#include "banker.h" // Inludes global variables: system limits

void* client_thread(void* arg) {
    // Cast the argument back to an integer to get the customer ID
    int customer_num = *(int*)arg;
    int request[NUMBER_OF_RESOURCES];
    int release[NUMBER_OF_RESOURCES];

    // Seed the random number generator uniquely for each thread using its ID
    unsigned int seed = time(NULL) ^ customer_num; 

    // Simulate the thread making 3 rounds of requests for testing
    for (int loop = 0; loop < 3; loop++) {
        
        // Generate a random request
        bool valid_request = false;
        for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
            if (need[customer_num][i] > 0) {
                // Request a random amount between 0 and their current need
                request[i] = rand_r(&seed) % (need[customer_num][i] + 1);
                if (request[i] > 0) valid_request = true;
            } else {
                request[i] = 0;
            }
        }

        // If the thread randomly requested nothing, skip this iteration
        if (!valid_request) {
            continue; 
        }

        printf(COLOR_CYAN "[Customer %d] Attempting to REQUEST: %d %d %d" COLOR_RESET "\n", customer_num, request[0], request[1], request[2]);

        // Make the request to the Resource Manager
        if (request_resources(customer_num, request) == 0) {
            // GRANTED: Simulate doing work with the resources by sleeping for 3-5 seconds
            sleep(rand_r(&seed) % 3 + 5); 

            // Generate a random release (must be <= what is currently allocated)
            bool valid_release = false;
            for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
                if (allocation[customer_num][i] > 0) {
                    release[i] = rand_r(&seed) % (allocation[customer_num][i] + 1);
                    if (release[i] > 0) valid_release = true;
                } else {
                    release[i] = 0;
                }
            }

            // Release the resources
            if (valid_release) {
                printf("[Customer %d] Attempting to RELEASE: %d %d %d\n", 
                       customer_num, release[0], release[1], release[2]);
                release_resources(customer_num, release);
            }
        } else {
            // DENIED: Sleep briefly before trying again to avoid spamming the console
            sleep(1);
        }
    }

    printf("[Customer %d] Finished execution.\n", customer_num);
    return NULL;
}