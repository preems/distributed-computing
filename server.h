#include <stdio.h>	/* defines printf */
#include <stdlib.h>	/* defines exit and other sys calls */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "logger.h"

#define SERVER_PORT 4000
#define BUFF_SIZE 2048

#define pfi(X) printf("%s = %d\n",#X,X)