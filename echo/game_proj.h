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

#define SIZE_MAX 1024
#define CLOCKS_PER_SEC 30000

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
void func(int x, struct sockaddr *cl_addr, int s);
std::string RandMap(const std::map<int, std::string> &x);
void *get_approp_addr(struct sockaddr *sock_a);
void CreateMap(std::string map, char *buf);
char foo(char x, char *cnum);
int BoxPos1(const std::vector<Box>& k, const std::vector<int>& m);
int BoxPos(const std::vector<Box>& k, int t);
void Moving(char *buf, int x, int y, const std::vector<int>& plus, TCOD_key_t key, std::vector<Box>& boxes, int rnd);
void MovingCl(char *buf, int x, int y, const std::vector<int>& plus, TCOD_key_t key, std::vector<Box>& boxes);
void CreateBoxAndPlus(std::vector<Box>& b, char *buf, int w, int h, std::vector<int>& plus, int* c_ptr);
void ReadAndMoving(char *b, std::vector<Box>& boxes, const std::vector<int>& plus, TCOD_key_t key, int rnd);
void ReadAndMovingCl(char *b, std::vector<Box>& boxes, const std::vector<int>& plus, TCOD_key_t key);
void Convert(char* b, char* nb, const std::vector<Box>& boxes, char *pos);
int ControlRand(char *buf, int tnp, int tnp2, int tnp3, int tnp4, int tnp5, int tnp6, int tnp7, int tnp8, int y, int rand);

void Paint(int p);
void GiveMeMap(int x);
void GiveMeNewMap(TCOD_key_t k, int x);
bool Situation(char *b, bool sit);

#endif GAME_PROJ_H



/*void PrintMap(TwoArray<char>& parr) {
        std::vector<size_t> player;
        for (auto i = 0; i < _width; i++) {
            for (auto j = 0; j < _height; j++) {
                TCODConsole::root->setChar(j, i, parr.getObjPos(i, j));
            }

            for (size_t i = 0; i < _width; i++)
                for (size_t j = 0; j < _height; j++) {
                    if (TCODConsole::root->getChar(j, i) == '@') {
                        TCODConsole::root->setCharBackground(j, i, {0, 255, 0});

                    } else if (TCODConsole::root->getChar(j, i) == '#') {
                        TCODConsole::root->setCharBackground(j, i, {255, 0, 0});
                    } else if (TCODConsole::root->getChar(j, i) == 'o') {
                        TCODConsole::root->setCharBackground(j, i, {250, 250, 0});
                        box = {j, i};
                    } else if (TCODConsole::root->getChar(j, i) == 'x') {
                        TCODConsole::root->setCharBackground(j, i, {255, 255, 255});
                    }
                }
        }
    }
    void Moving(TwoArray<char>& barr){
        char bk, bw;
        char up = TCODConsole::root->getChar(player[0], player[1] - 1);
        char down = TCODConsole::root->getChar(player[0], player[1] + 1);
        char left = TCODConsole::root->getChar(player[0] - 1, player[1]);
        char right = TCODConsole::root->getChar(player[0] + 1, player[1]);
        char upb = TCODConsole::root->getChar(player[0], player[1] - 2);
        char downb = TCODConsole::root->getChar(player[0], player[1] - 1);
        char leftb = TCODConsole::root->getChar(player[0] + 1, player[1]);
        char rightb= TCODConsole::root->getChar(player[0], player[1] - 2);
        std::vector<size_t> upp(player[0] - 1 , player[1]);
        std::vector<size_t> downp(player[0] + 1, player[1]);
        std::vector<size_t> leftp(player[0], player[1] - 1);
        std::vector<size_t> rightp(player[0], player[1] + 1);
        std::vector<size_t> upbox(player[0] - 2, player[1]);
        std::vector<size_t> downbox(player[0] + 2, player[1]);
        std::vector<size_t> leftbox(player[0], player[1] - 2);
        std::vector<size_t> rightbox(player[0], player[1] + 2);
        TCOD_key_t key = TCOD_console_wait_for_keypress(true);
        for (auto i = 0; i < _height; i++){
            for (auto j = 0; j < _width; j++) {
                bk = (TCODConsole::root->getChar(j, i) == 'o');
                bw = (TCODConsole::root->getChar(j, i) == '#');
            }
        }
        if (key.c == 24) {
            if (up != bk && up != bw) {
                std::swap(player, upp);
            } else if (player == box) {
                if (upb != bk && upb != bw)
                    std::swap(player, upp);
                std::swap(box, upbox);
            }
        }

        if (key.c == 25){
            if (down != bk && down != bw) {
                std::swap(player, downp);
            } else if (player == box) {
                if (downb != bk && downb != bw)
                    std::swap(player, downp);
                std::swap(box, downbox);
            }
        }


        if (key.c == 27){
            if (left != bk && left != bw) {
                std::swap(player, leftp);
            } else if (player == box) {
                if (leftb != bk && leftb != bw)
                    std::swap(player, leftp);
                std::swap(box, leftbox);
            }
        }

        if (key.c == 26) {
            if (right != bk && right != bw) {
                std::swap(player, rightp);
            } else if (player == box) {
                if (rightb != bk && rightb != bw)
                    std::swap(player, rightp);
                std::swap(box, rightbox);
            }
        }
        PrintMap(barr);
    };*/

