#ifndef GAME_PROJ_MAIN_H
#define GAME_PROJ_MAIN_H

#include <iostream>
#include <vector>
#include <libtcod.h>
#include <libtcod.hpp>
#include <ostream>

//TCOD_key_t key = TCODConsole::waitForKeypress(true);

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
/*    for (auto nc=0; nc < _width * _height; nc++) {
        _arr[nc] = new T[_width * _height];
    }
  */
    ~TwoArray(){
//      delete [] _arr;
    };


    friend std::ostream& operator<<(std::ostream& stream, std::vector<T>& Array);
    friend std::ifstream& operator>>(std::ifstream &file, std::vector<T> &Array);

    void setObjPos(const unsigned posX, const unsigned posY, T obj) {
        _arr[posX][posY] = obj;
    }

    void setboxPos(const unsigned posX, const unsigned posY, T obj) {
        Box(_arr[posX][posY], n) = obj;
    }

    char getObjPos (const unsigned posX, const unsigned posY) const{
        return _arr[posX][posY];
    }


    unsigned  getDimX() const {
        return _width;
    }

    unsigned  getDimY() const {
        return _height;
    }

private:
    //std::vector<size_t> player;
    //std::vector<size_t> box;

    T **_arr;
    int _width, _height;
};

struct Box{
public:
    Box() = default;
    Box(char o): _box(o){
       _number = new int[n];
    };

private:
    char _box;
    std::vector<int> _number;
};

#endif GAME_PROJ_MAIN_H



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

