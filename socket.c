#include <stdio.h>
#include <netdb.h>
#include <assert.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>

#include "socket.h"

extern SSL *ssl;
extern bool ssl_enabled;


/*
 *  Building a new IRC connection socket
 */
int socket_build(const char *host, const char *port) {
    int sockfd = 0;
    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = 0;
    if((getaddrinfo(host, port, &hints, &res)) < 0)
        return -1;
    if((sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) < 0) {
        freeaddrinfo(res);
        return -1;
    }
    if(connect(sockfd, res->ai_addr, res->ai_addrlen) < 0) {
        freeaddrinfo(res);
        return -1;
    }

    // SSL Enabled
    if(ssl != NULL) {
        SSL_set_fd(ssl, sockfd);
        if(SSL_connect(ssl) == -1)
            return -1;
    }

    freeaddrinfo(res);
    return sockfd;
}


/*
 *  Socket wrapper for sending data
 */
int socket_send(int sockfd, const char *data, size_t size) {
    ssize_t bytes_sent = 0;
    if (ssl != NULL) {
        if ((bytes_sent = SSL_write(ssl, data, (int) size)) <= 0)
            return -1;
    } else {
        if ((bytes_sent = send(sockfd, data, size, 0)) <= 0)
            return -1;
    }
    return (int) bytes_sent;
}

/*
 *  Socket wrapper for sending formatted data
 */
int socket_sendf(int sockfd, const char *fmt, ...) {
    int length = 0;
    char buffer[512] = {0};
    va_list args;
    if(strlen(fmt) != 0) {
        va_start(args, fmt);
        length = vsnprintf(buffer, sizeof(buffer), fmt, args);
        va_end(args);
        if(length > 512)
            length = 512;
        if(socket_send(sockfd, buffer, (size_t)length) <= 0)
            return -1;
        return length;
    }
    return 0;
}


/*
 *  Socket wrapper for receiving data from socket
 */
int socket_recv(int sockfd, char *buffer, size_t size) {
    ssize_t bytes_received = 0;
    if(ssl != NULL) {
        if ((bytes_received = SSL_read(ssl, buffer, (int)size)) <= 0)
            return -1;
    } else {
        if((bytes_received = recv(sockfd, buffer, size, 0)) <= 0)
            return -1;
    }
    return (int)bytes_received;
}

