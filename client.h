#include <stdio.h>	/* defines printf */
#include <stdlib.h>	/* defines exit and other sys calls */
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "logger.h"

#define ZERO 0
#define SERVER_PORT 4000
#define BUFF_SIZE 2048
#define MULTICAST_IP_PREFIX "228.1.1."


#define pfi(X) printf("%s = %d\n",#X,X)

typedef struct sockaddr_in sockAddrType;


