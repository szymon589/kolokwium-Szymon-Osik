#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include "coordinator.h"
#include "agent.h"

void start_agents() {
    const char* names[] = {"Gamma", "Beta", "Alpha", "Delta"};

    for (int i = 0; i < 4; ++i) {
        int pipefd[2];
        if (pipe(pipefd) == -1) {
            perror("pipe");
            exit(1);
        }

        pid_t pid = fork();

        if (pid < 0) {
            perror("fork");
            exit(1);
        }

        if (pid == 0) {
            // Proces potomny - agent
            close(pipefd[1]); // zamykamy koniec do zapisu
            agent_behavior(names[i], pipefd[0], STDOUT_FILENO);
            exit(0);
        } else {
            // Proces rodzica - koordynator
            close(pipefd[0]); // zamykamy koniec do czytania

            sleep(1);
            write(pipefd[1], "Identify!", strlen("Identify!"));
            sleep(1);
            write(pipefd[1], "Status?", strlen("Status?"));
            close(pipefd[1]);
        }
    }

    for (int i = 0; i < 4; ++i) {
        wait(NULL);
    }
}

