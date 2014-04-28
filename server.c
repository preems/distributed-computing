#include "server.h"

FILE *fp;

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
	printf("Recieved message from %s: %s\n",inet_ntoa(clientAddr.sin_addr),buf);
	return actualMessageSize;
}


void main()
{
	int fd;
	fp = logger_init("server.log");

	unsigned char buf[BUFF_SIZE];

	fd = init();
	
	recvMessage(fd,buf);
	
	LOG_INFO(fp,"Calling logger close");
	logger_close(fp);
}