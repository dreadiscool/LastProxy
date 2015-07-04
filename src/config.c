#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <arpa/inet.h>
#include <errno.h>

#include "headers/includes.h"
#include "headers/config.h"

static uint32_t bind_addr = INADDR_ANY;
static uint16_t bind_port = 25565;

static uint32_t max_clients = 16384;
static uint32_t max_display_clients = 1;

static uint8_t client_thrd_count = 4;

BOOL config_init(void)
{
    FILE *fp;
    char buf[2048];
    char *key, *val;

    fp = fopen("proxy.conf", "r");
    if ((fp = fopen("proxy.conf", "r")) != NULL)
    {
        while (fgets(buf, sizeof (buf), fp) != NULL)
        {
            if (buf[0] == '#' || strlen(buf) == 0)
                continue;

            key = strtok(buf, "=");
            if (key == NULL)
                continue;
            val = strtok(NULL, "=");
            if (val == NULL)
                continue;

            if (strcmp(key, "bind") == 0)
                bind_addr = inet_addr(val);
            else if (strcmp(key, "port") == 0)
                bind_port = atoi(val);
            else if (strcmp(key, "max_clients") == 0)
                max_clients = atoi(val);
            else if (strcmp(key, "max_display_clients") == 0)
                max_display_clients = atoi(val);
            else if (strcmp(key, "network_threads") == 0)
                client_thrd_count = atoi(val);
            else
                printf("Unknown key `%s`!\n", key);
        }

        printf("[Config] Loaded existing proxy.conf\n");
    }
    else if ((fp = fopen("proxy.conf", "w")) != NULL)
    {
        fprintf(fp, "# Config for LastProxy\n");

        fprintf(fp, "\n# Sets the IP address to listen on\n");
        fprintf(fp, "bind=0.0.0.0\n");

        fprintf(fp, "\n# Sets the server port to listen on\n");
        fprintf(fp, "port=%d\n", bind_port);

        fprintf(fp, "\n# Sets the maximum number of concurrent clients on this proxy\n");
        fprintf(fp, "max_clients=%d\n", max_clients);

        fprintf(fp, "\n# Max number of clients to DISPLAY (not actual max)\n");
        fprintf(fp, "max_display_clients=%d\n", max_display_clients);

        printf("[Config] Saved default config to file\n");
    }
    else
    {
        printf("[Config] Couldn't save default config! errno=%d\n", errno);
        return FALSE;
    }

    printf("[Config] Done\n");
    fclose(fp);
    return TRUE;
}

uint32_t config_bind_addr(void)
{
    return bind_addr;
}

uint16_t config_bind_port(void)
{
    return bind_port;
}

uint32_t config_max_clients(void)
{
    return max_clients;
}

uint32_t config_max_display_clients(void)
{
    return max_display_clients;
}

uint8_t config_client_thrd_count(void)
{
    return client_thrd_count;
}
