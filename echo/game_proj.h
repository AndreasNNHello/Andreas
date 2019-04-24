#ifndef GAME_PROJ_H
#define GAME_PROJ_H

#include <iostream>
#include <vector>
#include <libtcod.h>
#include <libtcod.hpp>
#include <ostream>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <map>
#include <time.h>

#define SIZE_MAX 20024

struct Box{
    int _box; // coordinates
    int _num; // char on box
    bool _win;
};


/*class Time{
public:
    Time() = default;
    Time(int t)
            : _t(t)
    {

    }

    GetCurrentTime(){
        timer = interval + timer1 - time(0) + sttime;
    }

    Reset(){
        timer = timer1;
        interval = time(0) - sttime;
    }

private:
    int _t;
};

class Player: public Time{
public:
    Player() = default;
    Player(int d, int b, bool br)
            : _dog(d), _baks(b), _brake(br)
    {

    }

    Turn(){
        if (buf2[0] == '1') {
            switch (buf2[1]) {
                case '4':
                    key.vk = TCODK_UP;
                    break;
                case '7':
                    key.vk = TCODK_DOWN;
                    break;
                case '5':
                    key.vk = TCODK_LEFT;
                    break;
                case '6':
                    key.vk = TCODK_RIGHT;
                    break;
                default:
                    break;
            }

            Moving(buf, firstPlrPos, secondPlrPos, Plus, key, newNum, rndmoves);
            Time::Reset();
            key.vk = TCODK_NONE;
    }

    Random(){
        std::cout << "rand first" << std::endl;
        rndmoves = TCODRandom::getInstance()->getInt(1, 4, 0);
        Moving(buf, firstPlrPos, secondPlrPos, Plus, key, newNum, rndmoves);
        Time::Reset();
        rndmoves = 0;
    }

private:
    int _dog;
    int _baks;
    bool _brake;

};*/

TCODColor asd(){
    TCODColor color(100,10,200);
    return color;
}

TCODColor colNum(){
    TCODColor color(0,0,0);
    return color;
}

TCODColor colWinPos(){
    TCODColor color(240,240,240);
    return color;
}

TCODColor colPlayer(){
    TCODColor color(0,250,0);
    return color;
}

TCODColor colBox(){
    TCODColor color(0,0,250);
    return color;
}

TCODColor colWall(){
    TCODColor color(255,0,0);
    return color;
}

bool WinPos(const std::vector<Box>& m);
int LosePos(const std::vector<Box>& x);
std::string RandMap(const std::map<int, std::string> &x);
void *get_approp_addr(struct sockaddr *sock_a);
void CreateMap(std::string map, char *buf);
int BoxPos1(const std::vector<Box>& k, const std::vector<int>& m);
int BoxPos(const std::vector<Box>& k, int t);
void Moving(char *buf, int x, int y, const std::vector<int>& plus, TCOD_key_t key, std::vector<Box>& boxes, int rnd);
void CreateBoxAndPlus(std::vector<Box>& b, char *buf, int w, int h, std::vector<int>& plus, int* c_ptr);
void Convert(char* b, char* nb, const std::vector<Box>& boxes, int timer);
int ControlRand(char *buf, int tnp, int tnp2, int tnp3, int tnp4, int tnp5, int tnp6, int tnp7, int tnp8, int y, int rand);

void Paint(char *c);

#endif GAME_PROJ_H