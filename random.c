#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/time.h>

#include "random.h"


char *random_string(size_t length) {
    int n = 0;
    int key = 0;
    char *string;
    struct timeval t1;

    gettimeofday(&t1, NULL);
    srand((unsigned int)t1.tv_usec * (unsigned int)t1.tv_sec);
    static char charset[] = "abcdefhijklmnopqrstuvwxyz"
                            "ABCDEFHIJKLMNOPQRSTUVWXYZ"
                            "0123456789";
    if(length) {
        string = malloc(sizeof(char) * (length + 1));
        if(string) {
            for(n = 0; n < length; n++) {
                key = rand() % (int)(sizeof(charset) -1);
                string[n] = charset[key];
            }
        }
        return string;
    }
}