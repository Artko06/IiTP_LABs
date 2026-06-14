#define _POSIX_C_SOURCE 200809L

#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUFFER_SIZE 256

static int read_line(int fd, char *buffer, size_t size)
{
    size_t total = 0;

    while (total + 1 < size)
    {
        char ch;
        ssize_t n = recv(fd, &ch, 1, 0);

        if (n == 0)
            break;
        if (n < 0)
            return -1;

        buffer[total++] = ch;
        if (ch == '\n')
            break;
    }

    buffer[total] = '\0';
    return (int)total;
}

int main(int argc, char *argv[])
{
    int sockfd = -1;
    struct addrinfo hints;
    struct addrinfo *result = NULL;
    struct addrinfo *rp;
    char request[BUFFER_SIZE];
    char response[BUFFER_SIZE];
    int status;

    if (argc != 4)
    {
        fprintf(stderr, "Usage: %s <host> <port> <integer>\n", argv[0]);
        return EXIT_FAILURE;
    }

    snprintf(request, sizeof(request), "%s\n", argv[3]);

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    status = getaddrinfo(argv[1], argv[2], &hints, &result);
    if (status != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return EXIT_FAILURE;
    }

    for (rp = result; rp != NULL; rp = rp->ai_next)
    {
        sockfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (sockfd < 0)
            continue;

        if (connect(sockfd, rp->ai_addr, rp->ai_addrlen) == 0)
            break;

        close(sockfd);
        sockfd = -1;
    }

    freeaddrinfo(result);

    if (sockfd < 0)
    {
        perror("connect");
        return EXIT_FAILURE;
    }

    if (send(sockfd, request, strlen(request), 0) < 0)
    {
        perror("send");
        close(sockfd);
        return EXIT_FAILURE;
    }

    status = read_line(sockfd, response, sizeof(response));
    if (status < 0)
    {
        perror("recv");
        close(sockfd);
        return EXIT_FAILURE;
    }

    printf("%s", response);

    close(sockfd);
    return EXIT_SUCCESS;
}
