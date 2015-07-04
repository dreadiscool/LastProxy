#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>

#include "includes.h"

BOOL config_init(void);

uint32_t config_bind_addr(void);
uint16_t config_bind_port(void);
uint32_t config_max_clients(void);
uint32_t config_max_display_clients(void);
uint8_t config_client_thrd_count(void);

#endif
