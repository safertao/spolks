#define _GNU_SOURCE
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdbool.h>

#define PORT 6969
#define MAX_CLIENTS_COUNT 3

int init (int *s)
{
    int sock;
    struct sockaddr_in address;
    if((sock = socket(AF_INET, SOCK_STREAM,0)) == -1)
    {
        perror("socket");
        return -1;
    }
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);  
    address.sin_port = htons(PORT);   
    int flag = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &flag, sizeof(flag)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    if(bind(sock, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind");
        close(sock);
        return -1;
    }
    if (listen(sock, MAX_CLIENTS_COUNT) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    *s = sock;
    return 0;
}