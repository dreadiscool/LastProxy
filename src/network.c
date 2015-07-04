#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <arpa/inet.h>

#include "headers/includes.h"
#include "headers/network.h"
#include "headers/config.h"

static int serv_fd, epoll_fd;
struct sockaddr_in serv_addr;

static BOOL *running;
static pthread_t *client_thrds;

BOOL network_init(BOOL *_running)
{
    int i, opt = 1, flags;
    struct epoll_event event;

    running = _running;

    if ((serv_fd = socket(AF_INET, SOCK_STREAM, 0)) <= 0)
    {
        printf("[Network] Failed to open socket!\n");
        return FALSE;
    }
    setsockopt(serv_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof (int));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = config_bind_addr();
    serv_addr.sin_port = htons(config_bind_port());

    if (bind(serv_fd, (struct sockaddr *)&serv_addr, sizeof (struct sockaddr_in)) == -1)
    {
        close(serv_fd);
        printf("[Network] Failed to bind to address!\n");
        return FALSE;
    }

    fcntl(serv_fd, F_SETFL, fcntl(serv_fd, F_GETFL, 0) | O_NONBLOCK);

    if (listen(serv_fd, 128) == -1)
    {
        close(serv_fd);
        printf("[Network] Failed to listen!\n");
        return FALSE;
    }

    if ((epoll_fd = epoll_create1(0)) == -1)
    {
        close(serv_fd);
        printf("[Network] Failed to initialize epoll! Does your system support it?\n");
        return FALSE;
    }

    event.data.fd = serv_fd;
    event.events = EPOLLIN | EPOLLET;
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, serv_fd, &event);

    client_thrds = (pthread_t *)calloc(config_client_thrd_count(), sizeof (pthread_t));
    for (i = 0; i < config_client_thrd_count(); i++)
    {
        int *ptr;

        ptr = (int *)malloc(sizeof (int));
        *ptr = i + 1;
        pthread_create(&client_thrds[i], NULL, (void *)&network_thread, (void *)ptr);
    }

    return TRUE;
}

static void network_thread(void *arg)
{
    int *tid, events;
    struct epoll_event event;

    tid = (int *)arg;
    printf("Network thread #%d started!\n", *tid);

    while (running)
    {
        events = epoll_wait(epoll_fd, &event, 1, 1);
        if (!(*running))
            break;
        if (events == 0)
            continue;

        if (event.events & EPOLLIN)
        {
            printf("Incoming connection on fd %d!\n", event.data.fd);
        }
    }

    printf("Network thread #%d finished!\n", *tid);
    free(tid);
}
