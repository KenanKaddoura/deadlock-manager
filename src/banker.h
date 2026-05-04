#ifndef BANKER_H
#define BANKER_H

#include <stdbool.h>
#include <pthread.h>

#define NUMBER_OF_CUSTOMERS 5
#define NUMBER_OF_RESOURCES 3

// ---------------------------------------------------------
// Global State Matrices 
// ---------------------------------------------------------
extern int available[NUMBER_OF_RESOURCES];
extern int maximum[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
extern int allocation[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
extern int need[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
extern pthread_mutex_t resource_lock;

// --- NEW FOR PHASE 3: Recovery Mode & Detection ---
extern int operating_mode; // 0 = Banker Mode, 1 = Recovery Mode
extern int active_request[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];

// Function Prototypes
int request_resources(int customer_num, int request[]);
int release_resources(int customer_num, int release[]);
bool is_safe();
void* deadlock_monitor_thread(void* arg); // New Monitor Thread

// Colors for terminal output
#define COLOR_RESET   "\x1b[0m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_RED     "\x1b[31m"
#define COLOR_YELLOW  "\x1b[33m"
#define COLOR_CYAN    "\x1b[36m"

#endif // BANKER_H