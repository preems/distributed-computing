server: server.h server.c logger.h logger.c
	gcc  -g -g3 -o server -pthread server.h server.c logger.h logger.c -I.

client: client.h client.c logger.h logger.c
	gcc -g -g3 -o client -pthread client.h client.c logger.h logger.c -I.

logger: logger.h logger.c
	gcc -o logger logger.h logger.c -I.