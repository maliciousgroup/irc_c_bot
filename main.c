#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <stdbool.h>

#include "irc.h"


SSL *ssl = NULL;
bool ssl_enabled = false;

int main(int argc, char *argv[]) {
    irc_t irc;
    memset(&irc, 0, sizeof(irc));

    extern int optind;
    extern char *optarg;

    int c;
    bool sflag = false;
    bool hflag = false;
    bool pflag = false;
    bool cflag = false;
    bool nflag = false;
    bool uflag = false;

    char *host = NULL;
    char *port = NULL;
    char *nick = NULL;
    char *user = NULL;
    char *channel = NULL;

    static char usage[] = "Usage: %s -h hostname -p port [-n nick] [-u user] [-c \"#chan\"] [-s ssl]\n";

    while((c = getopt(argc, argv, "h:p:sn:u:c:")) != -1) {
        switch (c) {
            case 'h':
                hflag = true;
                host = strndup(optarg, IRC_HOST_MAX_LEN);
                break;
            case 'p':
                pflag = true;
                port = strndup(optarg, IRC_PORT_MAX_LEN);
                break;
            case 'c':
                cflag = true;
                channel = strndup(optarg, IRC_CHANNEL_MAX_LEN);
                break;
            case 'n':
                nflag = true;
                nick = strndup(optarg, IRC_NICK_MAX_LEN);
                break;
            case 'u':
                uflag = true;
                user = strndup(optarg, IRC_USER_MAX_LEN);
                break;
            case 's':
                sflag = true;
                ssl_enabled = true;
                SSL_CTX *ctx = InitCTX();
                ssl = SSL_new(ctx);
                break;
        }
    }

    if((hflag == false) || (pflag == false)) {
        fprintf(stderr, usage, argv[0]);
        exit(EXIT_FAILURE);
    }
    else if (nflag == false)
        nick = random_string(8);
    else if (uflag == false)
        user = random_string(8);
    else if (cflag == false)
        channel = "#poc_bot";

    if(irc_connect(&irc, host, port) == -1) {
        fprintf(stderr, "error connecting to host\n");
        exit(EXIT_FAILURE);
    }

    if(irc_register_user(&irc, nick, user) <= 0) {
        fprintf(stderr, "error registering user\n");
        exit(EXIT_FAILURE);
    }

    if(irc_join_channel(&irc, channel) == -1) {
        fprintf(stderr, "error joining channel \"%s\"", channel);
        exit(EXIT_FAILURE);
    }

    while(irc_data_handler(&irc) >= 0);

    return 0;
}