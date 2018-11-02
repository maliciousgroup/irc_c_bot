#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>

#include "irc.h"


/*
 *  Build a socket connection to server/port (ipv4/ipv6)
 */
int irc_connect(irc_t *irc, const char *server, const char *port) {
    if((irc->sockfd = socket_build(server, port)) < 0)
        return -1;
    irc->buffer_ptr = 0;
    return 0;
} //end of irc_connection


/*
 *  Register the user with the IRC server according to RFC
 */
int irc_register_user(irc_t *irc, const char *nick, const char *user) {
    return _irc_register(irc->sockfd, nick, user);
}


/*
 *  Join an IRC channel
 */
int irc_join_channel(irc_t *irc, const char *data) {
    return _irc_join(irc->sockfd, data);
}


/*
 *  Leave a IRC channel
 */
int irc_leave_channel(irc_t *irc, const char *data) {
    return _irc_part(irc->sockfd, data);
}


/*
 *  Data Handler for incoming data
 */
int irc_data_handler(irc_t *irc) {
    assert(irc != NULL);
    int bytes = 0;
    char *line = NULL;
    char tbuffer[IRC_MSG_MAX_LEN] = {0};
    if((bytes = socket_recv(irc->sockfd, tbuffer, sizeof(tbuffer))) <= 0)
        return -1;
    tbuffer[bytes] = '\0';

    line = strtok(tbuffer, "\r\n");
    while(line != NULL) {
        strncpy(irc->buffer, line, sizeof(tbuffer));
        irc_parse_action(irc);
        line = strtok(NULL, "\r\n");
    }
    return 0;
}


/*
 *  Parse IRC Messages from the irc_data_handler()
 */
int irc_parse_action(irc_t *irc) {
    assert(irc != NULL);
    printf("[PARSE TEST]: %s\n", irc->buffer);

    //Listen for server PING, to auto-reply with PONG and payload
    if(strncmp(irc->buffer, "PING :", 6) == 0) {
        printf("[PARSE TEST PONG]\n");
        return _irc_pong(irc->sockfd, &irc->buffer[6]);
    }

    return 0;
}


/*
 *  Close IRC Socket
 */
void irc_close(irc_t *irc) {
    assert(irc != NULL);
    close(irc->sockfd);
}


/*
 *
 *  IRC Helper Functions
 *
 */


/*
 *  Function to auto-reply to PING requests
 */
int _irc_pong(int sockfd, const char *data) {
    return socket_sendf(sockfd, "PONG :%s\r\n", data);
}


/*
 *  Function to auto-register user with server
 */
int _irc_register(int sockfd, const char *nick, const char *user) {
    return socket_sendf(sockfd, "NICK %s\r\nUSER %s 0 localhost :%s\r\n", nick, user, "sizeof_char");
}


/*
 *  Function to JOIN a channel
 */
int _irc_join(int sockfd, const char *data) {
    return socket_sendf(sockfd, "JOIN %s\r\n", data);
}


/*
 *  Function to JOIN a channel
 */
int _irc_part(int sockfd, const char *data) {
    return socket_sendf(sockfd, "PART %s\r\n", data);
}