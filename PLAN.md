# QNX 10-Day Mastery Plan (80/20 Rule)

This plan focuses on the core 20% of QNX features that handle 80% of real-world RTOS development: The Microkernel, Message Passing, and Resource Managers.

## Schedule: 1 Hour/Day

| Day | Topic | Key Learning Objectives |
|:---|:---|:---|
| 1 | **Introduction & Architecture** | Microkernel vs. Monolithic, QNX Philosophy, Setup. |
| 2 | **Toolchain & "Hello QNX"** | Cross-compiling, Momentics IDE/Command line, Target communication. |
| 3 | **Processes & Threads** | QNX states, Priority-based preemptive scheduling. |
| 4 | **The Core: Message Passing (IPC)** | MsgSend, MsgReceive, MsgReply (The QNX "Secret Sauce"). |
| 5 | **Pulses & Shared Memory** | Non-blocking IPC and high-speed data sharing. |
| 6 | **Resource Managers (Part 1)** | Introduction to `/dev/` and the I/O framework. |
| 7 | **Resource Managers (Part 2)** | Implementing a simple `/dev/null` or `/dev/mydevice`. |
| 8 | **Synchronization** | Mutexes, Semaphores, and Atomic operations in RTOS. |
| 9 | **Timers & Interrupts** | Handling real-time events and hardware signals. |
| 10 | **Debugging & Profiling** | System Profiler, `pidin`, and performance optimization. |

## Deliverables
- Each day results in a `DayXX_Topic.md` file in English.
- Code samples will be stored in `src/dayXX/`.
