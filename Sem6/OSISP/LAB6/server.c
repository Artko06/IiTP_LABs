#define _POSIX_C_SOURCE 200809L

#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BACKLOG 5
#define BUFFER_SIZE 256

static volatile sig_atomic_t keep_running = 1;

static void handle_sigint(int sig)
{
    (void)sig;
    keep_running = 0;
}

static int read_line(int fd, char *buffer, size_t size)
{
    size_t total = 0;

    while (total + 1 < size)
    {
        char ch;
        ssize_t n = recv(fd, &ch, 1, 0);

        if (n == 0)
            return 0;
        if (n < 0)
            return -1;

        buffer[total++] = ch;
        if (ch == '\n')
            break;
    }

    buffer[total] = '\0';
    return 1;
}

int main(int argc, char *argv[])
{
    int server_fd = -1;
    int client_fd = -1;
    int port;
    struct sockaddr_in server_addr;

    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        return EXIT_FAILURE;
    }

    port = atoi(argv[1]);
    if (port <= 0 || port > 65535)
    {
        fprintf(stderr, "Invalid port: %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    signal(SIGINT, handle_sigint);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0)
    {
        perror("socket");
        return EXIT_FAILURE;
    }

    {
        int opt = 1;
        if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
        {
            perror("setsockopt");
            close(server_fd);
            return EXIT_FAILURE;
        }
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons((unsigned short)port);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("bind");
        close(server_fd);
        return EXIT_FAILURE;
    }

    if (listen(server_fd, BACKLOG) < 0)
    {
        perror("listen");
        close(server_fd);
        return EXIT_FAILURE;
    }

    printf("Server started on port %d\n", port);
    printf("Service: number processing (square and cube)\n");

    while (keep_running)
    {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        char buffer[BUFFER_SIZE];
        char response[BUFFER_SIZE];
        char client_ip[INET_ADDRSTRLEN];
        char *endptr;
        long value;
        long long square;
        long long cube;
        int status;

        client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
        if (client_fd < 0)
        {
            if (errno == EINTR && !keep_running)
                break;
            perror("accept");
            continue;
        }

        if (inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, sizeof(client_ip)) == NULL)
            strcpy(client_ip, "unknown");

        status = read_line(client_fd, buffer, sizeof(buffer));
        if (status <= 0)
        {
            if (status < 0)
                perror("recv");
            close(client_fd);
            client_fd = -1;
            continue;
        }

        value = strtol(buffer, &endptr, 10);
        while (*endptr == ' ' || *endptr == '\t')
            endptr++;

        if (endptr == buffer || (*endptr != '\n' && *endptr != '\0'))
        {
            snprintf(response, sizeof(response), "ERROR Invalid integer\n");
        }
        else
        {
            square = (long long)value * value;
            cube = square * value;
            snprintf(response, sizeof(response),
                     "OK value=%ld square=%lld cube=%lld\n",
                     value, square, cube);
        }

        if (send(client_fd, response, strlen(response), 0) < 0)
            perror("send");
        else
            printf("Client %s:%d -> %s",
                   client_ip,
                   ntohs(client_addr.sin_port),
                   response);

        close(client_fd);
        client_fd = -1;
    }

    if (client_fd >= 0)
        close(client_fd);
    close(server_fd);
    printf("Server stopped\n");

    return EXIT_SUCCESS;
}
