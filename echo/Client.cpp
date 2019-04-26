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
    int w, h;
    struct sockaddr_in addr;

    clsock = socket(AF_INET, SOCK_STREAM, 0);
    if (clsock == -1) {
        perror("sock not created");
    }
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(51002);

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
    if ((z = recv(clsock, c, SIZE_MAX, 0)) == -1) {
        perror("error");
    }
    h = std::atoi(&c[0]);
    w = std::atoi(&c[3]);
    TCODConsole::root->initRoot(w, h + 2, "Cl1", false);
    TCODConsole::root->setDefaultBackground(asd());
    TCODConsole::root->clear();

    Paint(c);

    while (1) {
        if ((z = recv(clsock, c, SIZE_MAX, 0)) == -1) {
            perror("error");
        }
        if (c[0] == 'L') {
            TCODConsole::initRoot(45, 45, "You Lose!", false, TCOD_RENDERER_GLSL);
            TCODConsole::root->setDefaultBackground(asd());
            TCODConsole::root->clear();
            TCODConsole::root->print(5, 5, "Name");//yourScore.c_str());
            TCODConsole::root->flush();
            std::cin.get();
        } else if (c[0] == 'W') {
            TCODConsole::initRoot(45, 45, "You win!", false, TCOD_RENDERER_GLSL);
            TCODConsole::root->setDefaultBackground(asd());
            TCODConsole::root->clear();
            //TCODConsole::root->printEx(15,15, TCOD_BKGND_BURN, TCOD_CENTER, "Congratulation!", TCOD_COLOR_BLUE);
            TCODConsole::root->print(5, 5, "Name");//yourScore.c_str());
            TCODConsole::root->flush();
            std::cin.get();
        } else {
            if(z>0) {
                Paint(c);
            }
        }
            TCOD_key_t key = TCODConsole::checkForKeypress(TCOD_KEY_PRESSED);
            if (key.vk == TCODK_UP || key.vk == TCODK_DOWN || key.vk == TCODK_LEFT || key.vk == TCODK_RIGHT) {
                char bufff1[SIZE_MAX];
                std::string mail;
                mail = std::to_string(key.vk);
                std::strcpy(bufff1, mail.c_str());
                send(clsock, (char *) &bufff1, strlen(bufff1), 0);
            }
    }
        return 0;

}
void Paint(char *c){
    int w, h, timer, score;
    int i = 6;
    h = std::atoi(&c[0]);
    w = std::atoi(&c[3]);
    timer = std::atoi(&c[strlen(c)-5]);
    score = std::atoi(&c[strlen(c)-3]);
    std::string strtime = std::to_string(timer / 60) + ":" + std::to_string(timer % 60);
    std::string strtime1 = std::to_string(score);
    TCODConsole::root->print(1, 1, strtime.c_str());
    TCODConsole::root->print(w-5, 1, strtime1.c_str());
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
    std::cout << "painting" << timer << std::endl;
}