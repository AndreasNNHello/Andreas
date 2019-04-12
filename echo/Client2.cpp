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
#include <libtcod.hpp>

#define SIZE_MAX 1024

TCODColor asd(){
    TCODColor color(100,10,200);
    return color;
}

TCODColor colNum(){
    TCODColor color(0,0,0);
    return color;
}

TCODColor colCh(){
    TCODColor color(255,0,0);
    return color;
}

void Paint(int z, char *c , int p);
void GiveMeMap(int x);

int main() {
    int clsock, newclsock;
    char buff[SIZE_MAX];
    struct sockaddr_in addr;

    clsock = socket(AF_INET, SOCK_STREAM, 0);
    if(clsock == -1){
        perror("sock not created");
    }
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(51002);

    if (newclsock = connect(clsock, (const struct sockaddr *) &addr, sizeof(addr)) == -1){
        perror("not connection");
    }

    while(1){
        GiveMeMap(clsock);
        Paint(newclsock, buff, clsock);
    }

    return 0;
}

void Paint(int z, char *c , int p){
    //while ((z = recv(p, c, SIZE_MAX, 0)) > 0) {
    if ((z = recv(p, c, SIZE_MAX, 0)) == -1){
        perror("error");
    } else {
        int w,h;
        int y = 2;
        int x = 0;
        int i = 6;
        w = (std::atoi(&c[0]) + std::atoi(&c[1]));
        h = (std::atoi(&c[3]) + std::atoi(&c[4]));
        TCODConsole::root->initRoot(w-2, h-2, "Cl2", false);
        TCODConsole::root->setDefaultBackground(asd());
        TCODConsole::root->clear();

        TCODConsole::root->flush();
        for (auto y = 2; y < h-2; y++) {
            for (auto x = 0; x < w-2; x++) {

                //for (auto i=6; i< newclsock; i++) {
                if (c[i] == '\n'){
                    i++;
                    x--;
                }else{
                    TCODConsole::root->setChar(x, y, c[i++]);
                    TCODConsole::root->setCharBackground(x, y, colCh());
                    TCODConsole::root->setCharForeground(x, y, colNum());
                    //y++;
                    //x++;
                    TCODConsole::root->flush();
                }
                //}
            }

        }


        write(1, c, z);
        //TCODConsole::root->setChar(x, y,buff[i]);
    }
}

void GiveMeMap(int x) {
    char buf1[SIZE_MAX];

    std::string mail;
    mail = "start";
    std::strcpy(buf1, mail.c_str());
    send(x, (char *) &buf1, sizeof(buf1), 0);
}