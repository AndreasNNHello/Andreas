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

int main() {
    int clsock, newclsock;
    char buff[SIZE_MAX];
    char buff1[SIZE_MAX];
    struct sockaddr_in addr;
    bool situation = true;

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
    Paint(newclsock, newclsock, buff, buff1, clsock);

    while(situation){
        TCOD_key_t key = TCODConsole::checkForKeypress(TCOD_KEY_PRESSED);
        if (key.vk == TCODK_UP || key.vk == TCODK_DOWN || key.vk == TCODK_LEFT || key.vk == TCODK_RIGHT){
            GiveMeNewMap(key, clsock);
            Paint(newclsock, newclsock, buff, buff1, clsock);
            Situation(buff, situation);
        }else{
            GiveMeMap(clsock);
            Paint(newclsock, newclsock, buff, buff1, clsock);
            Situation(buff, situation);
        }
    }

    return 0;
}

bool Situation(char *b,bool sit){
    if (b[0] == 'L' || b[0] == 'W')
        sit = false;
    return sit;
}

void Paint(int z, int z1, char *c, char *c1, int p){
    //while ((z = recv(p, c, SIZE_MAX, 0)) > 0) {
    //std::string yourScore = "Name: " + std::to_string(Score);
    if (((z = recv(p, c, SIZE_MAX-1, 0)) == -1) && ((z1 = recv(p, c1, SIZE_MAX-1, 0)) == -1)) {
        perror("error");
    } else if(c[0] == 'L'){
        TCODConsole::initRoot(45, 45, "You Lose!", false, TCOD_RENDERER_GLSL);
        TCODConsole::root->setDefaultBackground(asd());
        TCODConsole::root->clear();
        TCODConsole::root->print(5, 5, "Name");//yourScore.c_str());
        TCODConsole::root->flush();
    } else if(c[0] == 'W'){
        TCODConsole::initRoot(45, 45, "You win!", false, TCOD_RENDERER_GLSL);
        TCODConsole::root->setDefaultBackground(asd());
        TCODConsole::root->clear();
        //TCODConsole::root->printEx(15,15, TCOD_BKGND_BURN, TCOD_CENTER, "Congratulation!", TCOD_COLOR_BLUE);
        TCODConsole::root->print(5, 5, "Name");//yourScore.c_str());
        TCODConsole::root->flush();
    } else {
        int w,h;
        int i = 6, k = 0;
        h = std::atoi(&c[0]);
        w = std::atoi(&c[3]);
        TCODConsole::root->initRoot(w, h+2, "Cl1", false);
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
                    auto x = c[i];
                    switch (x){
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
                        case ' ':
                            TCODConsole::root->setChar(x, y, c[i++]);
                            TCODConsole::root->setCharBackground(x, y, asd());
                            TCODConsole::root->setCharForeground(x, y, asd());
                            break;
                        default:
                            char chBox = foo(x, c1);
                            TCODConsole::root->setChar(x, y, chBox);
                            TCODConsole::root->setCharBackground(x, y, colBox());
                            TCODConsole::root->setCharForeground(x, y, colNum());
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

char foo(char x, char *cnum){
    for (auto j = 0; j < sizeof(cnum); j++){
            if (x == cnum[j]){
                return cnum[j];
            }
    }
}