# Day 2: Toolchain & "Hello QNX"

Now that your QNX environment is running, it's time to write and run your first program. In the world of embedded systems, we rarely compile code on the device itself. Instead, we use **Cross-compilation**.

## 1. What is Cross-compilation?

- **Host**: The powerful machine where you write and compile code (your Linux Ubuntu machine).
- **Target**: The device where the code actually runs (your QNX QEMU instance).

Because the Host and Target might have different CPU architectures (e.g., your PC is x86_64, but your embedded board might be ARM), you need a special compiler that runs on the Host but produces binaries for the Target.

## 2. The QNX Compiler: `qcc`

QNX uses a wrapper called `qcc` which sits on top of GCC. To compile for a specific architecture, you use the `-V` flag.

Common variants:
- `qcc -Vgcc_ntox86_64`: Compile for 64-bit x86 (Standard for QNX 8.x).
- `qcc -Vgcc_ntoaarch64le`: Compile for 64-bit ARM (Little Endian).

## 3. Communication: Host to Target

To run your code, you must move the compiled binary from your Linux machine to the QNX Target. The most common methods are:
1. **SCP (Secure Copy)**: Over the network.
2. **Shared Folder**: If using VMware/VirtualBox.
3. **FTP**: A classic but less secure method.

---

## 🏁 Day 2 Mission: Hello QNX

Your goal is to compile a simple C program on Linux and run it on QNX.

### Step 1: Write the code
Create a file named `hello.c` on your Linux machine:
```c
#include <stdio.h>
#include <unistd.h>

int main() {
    char hostname[1024];
    gethostname(hostname, 1024);
    printf("Hello QNX! I am running on: %s\n", hostname);
    return 0;
}
```

### Step 2: Compile it
Open a terminal on Linux, source the environment, and compile:
```bash
source ~/qnx800/qnxsdp-env.sh
qcc -Vgcc_ntox86_64 -o hello hello.c
```

### Step 3: Transfer to Target
Find the IP address of your QNX target (run `ifconfig` in QNX). Then, from Linux:
```bash
scp hello qnxuser@<TARGET_IP>:/tmp
```
*(Note: Default password is often "qnxuser" or empty depending on your image).*

### Step 4: Run it on QNX
In your QNX terminal:
```bash
chmod +x /tmp/hello
/tmp/hello
```

**Submit the output of your "Hello QNX" program and any questions about the toolchain!**
