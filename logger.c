#include "logger.h"

FILE* logger_init(char* filename)
{
	FILE* fp;

	fp = fopen(filename,"a");

  	if(fp == NULL)
  		perror("Logger init failed\n");
  	else
  	{
  		setlinebuf(fp);
  		return fp;
  	}
}

int logger_close(FILE* fp)
{
	fclose(fp);
	return 1;
}


int logger_log(char*filename,int line,char* level,FILE* fp,char* logmessage,...)
{
	time_t t;
	time(&t);
	int ret;
	char * time = ctime(&t);
	time[strlen(time)-6]=0;
	ret =fprintf(fp,"%s %-7s %s %d %s\n",time,level,filename,line,logmessage);
	if(ret < 1) 
	{
		perror("Could not write to lof file");
	}
	return ret;
}

/*void main()
{
	FILE *fp = logger_init("server.log");
	//logger_log(fp,INFO, "This is a log message");
	LOG_INFO(fp,"This is a info");
	LOG_WARN(fp,"This is a warning");
	LOG_ERROR(fp,"This is a error");

	logger_close(fp);
}*/