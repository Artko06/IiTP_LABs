#define _POSIX_C_SOURCE 200809L

#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#define LOGFILE "self_healing.log"

volatile sig_atomic_t restart_requested = 0;

void handle_signal(int sig)
{
    if (sig == SIGTERM || sig == SIGINT)
    {
        restart_requested = 1;
    }
}

void log_state(pid_t pid, int counter)
{
    int fd = open(LOGFILE, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd < 0)
        return;

    char buf[128];
    int len = snprintf(buf, sizeof(buf),
                       "PID=%d counter=%d\n", pid, counter);

    write(fd, buf, len);
    close(fd);
}

int main()
{
    struct sigaction sa;
    sa.sa_handler = handle_signal;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;

    if (sigaction(SIGTERM, &sa, NULL) == -1)
    {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }
    if (sigaction(SIGINT, &sa, NULL) == -1)
    {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    int counter = 0;
    pid_t pid = getpid();

    while (1)
    {
        counter++;
        log_state(pid, counter);
        sleep(3);

        if (restart_requested)
        {
            pid_t child = fork();

            if (child < 0)
            {
                _exit(EXIT_FAILURE);
            }

            if (child == 0)
            {
                pid = getpid();
                restart_requested = 0;
                continue;
            }
            else
            {
                _exit(EXIT_SUCCESS);
            }
        }
    }
}

/*
make run
ps -f | grep self_healing

kill -TERM <pid>
ps -f | grep self_healing

pkill -9 -f self_healing
make clean
*/