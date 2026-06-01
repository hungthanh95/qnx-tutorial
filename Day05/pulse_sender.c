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