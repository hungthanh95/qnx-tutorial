# Deep Dive: Understanding QNX Buildfiles (.build)

A `.build` file is a text configuration file used by the `mkifs` utility to create an **Image File System (IFS)**. Think of it as a "manifest" or "recipe" for your entire operating system.

## 1. Structure of a Buildfile

A buildfile typically consists of three main parts:
1.  **Search Paths**: Where to find files on your host machine (Linux).
2.  **Boot Attributes**: Kernel parameters and memory settings.
3.  **Boot Script**: The list of commands executed by the kernel after it starts.
4.  **File List**: The files, libraries, and drivers to be included in the image.

---

## 2. Key Sections Explained

### A. The [search] Attribute
This tells `mkifs` where to look for the binaries and libraries on your development machine.
```bash
[search=${QNX_TARGET}/x86_64/bin:${QNX_TARGET}/x86_64/lib]
```

### B. The [boot] Attributes
This section defines the kernel and how it should start.
- `procnto`: The name of the QNX kernel/process manager.
- `-v`: Verbose output during boot.
```bash
[virtual=x86_64,elf] .bootstrap = {
    procnto-smp-instr -v
}
```

### C. The Boot Script
Everything after the `[+script]` tag is executed sequentially by the kernel.
```bash
[+script] .script = {
    # Start the system logger
    slogger2 &
    
    # Start the pipe manager (needed for shells)
    pipe &
    
    # Start a shell on the serial console
    devc-ser8250 -e -b115200 0x3f8,4 &
    reopen /dev/ser1
    [+session] sh
}
```

### D. The File List
You must explicitly list every file you want inside the OS. If a file isn't listed here, it won't exist in your QNX system.
- `[type=link]`: Creates a symbolic link.
- `libc.so`: The standard C library (essential!).
```bash
# Include the C library
libc.so

# Include basic commands
ls
cat
pidin
```

---

## 3. A Complete "Hello World" Buildfile (QEMU x86_64)

Save this as `minimal.build`:

```bash
[search=${QNX_TARGET}/x86_64/boot/sys:${QNX_TARGET}/x86_64/bin:${QNX_TARGET}/x86_64/lib]

[virtual=x86_64,elf] .bootstrap = {
    procnto-smp-instr -v
}

[+script] .script = {
    # Initialize basic drivers
    devc-ser8250 -e -b115200 0x3f8,4 &
    reopen /dev/ser1
    
    display_msg "----------------------------------------"
    display_msg "Welcome to your Custom QNX Image!"
    display_msg "----------------------------------------"
    
    # Start a shell
    [+session] sh
}

# Essential Libraries
libc.so

# Essential Binaries
sh
ls
pidin
date
```

---

## 4. How to Build and Run

1.  **Source the environment**:
    ```bash
    source ~/qnx800/qnxsdp-env.sh
    ```
2.  **Generate the IFS**:
    ```bash
    mkifs minimal.build minimal.ifs
    ```
3.  **Boot in QEMU**:
    *Note: Sometimes the QNX environment (LD_LIBRARY_PATH) conflicts with your host's QEMU. If you see a "symbol lookup error", run it like this:*
    ```bash
    env -u LD_LIBRARY_PATH qemu-system-x86_64 -kernel minimal.ifs -serial stdio
    ```

---

## 🏁 Mission: The Custom Banner
Try to modify the `minimal.build` file to:
1.  Change the `display_msg` to show your own name or blog title.
2.  Add the `date` command to the file list so you can check the system time after booting.
3.  Rebuild and run it in QEMU.
