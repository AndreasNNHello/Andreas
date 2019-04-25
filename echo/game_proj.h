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


class Time{
public:
    Time() = default;
    Time(int t)
            : _t(t)
    {

    }

    int GetCurrentTime(int, int, int);
    void Reset(int, int, int, int);

    //~Time();
private:
    int _t;
};

class Player: public Time{
public:
    Player() = default;
    Player(char f, char s, bool br)
            : _first(f), _second(s), _brake(br)
    {

    }

    void Turn(char *, char *, int, int, const std::vector<int>&, std::vector<Box>&, int, int, int, int, int, int, Time);
    void RandomTurn(char *, int, int, const std::vector<int>& , std::vector<Box>& , int , int, int, int, int, int, Time);
    void WhoIsWho(char , char , char *, int, int);
    void Send(char *, char *, const std::vector<Box>&, int, int, int);
    static void Lose(char *, const std::vector<Box>&, Player, int, int);
    static void Win(char *, const std::vector<Box>&, Player, int, int);
    void setBrake(bool f) {
        _brake = f;
    }

    //~Player();
private:
    char _first;
    char _second;
    bool _brake;

};

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
int CreateMap(std::map<int, std::string> maps, char *buf);
int BoxPos1(const std::vector<Box>& k, const std::vector<int>& m);
int BoxPos(const std::vector<Box>& k, int t);
void Moving(char *buf, int fp, int sp, const std::vector<int>& plus, TCOD_key_t key, std::vector<Box>& boxes, int rnd, int w, int h);
void CreateBoxAndPlus(std::vector<Box>& b, char *buf, int w, int h, std::vector<int>& plus, int* c_ptr);
void Convert(char* b, char* nb, const std::vector<Box>& boxes, int timer);
int ControlRand(char *buf, int tnp, int tnp2, int tnp3, int tnp4, int tnp5, int tnp6, int tnp7, int tnp8, int y, int rand);

void Paint(char *c);

#endif GAME_PROJ_H