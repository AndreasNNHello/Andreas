#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <iostream>
#include <cstring>

#define SIZE_MAX 1024

int main() {
    int clsock, newclsock;
    struct sockaddr_in addr;
    char buff[SIZE_MAX];
    char buf1[SIZE_MAX];

    clsock = socket(AF_INET, SOCK_STREAM, 0);
    if(clsock == -1){
        perror("sock not created");
    }
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(51002);

    if (newclsock = connect(clsock, (const struct sockaddr *) &addr, sizeof(addr)) == -1){
        perror("not connection");
    }else {
        std::string mail;
        mail = "start";
        std::strcpy(buf1, mail.c_str());
        send(clsock, (char *) &buf1, sizeof(buf1), 0);
    }

    while ((newclsock = recv(clsock, buff, SIZE_MAX-1, 0)) > 0)
        write(1, buff, newclsock);

    return 0;
}