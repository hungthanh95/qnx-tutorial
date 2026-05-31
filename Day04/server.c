#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/neutrino.h>

int main(int argc, char *argv[]) {
    int chid;
    int rcvid;
    char msg[100];

    // 1. Create a channel for communication
    chid = ChannelCreate(0);
 
    if (chid == -1) {
        perror("ChannelCreate");
        return EXIT_FAILURE;
    }
    printf("Server is running. Channel created with ID: %d, PID: %d\n", chid, getppid());

    while (1) {
        // 2. Wait for a message from a client (Receive-blocked)
        rcvid = MsgReceive(chid, msg, sizeof(msg), NULL);
        if (rcvid == -1) {
            perror("MsgReceive");
            continue; // Continue to wait for the next message
        }

        printf("Received a message from client with RCVID: %d\n", rcvid);

        // 3. Send a reply back to the client
        if (MsgReply(rcvid, 0, "Hello from the server!", 22) == -1) {
            perror("MsgReply");
        }
    }

    // Cleanup (unreachable in this example)
    ChannelDestroy(chid);
    return EXIT_SUCCESS;
}