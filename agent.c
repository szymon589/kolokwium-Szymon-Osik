#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include "agent.h"

static const char* agent_name = NULL;

void handle_sigusr1(int sig) {
    printf("%s: Czekam na rozkazy\n", agent_name);
}

void handle_sigusr2(int sig) {
    printf("%s: Rozpoczynam misję\n", agent_name);
}

void handle_sigterm(int sig) {
    printf("%s: Misja zakończona\n", agent_name);
    exit(0);
}

void agent_behavior(const char* name, int fd_read, int fd_write) {
    agent_name = name;

    printf("Imię agenta: Gotowy do misji\n");

    signal(SIGUSR1, handle_sigusr1);
    signal(SIGUSR2, handle_sigusr2);
    signal(SIGTERM, handle_sigterm);

    char buffer[128];
    while (1) {
        int bytes = read(fd_read, buffer, sizeof(buffer) - 1);
        if (bytes <= 0) continue;
        buffer[bytes] = '\0';

        if (strcmp(buffer, "Identify!") == 0) {
            dprintf(fd_write, "My codename is %s\n", name);
        } else if (strcmp(buffer, "Status?") == 0) {
            dprintf(fd_write, "%s: My PID is: %d\n", name, getpid());
        }
    }
}
