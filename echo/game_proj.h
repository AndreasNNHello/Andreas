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

template <typename T> class TwoArray {
public:
    TwoArray() = default;
    TwoArray(const int h, const int w)
            : _height(h), _width(w)
            {
            _arr = new T *[_height];
             for (auto i = 0; i < _height; i++)
            _arr[i] = new T[_width];
            }

    ~TwoArray(){
//      delete [] _arr;
    };

private:

    T **_arr;
    int _width, _height;
};

struct Box{
    int _box; // coordinates
    int _num; // char on box
    bool _win;
};

/*class MyPred
{
public:
    MyPred(int x)
            : _x(x)
    {

    }
    bool operator()(const Box& b)
    {
        return b._box == _x;
    }
private:
    int _x;
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
void MovingCl(char *buf, int x, int y, const std::vector<int>& plus, TCOD_key_t key, std::vector<Box>& boxes);
void CreateBoxAndPlus(std::vector<Box>& b, char *buf, int w, int h, std::vector<int>& plus, int* c_ptr);
void Convert(char* b, char* nb, const std::vector<Box>& boxes, char *pos, int timer);
int ControlRand(char *buf, int tnp, int tnp2, int tnp3, int tnp4, int tnp5, int tnp6, int tnp7, int tnp8, int y, int rand);

void Paint(char *c);

#endif GAME_PROJ_H