#ifndef IRC_SSL_H
#define IRC_SSL_H

#include <openssl/x509.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#include "socket.h"


/* Function Prototypes */
SSL_CTX* InitCTX(void);
void ShowCerts(SSL* ssl);

#endif //IRC_SSL_H
