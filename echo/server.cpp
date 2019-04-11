#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string>
#include <iostream>
#include <ostream>
#include <fstream>
#include <vector>
#include<stdio.h>
#include <netdb.h>
#include <cstring>
#include <dirent.h>
#include "libtcod.hpp"
#include "libtcod.h"
#include <map>

std::string RandMap(const std::map<int, std::string> &x);
void *get_approp_addr(struct sockaddr *sock_a);

int main() {
    int sock, newsock1, newsock2, sz, rndmap;
    struct sockaddr_in addr, cl_addr;
    struct sockaddr *cp;
    struct hostent *rhost;
    char *host;
    char buf[1024] = {0};
    char buf1[1024] = {0};
    char buf2[1024] = {0};
    char address_pres[INET6_ADDRSTRLEN];
    std::string nameMap;
    std::map<int, std::string> maps;
    socklen_t cladrrsz = sizeof(cl_addr);
    cp = (struct sockaddr *) &cl_addr;
    sz = sizeof(addr);


    sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock == -1) {
        perror("sock not created");
        exit(1);
    }

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(51002);

    if (bind(sock, (struct sockaddr *) &addr, sz) == -1) {
        perror("no binding");
    }

    if (listen(sock, 1) == -1) {
        perror("listen");
    }

    while (1) {

        int n = -1;
        std::string it;

        DIR *dir;
        struct dirent *ent;
        if ((dir = opendir("/home/andreas/CLionProjects/echo/cmake-build-debug/Maps/")) != NULL) {
            /* print all the files and directories within directory */
            while ((ent = readdir(dir)) != NULL) {
                it = ent->d_name;
                maps.insert(std::pair<int, std::string>(n++, it));
            }
            closedir(dir);
        } else {
            /* could not open directory */
            perror("");
            return EXIT_FAILURE;
        }


        maps.erase(-1);
        maps.erase(0);
        std::string tmp2 = "Maps/" + RandMap(maps);

        std::ifstream in(tmp2);
        printf("\nHi,Iam running server.Some Client hit me\n");
        newsock1 = accept(sock, (struct sockaddr *) &cl_addr, &cladrrsz);

        if (newsock1 == -1) {
            perror("no accept Cl1");
        }

        inet_ntop(cl_addr.sin_family,
                  get_approp_addr((struct sockaddr *) &cl_addr),
                  address_pres, sizeof address_pres);
        printf("server: Client1 connection from %s\n", address_pres);

        if ((recv(newsock1, (char *) &buf2, SIZE_MAX, 0)) == -1) {
            perror("Client1 doesn't want starting play");
        } else {
            if (buf2[0] == 's') {
                std::ifstream in(tmp2);
                std::string str;
                while (!in.eof()) {
                    getline(in, str);
                    str.append("\n");
                    std::strcpy(buf, str.c_str());
                    send(newsock1, buf, strlen(buf), 0);
                }
                printf("Map message sent\n");
            }
        }


        printf("\nHi,Iam running server.Some Client hit me\n");

        newsock2 = accept(sock, (struct sockaddr *) &cl_addr, &cladrrsz);

        if (newsock2 == -1) {
            perror("no accept Cl2");
        }

        inet_ntop(cl_addr.sin_family,
                  get_approp_addr((struct sockaddr *) &cl_addr),
                  address_pres, sizeof address_pres);
        printf("server: Client2 connection from %s\n", address_pres);

        if ((recv(newsock2, (char *) &buf2, SIZE_MAX, 0)) == -1) {
            perror("Client2 doesn't want starting play");
        } else {
            if (buf2[0] == 's') {
                std::ifstream in1(tmp2);
                std::string str1;
                while (!in1.eof()) {
                    getline(in1, str1);
                    str1.append("\n");
                    std::strcpy(buf1, str1.c_str());
                    send(newsock2, buf1, strlen(buf1), 0);
                }

                printf("Map message sent\n");
            }
        }
    }
    return 0;
}
std::string RandMap(const std::map<int, std::string> &x)
{
    std::string tmp;
    int rndmap = TCODRandom::getInstance()->getInt((x.begin()->first)-1, x.size()-1, 5);
    for (auto p : x) {
        if (p.first == (rndmap+1)){
            tmp = p.second;
            return tmp;
        }
    }
}


void *get_approp_addr(struct sockaddr *sock_a)
{
    if (sock_a->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sock_a)->sin_addr);
    }
    return &(((struct sockaddr_in6*)sock_a)->sin6_addr);
}