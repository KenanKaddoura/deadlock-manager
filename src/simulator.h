#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <pthread.h>

/**
 * The thread function that simulates a single customer/process.
 * It will run in a loop, generating random resource requests, 
 * holding them briefly, and then releasing them.
 * * @param customer_num A pointer to the customer's ID integer.
 */
void* client_thread(void* customer_num);

#endif 