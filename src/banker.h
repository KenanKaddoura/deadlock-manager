#ifndef BANKER_H
#define BANKER_H

#include <stdbool.h>
#include <pthread.h>

// Colors for readable print statements in the terminal. 
#define COLOR_RESET   "\x1b[0m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_RED     "\x1b[31m"
#define COLOR_YELLOW  "\x1b[33m"
#define COLOR_CYAN    "\x1b[36m"

// Define system limits 
#define NUMBER_OF_CUSTOMERS 5
#define NUMBER_OF_RESOURCES 3


// The available amount of each resource
extern int available[NUMBER_OF_RESOURCES];

// The maximum demand of each customer
extern int maximum[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];

// The amount currently allocated to each customer
extern int allocation[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];

// The remaining need of each customer
extern int need[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];

// Mutex lock to prevent race conditions when threads request/release
extern pthread_mutex_t resource_lock;


/**
 * Request resources for a specific customer.
 * Returns 0 if successful (granted), -1 if denied (would lead to unsafe state).
 */
int request_resources(int customer_num, int request[]);

/**
 * Release resources for a specific customer.
 * Returns 0 if successful, -1 if invalid release.
 */
int release_resources(int customer_num, int release[]);

/**
 * Banker's Algorithm safety check.
 * Returns true if the system is in a safe state, false otherwise.
 */
bool is_safe();

#endif 