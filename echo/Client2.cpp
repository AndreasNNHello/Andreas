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
#include "game_proj.h"

#define SIZE_MAX 1024


void Paint(int z, char *c , int p);
void GiveMeMap(int x);
void GiveMeNewMap(TCOD_key_t k, int x);

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

    GiveMeMap(clsock);
    Paint(newclsock, buff, clsock);

    while(1){
        TCOD_key_t key = TCODConsole::checkForKeypress(TCOD_KEY_PRESSED);
        if (key.vk == TCODK_UP || key.vk == TCODK_DOWN || key.vk == TCODK_LEFT || key.vk == TCODK_RIGHT){
            GiveMeNewMap(key, clsock);
            Paint(newclsock, buff, clsock);
        }else{
            GiveMeMap(clsock);
            Paint(newclsock, buff, clsock);
        }
    }

    return 0;
}

void Paint(int z, char *c , int p){
    //while ((z = recv(p, c, SIZE_MAX, 0)) > 0) {
    if ((z = recv(p, c, SIZE_MAX, 0)) == -1){
        perror("error");
    } else {
        int w,h;
        int i = 6;
        h = std::atoi(&c[0]);
        w = std::atoi(&c[3]);
        TCODConsole::root->initRoot(w, h+2, "Cl2", false);
        TCODConsole::root->setDefaultBackground(asd());
        TCODConsole::root->clear();

        TCODConsole::root->flush();
        for (auto y = 2; y < h+2; y++) {
            for (auto x = 0; x < w; x++) {
                //for (auto i=6; i< newclsock; i++) {
                if (c[i] == '\n'){
                    i++;
                    x--;
                }else{
                    switch (c[i]){
                        case '@':
                            TCODConsole::root->setChar(x, y, c[i++]);
                            TCODConsole::root->setCharBackground(x, y, colPlayer());
                            TCODConsole::root->setCharForeground(x, y, colNum());
                            break;
                        case '$':
                            TCODConsole::root->setChar(x, y, c[i++]);
                            TCODConsole::root->setCharBackground(x, y, colPlayer());
                            TCODConsole::root->setCharForeground(x, y, colNum());
                            break;
                        case '+':
                            TCODConsole::root->setChar(x, y, c[i++]);
                            TCODConsole::root->setCharBackground(x, y, colWinPos());
                            TCODConsole::root->setCharForeground(x, y, colNum());
                            break;
                        case '#':
                            TCODConsole::root->setChar(x, y, c[i++]);
                            TCODConsole::root->setCharBackground(x, y, colWall());
                            TCODConsole::root->setCharForeground(x, y, colNum());
                            break;
                        case 'o':
                            TCODConsole::root->setChar(x, y, c[i++]);
                            TCODConsole::root->setCharBackground(x, y, colBox());
                            TCODConsole::root->setCharForeground(x, y, colNum());
                            break;
                        default:
                            TCODConsole::root->setChar(x, y, c[i++]);
                            TCODConsole::root->setCharBackground(x, y, asd());
                            TCODConsole::root->setCharForeground(x, y, asd());
                            break;
                    }

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

void GiveMeNewMap(TCOD_key_t k, int x) {
    char buf1[SIZE_MAX];
    std::string mail;
    mail = std::to_string(k.vk);
    std::strcpy(buf1, mail.c_str());
    send(x, (char *) &buf1, sizeof(buf1), 0);
}