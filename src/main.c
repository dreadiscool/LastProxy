#include <stdio.h>

#include "headers/includes.h"
#include "headers/config.h"
#include "headers/network.h"

BOOL running = TRUE;

int main(int argc, char **args)
{
    printf("Last Proxy by dreadiscool starting up...\n");

    if (!config_init())
    {
        printf("[Fatal] Failed to load config!\n");
        return -1;
    }

    if (!network_init(&running))
    {
        printf("[Fatal] Failed to initialize network!\n");
        return -1;
    }

    while (running)
    {
        sleep(1);
    }
}