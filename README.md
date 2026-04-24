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
The system requires you to specify the initial available resources as command-line arguments when executing the program. The default configuration uses 3 resource types and 5 customers.
### Command Syntax:
```
./deadlock_manager <resource_1> <resource_2> <resource_3>
```

### Test Scenarios
**1. Normal Operation (Balanced Resources)**  
Provides a balanced environment where threads compete but can eventually finish.
```
./deadlock_manager 10 5 7
```

**2. Resource Abundance (Stress-Free)**  
Provides more than enough resources. You will see near-instant approvals.
```
./deadlock_manager 50 50 50
```

**3. Resource Scarcity (High Stress)**  
Severely limits resources. The Banker's Algorithm will frequently deny requests to protect the system, forcing threads to wait.
```
./deadlock_manager 8 4 4
```

### 📝 Execution Results

Below are the raw console outputs demonstrating the system's behavior and the Banker's Algorithm in action under different resource constraints.

**Command:** `make`
**Result:**
```text
gcc -Wall -Wextra -g -pthread -c src/main.c -o src/main.o
gcc -Wall -Wextra -g -pthread -c src/banker.c -o src/banker.o
gcc -Wall -Wextra -g -pthread -c src/simulator.c -o src/simulator.o
gcc -Wall -Wextra -g -pthread -o deadlock_manager src/main.o src/banker.o src/simulator.o
```

**Command:** `./deadlock_manager 10 5 7`
**Result:**
<img width="469" height="1032" alt="image" src="https://github.com/user-attachments/assets/b8e8af2c-1306-4fe3-8022-7cf1f49c16d2" />

**Command:** `./deadlock_manager 50 50 50`
**Result:**
<img width="391" height="1033" alt="image" src="https://github.com/user-attachments/assets/5aae50b1-448e-408f-ae90-c46165572a9b" />
<img width="387" height="137" alt="image" src="https://github.com/user-attachments/assets/35ce14b6-1fdd-4039-a0ee-b0919283cbaf" />


**Command:** `./deadlock_manager 8 4 4`
**Result:**
<img width="465" height="798" alt="image" src="https://github.com/user-attachments/assets/9a8c96be-b85d-47ff-93fe-115e42c2d860" />

