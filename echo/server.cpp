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

int main()
{
    int sock, newsock, sz, rndmap;
    struct sockaddr_in addr, cl_addr;
    struct sockaddr *cp;
    struct hostent *rhost;
    char *host;
    char buf[1024] = {0};
    std::string nameMap;
    std::map<int, std::string> maps;

    cp=(struct sockaddr *)&cl_addr;
    sz = sizeof(addr);

    sock = socket(AF_INET, SOCK_STREAM , 0);

    if (sock == -1){
        perror("sock not created");
        exit(1);
    }

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(51002);

    if (bind(sock, (struct sockaddr *)&addr, sz) == -1){
        perror("no binding");
        exit(1);
    }

    if (listen(sock, 2) == -1){
        perror("listen");
        exit(1);
    }

    while(1) {
        printf("\nHi,Iam running server.Some Client hit me\n");
        if((newsock = accept(sock, NULL, NULL)) == -1){
            perror("no accept");
            exit(1);
        }

        /*rhost=gethostbyaddr((char*)(&cl_addr.sin_addr),
                            sizeof(cl_addr.sin_addr), AF_INET);
        if(h_errno){
            printf("gethostbyaddr error: %d\n", h_errno);
            host=inet_ntoa(cl_addr.sin_addr);
        } else {
            host=rhost->h_name;
        }

        printf("%s request from %s:%d\n", host,
               htons(cl_addr.sin_port));
        */
        int n=-1;

        std::string it;

        DIR *dir;
        struct dirent *ent;
        if ((dir = opendir ("/home/andreas/CLionProjects/echo/cmake-build-debug/Maps/")) != NULL) {
            /* print all the files and directories within directory */
            while ((ent = readdir (dir)) != NULL) {
                it = ent->d_name;
                maps.insert(std::pair<int, std::string> (n++, it));

                printf ("%s\n", ent->d_name);
            }
            closedir (dir);
        } else {
            /* could not open directory */
            perror ("");
            return EXIT_FAILURE;
        }


        maps.erase(-1);
        maps.erase(0);

        //std::ifstream in(RandMap(maps));
        std::ifstream in("/home/andreas/CLionProjects/echo/cmake-build-debug/Maps/Field.txt");
        std::string str;
        while(!in.eof())
        {
            getline(in, str);
            str.append("\n");
            std::strcpy (buf, str.c_str());
            send(newsock, buf, strlen(buf) , 0 );
            //delete cstr;
            //printf(cstr);
        }

        printf("Map message sent\n");

        close(newsock);
    }

    return 0;
}

std::string RandMap(const std::map<int, std::string> &x)
{
    std::string tmp;
    int rndmap = TCODRandom::getInstance()->getInt((x.begin()->first)-1, x.size()-1, 0);
    for (auto p : x) {
        if (p.first == rndmap+1)
            tmp = p.second;
            return tmp;
    }
}