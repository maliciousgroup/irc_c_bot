#ifndef IRC_SOCKET_H
#define IRC_SOCKET_H

#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "ssl.h"


/* Function Prototypes */
int socket_build(const char *, const char *);
int socket_send(int sockfd, const char *, size_t);
int socket_sendf(int sockfd, const char *, ...);
int socket_recv(int sockfd, char *, size_t);

/* SSL Socket Wrap Prototype */
int socket_wrap_ssl(int sockfd, SSL *ssl);

#endif //IRC_SOCKET_H
