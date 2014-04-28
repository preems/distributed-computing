#include "client.h"


FILE* fp;
int init(char* groupSuffix,char* serverIP)
{
	int fd;
	struct ip_mreq group;
	//char* multicast_prefix= "255.255.255.255";
	//multicast_prefix = MULTICAST_IP_PREFIX;
	char multicast_prefix[50];
	strcpy(multicast_prefix,MULTICAST_IP_PREFIX);

	//create socket
	if( (fd = socket(AF_INET,SOCK_DGRAM,0)) < 0 )
	{
		LOG_ERROR(fp,"Client socket creation failed");
		perror("Client Socket Creation failed\n");
		return 0;
	}
	LOG_INFO(fp,"Client Socket created");

	//Bind to INADDR_ANY
	struct sockaddr_in sa = { .sin_family=AF_INET,  .sin_addr.s_addr=htonl(INADDR_ANY) , .sin_port=htons(ZERO) };
	if (bind(fd, (struct sockaddr *)&sa, sizeof(sa)) < 0) 
	{ 
		LOG_ERROR(fp,"Client Bind failed");
		perror("client bind failed"); 
		return 0; 
	}
	LOG_INFO(fp,"Client Bind Successfull");
	

	//Set socket group
	//struct ip_mreq group = { .imr_multiaddr.s_addr=inet_addr(strcat(MULTICAST_IP_PREFIX,groupSuffix)), .imr_multiaddr.s_addr=inet_addr(serverIP) };
	

	strcat(multicast_prefix,groupSuffix);
	group.imr_multiaddr.s_addr=inet_addr(multicast_prefix);
	//group.imr_multiaddr.s_addr=inet_addr(strcat(multicast_prefix,groupSuffix));
	//group.imr_multiaddr.s_addr=inet_addr(serverIP);

	if ( setsockopt(fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&group,sizeof(group) ) <0 )
	{
		LOG_ERROR(fp,"Group assignment for client failed");
		perror("Group assignment for client failed"); 
		return 0; 
	}
	LOG_INFO(fp,"Multicast group assigned for client");

	return fd;
}


sockAddrType getsockaddr(char * ip)
{
	//sockAddrType *sa = (sockAddrType *)malloc(sizeof(sockAddrType));
	sockAddrType sa = { .sin_family=AF_INET,  .sin_addr.s_addr=inet_addr(ip) , .sin_port=htons(SERVER_PORT) };
	return sa;
}

int sendMsg(int fd,char * buf,sockAddrType sa)
{

	return (int)sendto(fd,buf,strlen(buf),0,(struct sockaddr *)&sa,sizeof(sa));

}


int main(int argc, char **argv)
{

	if(argc != 3)
	{
		printf("Usage: ./client <Server IP Address> <Multicast Group>\n");
		exit(0);
	}

	fp = logger_init("client.log");

	int fd = init(argv[2],argv[1]);

	sockAddrType sa = getsockaddr(argv[1]);

	//char* buf ="Join";
	char buf[BUFF_SIZE];
	strcpy(buf,"JOIN ");
	strcat(buf,argv[2]);

	pfi((int)strlen(buf));
	
	//int ret =sendto(fd,buf,strlen(buf),0,(struct sockaddr *)&sa,sizeof(sa));

	int ret = sendMsg(fd,buf,sa);

	//pfi(ret);

	recvMessage(fd,buf);


	close(fd);
	logger_close(fp);
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
