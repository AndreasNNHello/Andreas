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


int main() {
    int clsock;
    struct sockaddr_in addr;
    bool situation = true;

    clsock = socket(AF_INET, SOCK_STREAM, 0);
    if (clsock == -1) {
        perror("sock not created");
    }
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(52002);

    if (connect(clsock, (const struct sockaddr *) &addr, sizeof(addr)) == -1) {
        perror("not connection");
    }

    char buf1[SIZE_MAX];

    std::string mail;
    mail = "start";
    std::strcpy(buf1, mail.c_str());
    send(clsock, (char *) &buf1, strlen(buf1), 0);

     int z;
    char c[SIZE_MAX];
    if ((z = recv(clsock, c, SIZE_MAX - 1, 0)) == -1) {
        perror("error");
    }
    while (situation) {

        //std::cout << "wh" << std::endl;
        if (c[0] == 'L') {
            TCODConsole::initRoot(45, 45, "You Lose!", false, TCOD_RENDERER_GLSL);
            TCODConsole::root->setDefaultBackground(asd());
            TCODConsole::root->clear();
            TCODConsole::root->print(5, 5, "Name");//yourScore.c_str());
            TCODConsole::root->flush();
            situation = false;
            std::cin.get();

        } else if (c[0] == 'W') {
            TCODConsole::initRoot(45, 45, "You win!", false, TCOD_RENDERER_GLSL);
            TCODConsole::root->setDefaultBackground(asd());
            TCODConsole::root->clear();
            //TCODConsole::root->printEx(15,15, TCOD_BKGND_BURN, TCOD_CENTER, "Congratulation!", TCOD_COLOR_BLUE);
            TCODConsole::root->print(5, 5, "Name");//yourScore.c_str());
            TCODConsole::root->flush();
            situation = false;
            std::cin.get();
        } else {
            //while (Situation(c, situation)) {

            //while ((z = recv(p, c, SIZE_MAX, 0)) > 0) {
            //std::string yourScore = "Name: " + std::to_string(Score);


            TCOD_key_t key = TCODConsole::checkForKeypress(TCOD_KEY_PRESSED);
            //std::cout << "key" << std::endl;
            if (key.vk == TCODK_UP || key.vk == TCODK_DOWN || key.vk == TCODK_LEFT || key.vk == TCODK_RIGHT) {
                char bufff1[SIZE_MAX];
                std::string mail;
                mail = std::to_string(key.vk);
                std::strcpy(bufff1, mail.c_str());
                send(clsock, (char *) &bufff1, strlen(bufff1), 0);
                int w, h, timer;
                int i = 6;
                h = std::atoi(&c[0]);
                w = std::atoi(&c[3]);
                timer = std::atoi(&c[strlen(c)-1]);
                TCODConsole::root->initRoot(w, h + 2, "Cl1", false);
                TCODConsole::root->setDefaultBackground(asd());
                TCODConsole::root->clear();

                std::string strtime = std::to_string(timer / 60) + ":" + std::to_string(timer % 60);
                TCODConsole::root->print(1, 1, strtime.c_str());
                //TCODConsole::root->flush();

                for (auto y = 2; y < h + 2; y++) {
                    for (auto x = 0; x < w; x++) {
                        if (c[i] == '\n') {
                            i++;
                            x--;
                        } else {
                            auto v = c[i];
                            switch (v) {
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
                                    //char chBox = foo(z, c);
                                    TCODConsole::root->setChar(x, y, v);
                                    TCODConsole::root->setCharBackground(x, y, colBox());
                                    TCODConsole::root->setCharForeground(x, y, colNum());
                                    i++;
                                    break;
                            }
                        }
                    }

                }
                TCODConsole::root->flush();
                write(1, c, z);
            } else {
                char buf3[SIZE_MAX];

                std::string mail;
                mail = "start";
                std::strcpy(buf3, mail.c_str());
                send(clsock, (char *) &buf3, strlen(buf3), 0);
                //std::cout << "paint" << std::endl;
                int w, h, timer;
                int i = 6;
                h = std::atoi(&c[0]);
                w = std::atoi(&c[3]);
                timer = std::atoi(&c[strlen(c)-1]);
                TCODConsole::root->initRoot(w, h + 2, "Cl2", false);
                TCODConsole::root->setDefaultBackground(asd());
                TCODConsole::root->clear();

                std::string strtime = std::to_string(timer / 60) + ":" + std::to_string(timer % 60);
                TCODConsole::root->print(1, 1, strtime.c_str());
                //TCODConsole::root->flush();

                for (auto y = 2; y < h + 2; y++) {
                    for (auto x = 0; x < w; x++) {
                        if (c[i] == '\n') {
                            i++;
                            x--;
                        } else {
                            auto v = c[i];
                            switch (v) {
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
                                    //char chBox = foo(z, c);
                                    TCODConsole::root->setChar(x, y, v);
                                    TCODConsole::root->setCharBackground(x, y, colBox());
                                    TCODConsole::root->setCharForeground(x, y, colNum());
                                    i++;
                                    break;
                            }
                        }
                    }

                }
                TCODConsole::root->flush();
                //std::cout << "picture" << std::endl;
                write(1, c, z);
            }
        }   //TCODConsole::root->setChar(x, y,buff[i]);
    }
    return 0;
}
//}

bool Situation(char *b,bool sit){
    if (b[0] == 'L' || b[0] == 'W')
    {
        sit = false;
    } else {
        sit = true;
    }
    return sit;
}

//void Paint(int p){


//void GiveMeMap(int x)

//void GiveMeNewMap(TCOD_key_t k, int x)

/*char foo(char x, char *cnum){
    for (auto j = 0; j < strlen(cnum); j++){
            if (x == cnum[j]){
                return cnum[j];
            }
    }
}*/