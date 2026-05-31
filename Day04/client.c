#include <stdio.h>
#include <stdlib.h>
#include <sys/neutrino.h>
#include <sys/netmgr.h>

int main(int argc, char *argv[]) {
    if (argc <3) {
        fprintf(stderr, "Usage: %s <server_pid>\n", argv[0]);
        return EXIT_FAILURE;
    }
    int pid;
    int chid;
    int coid;
    char reply[100] = {0};
    char msg[100] = {0};


    // 1. Connect to the server's channel using the server's PID
    pid = atoi(argv[1]);
    chid = atoi(argv[2]);
    printf("Client is running. Attempting to connect to server with PID: %d and Channel ID: %d\n", pid, chid);
    coid = ConnectAttach(ND_LOCAL_NODE, pid, chid, _NTO_SIDE_CHANNEL, 0);
    
    if (coid == -1) {
        perror("ConnectAttach");
        return EXIT_FAILURE;
    }
    printf("Connected to server with PID: %d\n", atoi(argv[1]));

    // 2. Send a message to the server
    snprintf(msg, sizeof(msg), "Hello from client with PID: %d", getppid());
    
    if (MsgSend(coid, msg, sizeof(msg), reply, sizeof(reply)) == -1) {
        perror("MsgSend");
        ConnectDetach(coid);
        return EXIT_FAILURE;
    }
    printf("Message sent to server: %s\n", msg);

    // 3. Wait for a reply from the server
    printf("Received reply from server: %s\n", reply);

    // Cleanup
    ConnectDetach(coid);
    return EXIT_SUCCESS;
}