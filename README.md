# Deadlock Detection & Recovery Manager

This project is a multithreaded Operating System resource manager implemented in C. It simulates an OS environment where multiple concurrent processes (clients) compete for limited shared resources. The system utilizes **Banker's Algorithm** to control resource allocation, prevent unsafe states, and avoid deadlocks.

**Developed by Team 12:**
* Abdulrahman Alhaidari 
* Kenan Kaddoura 
* Ayman

---

## 🚀 Features
* **Multithreaded Simulation:** Uses POSIX threads (`pthreads`) to simulate independent processes requesting and releasing resources concurrently.
* **Banker's Algorithm:** Implements robust safety checks to ensure the system never enters a deadlock state.
* **Thread-Safe Operations:** Protects shared memory matrices (Available, Max, Allocation, Need) using strict `pthread_mutex` locks to prevent race conditions.
* **Dynamic Scenarios:** Generates randomized, valid request and release parameters for each thread based on their current needs and allocations.

---

## 📁 Project Structure
```text
📦 deadlock-manager
 ┣ 📂 src
 ┃ ┣ 📜 main.c          # Entry point, initialization, and thread generation
 ┃ ┣ 📜 banker.c        # Core Resource Manager logic & Banker's Algorithm
 ┃ ┣ 📜 banker.h        # System state matrices & function prototypes
 ┃ ┣ 📜 simulator.c     # Client simulation logic (requests & releases)
 ┃ ┗ 📜 simulator.h     # Simulator function prototypes
 ┣ 📜 Makefile          # Automated build system commands
 ┗ 📜 README.md         # Project documentation
```
---

## ⚙️ Compilation & Building
The project includes a Makefile for automated compilation. Run the following commands in the root directory:
### Compile the project:
```
make
```
This will compile the source code and generate an executable named deadlock_manager.

### Clean build artifacts:
```
make clean
```
Use this to remove compiled .o files and the executable for a fresh build.

---

## 📊 Default System Configuration

By default, the simulation initializes with **5 Customers** and **3 Resource Types**. The maximum resource demand for each customer is hardcoded in the system for testing purposes as follows:

| Customer ID | Resource 1 | Resource 2 | Resource 3 |
| :---: | :---: | :---: | :---: |
| **0** | 7 | 5 | 3 |
| **1** | 3 | 2 | 2 |
| **2** | 9 | 0 | 2 |
| **3** | 2 | 2 | 2 |
| **4** | 4 | 3 | 3 |

*Note: You can modify the `temp_max` array inside `src/main.c` to test different system stress limits and deadlock scenarios.*

---

## 💻 Usage & Testing
The system supports two distinct operational modes. You must specify the initial available resources as command-line arguments. The default configuration uses **3 resource types** and **5 customers**.
### Command Syntax:
```bash
# For Avoidance (Banker's Algorithm) Mode:
./deadlock_manager <resource_1> <resource_2> <resource_3>

# For Detection & Recovery Mode:
./deadlock_manager recovery <resource_1> <resource_2> <resource_3>
```

### Phase 3 Test Scenarios
**1. Banker Mode: Avoidance under High Stress**  
Algorithm will frequently deny requests to protect the system, preventing deadlocks entirely.
```
./deadlock_manager 8 4 4
```

**2. Recovery Mode: Deadlock Detection & Victim Termination**  
Algorithm, forcing a deadlock. The background monitor will detect the circular wait, terminate a victim process, and reclaim its resources to allow the system to recover.
```
./deadlock_manager recovery 3 2 2
```

### 📝 Execution Results

Below are the raw console outputs demonstrating the system's behavior in both modes.

**Mode 1: Banker's Algorithm (Avoidance)**  

**Command:** `./deadlock_manager 8 4 4`
**Result:**
```text
System Initialized in BANKER MODE.
Initial Available Resources: 8 4 4

[Customer 0] Attempting to REQUEST: 3 4 1
[DENIED] Customer 0 request leads to UNSAFE state.
[Customer 1] Attempting to REQUEST: 0 2 0
[DENIED] Customer 1 request leads to UNSAFE state.
[Customer 4] Attempting to REQUEST: 1 1 0
[DENIED] Customer 4 request leads to UNSAFE state.
...
[Simulation completes safely with no deadlocks]
```

**Mode 2: Detection & Recovery (Phase 3)**  

**Command:** `./deadlock_manager recovery 3 2 2`
**Result:**
```text
System Initialized in RECOVERY MODE.
Initial Available Resources: 3 2 2

[Customer 0] Attempting to REQUEST: 2 2 1
[GRANTED] Customer 0 request approved (Blind Allocation).
[Customer 3] Attempting to REQUEST: 0 0 1
[GRANTED] Customer 3 request approved (Blind Allocation).

!!! [DEADLOCK DETECTED] Customer 0 is stuck! Initiating Recovery !!!
>>> [RECOVERY] Terminated Customer 0 and reclaimed resources. <<<

[Customer 3] Attempting to RELEASE: 0 0 1
[RELEASED] Customer 3 released resources.
[Customer 3] Finished execution.
--- Simulation Complete ---
```