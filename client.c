#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>


#define KEY 0xAA

void xor_encrypt_decrypt(char *data, size_t len, char key) {
    for (size_t i = 0; i < len; i++) {
        data[i] ^= key;
    }
}

int main(int argc, char *argv[]) {
    int sockfd, n;
    struct sockaddr_in serv_addr;
    char buffer[4096];

    if (argc < 3) {
        fprintf(stderr, "Usage: %s <IP> <port>\n", argv[0]);
        return 1;
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        return 1;
    }

    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[2]));
    if (inet_pton(AF_INET, argv[1], &serv_addr.sin_addr) <= 0) {
        perror("inet_pton");
        return 1;
    }

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("connect");
        return 1;
    }

    char hello[] = "Hello from client!\n";
    xor_encrypt_decrypt(hello, strlen(hello), KEY);
    send(sockfd, hello, strlen(hello), 0);

    fd_set rset;
    for (;;) {
        FD_ZERO(&rset);
        FD_SET(sockfd, &rset);
        FD_SET(STDIN_FILENO, &rset);

        if (select(sockfd + 1, &rset, NULL, NULL, NULL) < 0)
            break;

        if (FD_ISSET(sockfd, &rset)) {
            n = recv(sockfd, buffer, sizeof(buffer), 0);
            if (n <= 0) break;
            xor_encrypt_decrypt(buffer, n, KEY);
            write(STDOUT_FILENO, buffer, n);
        }

        if (FD_ISSET(STDIN_FILENO, &rset)) {
            n = read(STDIN_FILENO, buffer, sizeof(buffer));
            if (n <= 0) break;
            xor_encrypt_decrypt(buffer, n, KEY);
            send(sockfd, buffer, n, 0);
        }
    }

    close(sockfd);
    return 0;
}
