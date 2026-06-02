# Day 5: IPC - Pulses & Shared Memory

On Day 4, we learned about synchronous Message Passing. Today, we look at the alternatives for when you don't want to block the sender or when you need to move massive amounts of data.

## 1. Pulses: The Asynchronous Signal

A **Pulse** is a special type of message that is:
- **Non-blocking**: The sender (`MsgSendPulse`) sends it and continues immediately.
- **Fixed-size**: It only carries a small payload (8-bit code and 32-bit/64-bit value).
- **Asynchronous**: It is queued by the kernel on the receiver's channel.

### Why use Pulses?
- **Interrupt Handlers**: Hardware interrupts cannot block; they use pulses to notify a driver.
- **Timers**: When a timer expires, it sends a pulse to your program.
- **Event Notification**: Letting a process know "something happened" without waiting for it to finish.

---

## 🏁 Day 5 Mission 1: The Pulse Listener

Your goal is to create a program that waits for both messages AND pulses on the same channel. This is how real QNX drivers work!

### The Multi-Event Receiver (`pulse_receiver.c`)

```c
#include <stdio.h>
#include <stdlib.h>
#include <sys/neutrino.h>
#include <sys/iomsg.h>

int main() {
    int chid, rcvid;
    struct _pulse pulse;

    chid = ChannelCreate(0);
    printf("Receiver ready. CHID: %d, PID: %d\n", chid, getpid());

    while (1) {
        // MsgReceive handles both regular messages and pulses
        rcvid = MsgReceive(chid, &pulse, sizeof(pulse), NULL);

        if (rcvid == 0) {
            // It's a Pulse!
            printf("Received a PULSE! Code: %d, Value: %d\n", pulse.code, pulse.value.sival_int);
        } else if (rcvid > 0) {
            // It's a regular Message
            printf("Received a MESSAGE from rcvid %d\n", rcvid);
            MsgReply(rcvid, 0, NULL, 0);
        }
    }
    return 0;
}
```

### The Pulse Sender (`pulse_sender.c`)

```c
#include <stdio.h>
#include <stdlib.h>
#include <sys/neutrino.h>
#include <sys/netmgr.h>

int main(int argc, char **argv) {
    if (argc < 3) {
        printf("Usage: pulse_sender <PID> <CHID>\n");
        return 1;
    }

    int pid = atoi(argv[1]);
    int chid = atoi(argv[2]);

    int coid = ConnectAttach(ND_LOCAL_NODE, pid, chid, _NTO_SIDE_CHANNEL, 0);

    // Send a pulse (Non-blocking!)
    printf("Sending pulse...\n");
    MsgSendPulse(coid, 10, _PULSE_CODE_MINAVAIL, 12345);
    printf("Pulse sent. Sender is NOT blocked.\n");

    return 0;
}
```

### Observation Task
1. Run `pulse_receiver`.
2. Run `pulse_sender`.
3. Notice that the sender finishes immediately, while the receiver processes the pulse.
4. Try to find your shared memory objects in QNX by running `ls /dev/shmem`.

**Pulse_receiver created a connect with PID 847910, pulse_sender sent a pulse to PID 847910.**
![](pulse_sender_receiver.png)

## 2. Shared Memory: The Zero-copy King

When you need to transfer megabytes of data (like a 4K camera frame), copying it via messages is too slow. **Shared Memory** allows two processes to look at the exact same physical RAM.

### The Setup Workflow:
1. **Process A**: Creates a memory object using `shm_open()`.
2. **Process A**: Sets the size with `ftruncate()`.
3. **Both Processes**: Map that memory into their own space using `mmap()`.

```mermaid
graph LR
    subgraph "Process A"
        A_Ptr[Pointer A]
    end
    subgraph "Physical RAM"
        SharedBlock[Shared Memory Block]
    end
    subgraph "Process B"
        B_Ptr[Pointer B]
    end
    A_Ptr --> SharedBlock
    B_Ptr --> SharedBlock
```
---

## 🏁 Day 5 Mission 2: The Shared Secret

### Step 1: Create the "Writer" (`shm_writer.c`)

```c
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

int main() {
    int fd = shm_open("/my_shared_mem", O_RDWR | O_CREAT, 0666);
    ftruncate(fd, 4096);
    
    char *ptr = mmap(0, 4096, PROT_READ | O_RDWR, MAP_SHARED, fd, 0);
    
    sprintf(ptr, "QNX is fast!");
    printf("Writer: I wrote a secret to memory.\n");
    
    munmap(ptr, 4096);
    close(fd);
    return 0;
}
```

### Step 2: Create the "Reader" (`shm_reader.c`)

```c
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

int main() {
    int fd = shm_open("/my_shared_mem", O_RDONLY, 0666);
    char *ptr = mmap(0, 4096, PROT_READ, MAP_SHARED, fd, 0);
    
    printf("Reader: I found the secret: %s\n", ptr);
    
    munmap(ptr, 4096);
    close(fd);
    shm_unlink("/my_shared_mem"); // Clean up
    return 0;
}
```

**Submission Task:**
1. Run the writer, then the reader.
2. Run `ls -l /dev/shmem`. Can you see your memory object there?
3. **Observation**: What happens if you run the reader twice?

**Submit your terminal logs and tell me: Why would you use a Pulse along with Shared Memory?**


