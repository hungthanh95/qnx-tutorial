# Day 4: The Core - Message Passing (IPC)

Today is the most important day. We are learning **Message Passing**, the "Secret Sauce" of QNX. In QNX, almost everything—from reading a file to sending a packet—is done via message passing between your application and a system process.

## 1. The Synchronous Model

QNX IPC follows a strict **Send-Receive-Reply** cycle. It is **synchronous** and **blocking**.

1.  **MsgSend(connection, ...)**: The Client sends a message and **blocks** (REPLY-blocked) until the Server replies.
2.  **MsgReceive(channel, ...)**: The Server waits for a message and **blocks** (RECEIVE-blocked) until a message arrives.
3.  **MsgReply(receive_id, ...)**: The Server processes the message and sends a reply back. This unblocks the Client.

### Why is this efficient?
- **Zero-copy (almost)**: Data is copied directly from the Client's memory to the Server's memory.
- **Priority Inheritance**: If a high-priority Client sends a message to a low-priority Server, the Server's priority is temporarily boosted to match the Client's. This prevents "Priority Inversion."

## 2. Channels and Connections

- To receive messages, a process creates a **Channel** (`ChannelCreate`).
- To send messages, a process creates a **Connection** (`ConnectAttach`) to that Channel.

---

## 🏁 Day 4 Mission: Server-Client Communication

Your goal is to create two separate programs: a **Server** that waits for a name and a **Client** that sends its name and gets a greeting back.

### Step 1: The Server (`server.c`)

```c
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/neutrino.h>

int main() {
    int chid;
    int rcvid;
    char msg[100];

    // 1. Create a Channel
    chid = ChannelCreate(0);
    printf("Server is running. Channel ID: %d, PID: %d\n", chid, getpid());

    while (1) {
        // 2. Wait for a message (Receive-blocked)
        rcvid = MsgReceive(chid, msg, sizeof(msg), NULL);
        
        printf("Server received: %s\n", msg);

        // 3. Process and Reply
        char reply[] = "Hello from Server!";
        MsgReply(rcvid, 0, reply, sizeof(reply));
    }
    return 0;
}
```

### Step 2: The Client (`client.c`)

```c
#include <stdio.h>
#include <stdlib.h>
#include <sys/neutrino.h>
#include <sys/netmgr.h>

int main(int argc, char **argv) {
    if (argc < 3) {
        printf("Usage: client <PID> <CHID>\n");
        return 1;
    }

    int pid = atoi(argv[1]);
    int chid = atoi(argv[2]);
    char reply[100];

    // 1. Establish a Connection
    int coid = ConnectAttach(ND_LOCAL_NODE, pid, chid, _NTO_SIDE_CHANNEL, 0);

    // 2. Send a Message (Reply-blocked)
    char msg[] = "I am the Client";
    MsgSend(coid, msg, sizeof(msg), reply, sizeof(reply));

    printf("Server replied: %s\n", reply);

    ConnectDetach(coid);
    return 0;
}
```

### Step 3: Run and Observe
1. Start the **Server** first. Note its PID and CHID.
2. Open another terminal and start the **Client**: `./client <SERVER_PID> <SERVER_CHID>`.
3. **Advanced Observation**: 
   - While the Server is waiting, run `pidin -p <SERVER_PID> threads`. It should be in **RECEIVE** state.
   - If you can catch the Server during processing (add a `sleep(5)` before `MsgReply`), check the Client's state. It should be in **REPLY** state.

**Submit your results! This is the foundation for Day 6 & 7 (Resource Managers).**
