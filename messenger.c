#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <signal.h>
#include <stdlib.h>

#define KEY 0xAA

void xor_encrypt_decrypt(char *data, size_t len, char key) {
    for (size_t i = 0; i < len; i++) {
        data[i] ^= key;
    }
}

char petz[200];

int main(int argc, char *argv[]) {
    struct sockaddr_in bleh;
    struct sockaddr_in blah;
    int fd, k, m, ot = 0, n = 0;
    char buffer[4096];
    fd_set rset;
    static char sc[1500];
    bzero(&bleh, sizeof(bleh));
    bleh.sin_family = AF_INET;
    bleh.sin_port = htons(atoi(argv[1]));

    fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    m = bind(fd, (struct sockaddr *)&bleh, sizeof(bleh));

    printf("Waiting connection...");
    fflush(stdout);

    if (m < 0) {
        perror("bind");
        return 1;
    }
    m = listen(fd, 5);
    if (m < 0) {
        perror("listen");
        return 1;
    }
    ot = sizeof(blah);
    k = accept(fd, (struct sockaddr *)&blah, &ot);
    if (k <= 0) {
        perror("accept");
        return 1;
    }

    printf("\nGotcha!\n");

    read(k, sc, sizeof(sc));
    printf("Received: %s", sc);

    xor_encrypt_decrypt(sc, strlen(sc), KEY);
    printf("Encrypted: %s", sc);

    for (;;) {
        FD_ZERO(&rset);
        FD_SET(k, &rset);
        FD_SET(STDIN_FILENO, &rset);

        n = select(k + 1, &rset, NULL, NULL, NULL);
        if (n <= 0)
            return (-1);

        if (FD_ISSET(k, &rset)) {
            n = recv(k, buffer, sizeof(buffer), 0);
            if (n <= 0)
                break;

            xor_encrypt_decrypt(buffer, n, KEY);
            write(STDOUT_FILENO, buffer, n);
        }

        if (FD_ISSET(STDIN_FILENO, &rset)) {
            n = read(STDIN_FILENO, buffer, sizeof(buffer));
            if (n <= 0)
                break;

            xor_encrypt_decrypt(buffer, n, KEY);
            send(k, buffer, n, 0);
        }
    }

    return 0;
}
