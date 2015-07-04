#ifndef NETWORK_H
#define NETWORK_H

#include "includes.h"

BOOL network_init(BOOL *);

static void network_thread(void *);

#endif
