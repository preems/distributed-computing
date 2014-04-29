#include "server.h"

FILE *fp;

ClientGroup clients[MAX_GROUPS];

int init()
{
	int fd;

	if( (fd = socket(AF_INET,SOCK_DGRAM,0)) < 0 )
	{
		LOG_ERROR(fp,"Server socket creation failed");
		perror("Server Socket Creation failed\n");
		return 0;
	}
	LOG_INFO(fp,"Server Socket created");


	//Bind socket to INADDR_ANY
	struct sockaddr_in sa = { .sin_family=AF_INET,  .sin_addr.s_addr=htonl(INADDR_ANY) , .sin_port=htons(SERVER_PORT) };
	if (bind(fd, (struct sockaddr *)&sa, sizeof(sa)) < 0) 
	{ 
		LOG_ERROR(fp,"Server Bind failed");
		perror("bind failed"); 
		return 0; 
	}
	LOG_INFO(fp,"Server Bind Successfull");


	//Disable IP loopback
	int loop=0;
	if ( setsockopt(fd, IPPROTO_IP, IP_MULTICAST_LOOP, (char*)&loop,sizeof(loop) ) <0 )
	{
		LOG_ERROR(fp,"Disabling loopback for client failed");
		perror("Disabling loopback for client failed"); 
		return 0; 
	}
	LOG_INFO(fp,"Disabled loopback for client");

	printf ("Server initailized\n");
	return fd;

}

int recvMessage(int fd,char* buf)
{
	
	struct sockaddr_in clientAddr;
	socklen_t clientAddrLength = sizeof(clientAddr);
	int actualMessageSize;


	//printf("Waiting for a message to arrive\n");
	LOG_INFO(fp,"Waiting for a message");
	actualMessageSize = recvfrom(fd,buf,BUFF_SIZE,0,(struct sockaddr*)&clientAddr,&(clientAddrLength));
	
	//pfi(actualMessageSize);
	LOG_INFO(fp,"Message Received");
	//To be commented
	printf("Recieved message from %s:%d %s\n",inet_ntoa(clientAddr.sin_addr),ntohs(clientAddr.sin_port),buf);
	
	parseMessage(buf,&clientAddr);

	return actualMessageSize;
}


int parseMessage(char* buf,sockAddrType* clientAddr)
{
	ClientListNode *temp,*node,*head;
	char* token = strtok(buf," ");

	if( (strcmp(token,"JOIN") == 0 ) )
	{
		int group = atoi(strtok(NULL," "));
		printf("Join request recieved for group %d\n",group);
		
		head = clients[group].listHead;

		if(head==NULL)
		{
			clients[group].active=1;

			node = (ClientListNode*)malloc(sizeof(ClientListNode));
			node->next=NULL;
			node->ipAddress=clientAddr->sin_addr.s_addr;
			node->port = clientAddr->sin_port;
			clients[group].listHead=node;
		}

		else
		{

			while(head->next != NULL)
			{
				head=head->next;
			}

			temp = (ClientListNode*)malloc(sizeof(ClientListNode));
			temp->next=NULL;
			temp->ipAddress=clientAddr->sin_addr.s_addr;
			temp->port = clientAddr->sin_port;
			head->next=temp;
		}// End Insertion


	}


}

void * recvMessage_thread(void *arg)
{
	int *fd = (int*)arg;
	unsigned char buf[BUFF_SIZE];
	while(1) {
		recvMessage(*fd,buf);
		//parseMessage(buf);
	}
}


int sendMsg(int fd,char * buf,char * ip, int port)
{
	sockAddrType sa = { .sin_family=AF_INET,  .sin_addr.s_addr=inet_addr(ip) , .sin_port=htons(port) };
	return (int)sendto(fd,buf,strlen(buf),0,(struct sockaddr *)&sa,sizeof(sa));
}

void ClientGroupInit()
{
	int i;
	for(i=0;i<MAX_GROUPS;i++)
	{
		clients[i].listHead=NULL;
		clients[i].active=0;
	}

}

void printClientInfo()
{
	int i;
	ClientListNode *head;
	for (i=0;i<MAX_GROUPS;i++)
	{
		if( clients[i].listHead != NULL)
		{
			printf("Group %d\n",i);
			head = clients[i].listHead;
			while(head != NULL)
			{
				struct in_addr inaddr = {.s_addr=head->ipAddress};
				printf("Client IP:%s PORT:%d\n", inet_ntoa(inaddr),ntohs(head->port));
				head=head->next;
			}
		}
	}
}



void main()
{
	int fd,o;
	pthread_t recvThread;
	fp = logger_init("server.log");

	fd = init();
	ClientGroupInit();

	pthread_create(&recvThread,NULL,&recvMessage_thread,(void*)&fd);
	

	//sleep(60);
	//printf("Sending Multicast Msg now");
	
	//Menu
	while(1)
	{

		printf("Choose the following option\n1) Display group Info2) Assign a task3)Exit\n");
		scanf("%d",&o);

		switch(o)
		{
			case 1: printClientInfo();
					break;
			case 2: break;

			case 3: return;

			default: break;
		}
	}


	pthread_join(recvThread,NULL);
	LOG_INFO(fp,"Calling logger close");
	logger_close(fp);
	close(fd);
}