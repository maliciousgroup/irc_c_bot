#ifndef IRC_IRC_H
#define IRC_IRC_H

#include "socket.h"
#include "random.h"
#include "ssl.h"

#define IRC_MSG_MAX_LEN 512
#define IRC_HOST_MAX_LEN 256
#define IRC_CHANNEL_MAX_LEN 80
#define IRC_NICK_MAX_LEN 9
#define IRC_USER_MAX_LEN 9
#define IRC_PORT_MAX_LEN 5


/*
 *  IRC Type
 */
typedef struct {
    int sockfd;
    int buffer_ptr;
    char *nick;
    char *user;
    char *channel;
    char buffer[IRC_MSG_MAX_LEN];
} irc_t;


/*
 * Function Prototypes
 */
int irc_connect(irc_t *, const char *, const char *);
int irc_login(irc_t *, const char *);
int irc_register_user(irc_t *, const char *, const char *);
int irc_join_channel(irc_t *, const char *);
int irc_leave_channel(irc_t *, const char *);
int irc_data_handler(irc_t *);
int irc_parse_action(irc_t *);
void irc_close(irc_t *);


/*
 * IRC Helper Functions
 */
int _irc_pong(int, const char *);
int _irc_register(int, const char *, const char *);
int _irc_join(int, const char *);
int _irc_part(int, const char *);
int _irc_nick(int, const char *);
int _irc_quit(int, const char *);
//
// more to come
//



#endif //IRC_IRC_H
