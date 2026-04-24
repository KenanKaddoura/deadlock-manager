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


## ⚙️ Compilation & Building
The project includes a Makefile for automated compilation. Run the following commands in the root directory:
Compile the project:
