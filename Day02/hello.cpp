#include <stdio.h>
#include <unistd.h>

int main() {
    char hostname[1024];
    gethostname(hostname, 1024);
    printf("Hello QNX! I am running on: %s\n", hostname);
    return 0;
}