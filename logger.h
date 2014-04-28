#include <stdio.h>
#include <string.h>
#include <time.h>

#define LOG_FILE "server.log"

enum logLevel { ERROR, WARNING, INFO };

#define LOG_INFO(...) logger_log(__FILE__,__LINE__,"INFO",__VA_ARGS__)
#define LOG_WARN(...) logger_log(__FILE__,__LINE__,"WARNING",__VA_ARGS__)
#define LOG_ERROR(...) logger_log(__FILE__,__LINE__,"ERROR",__VA_ARGS__)


FILE* logger_init(char* filename);
int logger_close(FILE* fp);