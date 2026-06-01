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