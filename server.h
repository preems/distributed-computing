#include <stdio.h>	/* defines printf */
#include <stdlib.h>	/* defines exit and other sys calls */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "logger.h"
#include <sys/types.h>

#define SERVER_PORT 4000
#define BUFF_SIZE 2048
#define MAX_GROUPS 10

#define pfi(X) printf("%s = %d\n",#X,X)

//enum GroupState = {FREE,WORKNG}

typedef enum {FREE,WORKNG} GroupState;

typedef struct sockaddr_in sockAddrType;

struct ClientListNode
{
	struct ClientListNode* next;
	unsigned long ipAddress;
	unsigned int port;
};

typedef struct ClientListNode ClientListNode;


struct ClientGroup
{
	GroupState state;
	int group;
	int active;
	struct ClientListNode* listHead;
};

typedef struct ClientGroup ClientGroup;

