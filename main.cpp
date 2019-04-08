#include "libtcod.hpp"
#include "libtcod.h"
#include "main.h"
#include <iostream>
#include <ostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
#include <bsp.hpp>
#include <map>

class MyPred
{
public:
    MyPred(char x)
            : _x(x)
    {

    }
    bool operator()(const Box& b)
    {
        return b._num == _x;
    }
private:
    char _x;
};

std::vector<int> Num;
std::vector<Box> newNum;

std::ifstream& operator>>(std::ifstream &file, TwoArray<char> &Array){
    unsigned height, width;
    int num1, num2, num3, num4, num5;
    file >> height >> width;
    TwoArray<char> mapLocal(height, width);
    Array = mapLocal;
    std::string buf;
    std::string bufNum;
    getline(file, buf);

    for (auto i = 0; i < height; i++){
        for (auto j = 0; j <= width; j++) {
            Array.setObjPos(i, j, file.get());
        }
    }
    file >> num1 >> num2 >> num3 >> num4 >> num5;
    Num = {num1, num2, num3, num4, num5};
    getline(file, bufNum);
}

TCODColor asd(){
    TCODColor color(100,10,200);
    return color;
}

TCODColor colNum(){
    TCODColor color(0,0,0);
    return color;
}

int n=10;
int turn = 0;
int Score = 1000;
int interval;
int count;
int timer;
int sttime;
int rndmoves;
int k;
char chArray[]= {'0','1','2','3','4','5','6','7','8','9',
                 'a','b','c','d','e','f','g','h','i','j',
                 'k','l','m','n','o','p','q','r','s','t',
                 'u','v','w','x','y','z','A','B','C','D',
                 'E','F','G','H','I','J','K','L','M','N',
                 'O','P','Q','R','S','T','U','V','W','X',
                 'Y','Z'};
char* pos = chArray;

bool WinPos(const std::vector<Box>& m);
int BoxPos1(const std::vector<Box>& k, const std::vector<Coord>& m);
int BoxPos(const std::vector<Box>& k, char t);
char foo(const std::vector<Box>& x, char y);
int LosePos(const std::vector<Box>& x);
void Moving(std::vector<int>& tmpPlrPos, const std::vector<TCODColor>& colourVec, const std::vector<Coord>& CharWin, int x, TCOD_key_t key);
void PaintMap(TwoArray<char>& Array, const std::vector<TCODColor>& colourVec, std::vector<int>& tmpPlrPos, std::vector<Coord>& CharWin, int x);
void PrNM(int timer);

int main() {
    TwoArray<char> arr;
    std::ifstream in("Field.txt");
    in >> arr;
    const TCODColor player{0, 255, 0};
    const TCODColor wall{255, 0, 0};
    const TCODColor box{0, 255, 255};
    const TCODColor winCross{255, 255, 255};
    const std::vector<TCODColor> colourVec = {player, wall, box, winCross};
    //std::map<std::vector<int>, char> mapCharWin;
    std::vector<Coord> CharWin;
    std::vector<int> tempPlrPos;
    std::vector<int> Pth;
    int interval = 0;
    int count = 3;
    int sttime;
    //arr.PrintMap(Array);
    //arr.Moving(arr);
    PaintMap(arr, colourVec, tempPlrPos, CharWin, timer);

    TCOD_key_t key = TCODConsole::checkForKeypress(TCOD_KEY_PRESSED);///waitForKeypress(true);

    while (!WinPos(newNum) && (newNum.at(LosePos(newNum))._num != '0') ) {
            sttime = TCODSystem::getElapsedSeconds();
            TCOD_key_t key = TCODConsole::checkForKeypress(TCOD_KEY_PRESSED);
            if (key.vk == TCODK_UP || key.vk == TCODK_DOWN || key.vk == TCODK_LEFT || key.vk == TCODK_RIGHT) {
                Moving(tempPlrPos, colourVec, CharWin, timer, key);
                timer = interval + count - sttime;
                interval = sttime;
            } else {
                timer = interval + count - sttime;
                PrNM(timer);
            }
            if (timer == 0) {
                rndmoves = TCODRandom::getInstance()->getInt(1, 4, 0);
                Moving(tempPlrPos, colourVec, CharWin, timer, key);
                interval = sttime;
            }
            rndmoves = 0;
        }

    std::string yourScore = "Name: " + std::to_string(Score);

    if(WinPos(newNum)) {
        TCODConsole::initRoot(45, 45, "You win!", false, TCOD_RENDERER_GLSL);
        TCODConsole::root->setDefaultBackground(asd());
        TCODConsole::root->clear();
        //TCODConsole::root->printEx(15,15, TCOD_BKGND_BURN, TCOD_CENTER, "Congratulation!", TCOD_COLOR_BLUE);
        TCODConsole::root->print(5, 5, yourScore.c_str());
        TCODConsole::root->flush();
    }

            if (newNum.at(LosePos(newNum))._num == '0') {
                //TCODConsole::root->setColorControl(TCOD_COLCTRL_1, TCODColor::red, TCODColor::black);
                //TCODConsole::root->print(9, 11, "You Lose!", TCOD_COLCTRL_1, TCOD_COLCTRL_STOP);
                //TCODConsole::root->isWindowClosed();
                TCODConsole::initRoot(45, 45, "You Lose!", false, TCOD_RENDERER_GLSL);
                TCODConsole::root->setDefaultBackground(asd());
                TCODConsole::root->clear();
                TCODConsole::root->print(5, 5, yourScore.c_str());
                TCODConsole::root->flush();
            }

    while(1){}
    return 0;
}

void Moving(std::vector<int>& tmpPlrPos, const std::vector<TCODColor>& colVec, const std::vector<Coord>& CharWin, int timer, TCOD_key_t key){
    std::string strtime = std::to_string(timer/60) + ":" + std::to_string(timer%60);
    TCODConsole::root->print(1, 1, strtime.c_str());
    TCODConsole::root->flush();

    Box box;

    char tnp = TCODConsole::root -> getChar(tmpPlrPos[0], tmpPlrPos[1] - 1);
    MyPred pred1(tnp);
    auto count1 = std::find_if(newNum.begin(), newNum.end(), pred1);
    char tnp2 = TCODConsole::root -> getChar(tmpPlrPos[0], tmpPlrPos[1] - 2);
    MyPred pred2(tnp2);
    auto count2 = std::find_if(newNum.begin(), newNum.end(), pred2);
    char tnp3 = TCODConsole::root -> getChar(tmpPlrPos[0], tmpPlrPos[1] + 1);
    MyPred pred3(tnp3);
    auto count3 = std::find_if(newNum.begin(), newNum.end(), pred3);
    char tnp4 = TCODConsole::root -> getChar(tmpPlrPos[0], tmpPlrPos[1] + 2);
    MyPred pred4(tnp4);
    auto count4 = std::find_if(newNum.begin(), newNum.end(), pred4);
    char tnp5 = TCODConsole::root -> getChar(tmpPlrPos[0] - 1, tmpPlrPos[1]);
    MyPred pred5(tnp5);
    auto count5 = std::find_if(newNum.begin(), newNum.end(), pred5);
    char tnp6 = TCODConsole::root -> getChar(tmpPlrPos[0] - 2, tmpPlrPos[1]);
    MyPred pred6(tnp6);
    auto count6 = std::find_if(newNum.begin(), newNum.end(), pred6);
    char tnp7 = TCODConsole::root -> getChar(tmpPlrPos[0] + 1, tmpPlrPos[1]);
    MyPred pred7(tnp7);
    auto count7 = std::find_if(newNum.begin(), newNum.end(), pred7);
    char tnp8 = TCODConsole::root -> getChar(tmpPlrPos[0] + 2, tmpPlrPos[1]);
    MyPred pred8(tnp8);
    auto count8 = std::find_if(newNum.begin(), newNum.end(), pred8);

    char chBox = foo(newNum, tnp);
    char chBox3 = foo(newNum, tnp3);
    char chBox5 = foo(newNum, tnp5);
    char chBox7 = foo(newNum, tnp7);

    if (rndmoves == 1){
        if (tnp == '#') {
            rndmoves = TCODRandom::getInstance()->getInt(2, 4, 0);
        } else if (count1 != newNum.end() && tnp2 == '#') {
            rndmoves = TCODRandom::getInstance()->getInt(2, 4, 0);
        } else if (count1 != newNum.end() && count2 != newNum.end()){
                rndmoves = TCODRandom::getInstance()->getInt(2, 4, 0);
        }
    }

    if (rndmoves == 2){
        if (tnp3 == '#') {
            rndmoves = TCODRandom::getInstance()->getInt(1, 4, 0);
        } else if (count3 != newNum.end() && tnp4 == '#') {
            rndmoves = TCODRandom::getInstance()->getInt(1, 4, 0);
        } else if (count3 != newNum.end() && count4 != newNum.end()){
            rndmoves = TCODRandom::getInstance()->getInt(1, 4, 0);
        }
    }

    if (rndmoves == 3){
        if (tnp5 == '#') {
            rndmoves = TCODRandom::getInstance()->getInt(1, 4, 0);
        } else if (count5 != newNum.end() && tnp6 == '#') {
            rndmoves = TCODRandom::getInstance()->getInt(1, 4, 0);
        } else if (count5 != newNum.end() && count6 != newNum.end()){
            rndmoves = TCODRandom::getInstance()->getInt(1, 4, 0);
        }
    }

    if (rndmoves == 4){
        if (tnp7 == '#') {
            rndmoves = TCODRandom::getInstance()->getInt(1, 3, 0);
        } else if (count7 != newNum.end() && tnp8 == '#') {
            rndmoves = TCODRandom::getInstance()->getInt(1, 3, 0);
        } else if (count7 != newNum.end() && count8 != newNum.end()){
            rndmoves = TCODRandom::getInstance()->getInt(1, 3, 0);
        }
    }

    if ( key.vk == TCODK_UP  || rndmoves == 1 ) {
        if (tnp != '#') {
            if (count1 != newNum.end()) {
                if (tnp2 == '#' || (count2 != newNum.end())) {}
                else{
                    TCODConsole::root->setChar(tmpPlrPos[0], tmpPlrPos[1] - 1, '@');
                    TCODConsole::root->setCharBackground(tmpPlrPos[0], tmpPlrPos[1] - 1, colVec[0]);
                    TCODConsole::root->setCharForeground(tmpPlrPos[0], tmpPlrPos[1] - 1, colVec[3]);
                    TCODConsole::root->setChar(tmpPlrPos[0], tmpPlrPos[1] - 2, chBox);
                    TCODConsole::root->setCharBackground(tmpPlrPos[0], tmpPlrPos[1] - 2, colVec[2]);
                    TCODConsole::root->setCharForeground(tmpPlrPos[0], tmpPlrPos[1] - 2, colNum());
                    TCODConsole::root->setChar(tmpPlrPos[0], tmpPlrPos[1], ' ');
                    TCODConsole::root->setCharBackground(tmpPlrPos[0], tmpPlrPos[1], asd());
                    TCODConsole::root->setCharForeground(tmpPlrPos[0], tmpPlrPos[1], asd());
                    tmpPlrPos = {tmpPlrPos[0], tmpPlrPos[1] - 1};
                    --newNum[BoxPos(newNum, chBox)]._box._j;
                    for (auto m : CharWin){
                    if (tmpPlrPos[0] == m._i &&  (tmpPlrPos[1] - 1) == m._j ) {
                        int p = BoxPos1(newNum, CharWin);
                        newNum[p]._win = true;
                    }
                    }
                    for (auto m : CharWin){
                        if (tmpPlrPos[0] == m._i &&  (tmpPlrPos[1]) == m._j ) {
                            int p = BoxPos(newNum, chBox);
                            newNum[p]._win = false;
                    TCODConsole::root->setChar(tmpPlrPos[0], tmpPlrPos[1] - 1, newNum[p]._num);
                    TCODConsole::root->setCharBackground(tmpPlrPos[0], tmpPlrPos[1] - 1, colVec[2]);
                    TCODConsole::root->setCharForeground(tmpPlrPos[0], tmpPlrPos[1] - 1, colNum());
                }
                    }
                }
            } else {
                TCODConsole::root -> setChar(tmpPlrPos[0], tmpPlrPos[1] - 1, '@');
                TCODConsole::root->setCharBackground(tmpPlrPos[0], tmpPlrPos[1] - 1,colVec[0]);
                TCODConsole::root->setCharForeground(tmpPlrPos[0], tmpPlrPos[1] - 1,colVec[3]);
                TCODConsole::root-> setChar(tmpPlrPos[0], tmpPlrPos[1], ' ');
                TCODConsole::root->setCharBackground(tmpPlrPos[0], tmpPlrPos[1],asd());
                TCODConsole::root->setCharForeground(tmpPlrPos[0], tmpPlrPos[1],asd());
                tmpPlrPos = {tmpPlrPos[0], tmpPlrPos[1] - 1};
            }
        }
        for (auto k : CharWin) {
            if(tmpPlrPos[0] == k._i && (tmpPlrPos[1] + 1) == k._j){
                TCODConsole::root -> setChar(tmpPlrPos[0], tmpPlrPos[1] + 1, '+');
                TCODConsole::root->setCharBackground(tmpPlrPos[0], tmpPlrPos[1] + 1,colVec[3]);
                TCODConsole::root->setCharForeground(tmpPlrPos[0], tmpPlrPos[1] + 1,colNum());
            }
        }

        TCODConsole::root->flush();
    }

    else if ( key.vk == TCODK_DOWN  || rndmoves == 2 ) {
        if (tnp3 != '#'){
            if (count3 != newNum.end()){
                if (tnp4 == '#' || (count4 != newNum.end())){}
                else {
                TCODConsole::root -> setChar(tmpPlrPos[0], tmpPlrPos[1] + 1, '@');
                TCODConsole::root->setCharBackground(tmpPlrPos[0], tmpPlrPos[1] + 1,colVec[0]);
                TCODConsole::root->setCharForeground(tmpPlrPos[0], tmpPlrPos[1] + 1,colVec[3]);
                TCODConsole::root-> setChar(tmpPlrPos[0], tmpPlrPos[1], ' ');
                TCODConsole::root->setCharBackground(tmpPlrPos[0], tmpPlrPos[1],asd());
                TCODConsole::root->setCharForeground(tmpPlrPos[0], tmpPlrPos[1],asd());
                TCODConsole::root -> setChar(tmpPlrPos[0], tmpPlrPos[1] + 2, chBox3);
                TCODConsole::root->setCharBackground(tmpPlrPos[0], tmpPlrPos[1] + 2,colVec[2]);
                TCODConsole::root->setCharForeground(tmpPlrPos[0], tmpPlrPos[1] + 2,colNum());
                tmpPlrPos = {tmpPlrPos[0], tmpPlrPos[1] + 1};
                ++newNum[BoxPos(newNum, chBox3)]._box._j;
                    for (auto m : CharWin){
                        if (  tmpPlrPos[0] == m._i &&  (tmpPlrPos[1] + 1) == m._j ) {
                            int p = BoxPos1(newNum, CharWin);
                            newNum[p]._win = true;
                        }
                    }
                    for (auto m : CharWin){
                        if (tmpPlrPos[0] == m._i &&  (tmpPlrPos[1]) == m._j ) {
                            int p = BoxPos(newNum, chBox3);
                            newNum[p]._win = false;
                            TCODConsole::root->setChar(tmpPlrPos[0], tmpPlrPos[1] + 1, newNum[p]._num);
                            TCODConsole::root->setCharBackground(tmpPlrPos[0], tmpPlrPos[1] + 1, colVec[2]);
                            TCODConsole::root->setCharForeground(tmpPlrPos[0], tmpPlrPos[1] + 1, colNum());
                        }
                    }
            }
            }
            else {
                TCODConsole::root -> setChar(tmpPlrPos[0], tmpPlrPos[1] + 1, '@');
                TCODConsole::root->setCharBackground(tmpPlrPos[0], tmpPlrPos[1] + 1,colVec[0]);
                TCODConsole::root->setCharForeground(tmpPlrPos[0], tmpPlrPos[1] + 1,colVec[3]);
                TCODConsole::root-> setChar(tmpPlrPos[0], tmpPlrPos[1], ' ');
                TCODConsole::root->setCharBackground(tmpPlrPos[0], tmpPlrPos[1],asd());
                TCODConsole::root->setCharForeground(tmpPlrPos[0], tmpPlrPos[1],asd());
                tmpPlrPos = {tmpPlrPos[0], tmpPlrPos[1] + 1};
            }
        }
        for (auto k : CharWin) {
            if(tmpPlrPos[0] == k._i && (tmpPlrPos[1] - 1) == k._j){
                TCODConsole::root -> setChar(tmpPlrPos[0], tmpPlrPos[1] - 1, '+');
                TCODConsole::root->setCharBackground(tmpPlrPos[0], tmpPlrPos[1] - 1,colVec[3]);
                TCODConsole::root->setCharForeground(tmpPlrPos[0], tmpPlrPos[1] - 1,colNum());
            }
        }
        TCODConsole::root->flush();
    }

    else if ( key.vk == TCODK_LEFT || rndmoves == 3 ) {
        if (tnp5 != '#'){
            if (count5 != newNum.end()){
                if (tnp6 == '#' || (count6 != newNum.end())){}
                else {
                    TCODConsole::root -> setChar(tmpPlrPos[0] - 1, tmpPlrPos[1], '@');
                    TCODConsole::root->setCharBackground(tmpPlrPos[0] - 1, tmpPlrPos[1],colVec[0]);
                    TCODConsole::root->setCharForeground(tmpPlrPos[0] - 1, tmpPlrPos[1],colVec[3]);
                    TCODConsole::root-> setChar(tmpPlrPos[0], tmpPlrPos[1], ' ');
                    TCODConsole::root->setCharBackground(tmpPlrPos[0], tmpPlrPos[1],asd());
                    TCODConsole::root->setCharForeground(tmpPlrPos[0], tmpPlrPos[1],asd());
                    TCODConsole::root -> setChar(tmpPlrPos[0] - 2, tmpPlrPos[1], chBox5);
                    TCODConsole::root->setCharBackground(tmpPlrPos[0] - 2, tmpPlrPos[1],colVec[2]);
                    TCODConsole::root->setCharForeground(tmpPlrPos[0] - 2, tmpPlrPos[1],colNum());
                    tmpPlrPos = {tmpPlrPos[0] - 1, tmpPlrPos[1]};
                    --newNum[BoxPos(newNum, chBox5)]._box._i;
                    for (auto m : CharWin){
                        if (  (tmpPlrPos[0] - 1) == m._i &&  tmpPlrPos[1] == m._j ) {
                            int p = BoxPos1(newNum, CharWin);
                            newNum[p]._win = true;
                        }
                    }
                    for (auto m : CharWin){
                        if (tmpPlrPos[0] == m._i &&  (tmpPlrPos[1]) == m._j ) {
                            int p = BoxPos(newNum, chBox5);
                            newNum[p]._win = false;
                            TCODConsole::root->setChar(tmpPlrPos[0] - 1, tmpPlrPos[1], newNum[p]._num);
                            TCODConsole::root->setCharBackground(tmpPlrPos[0] - 1, tmpPlrPos[1], colVec[2]);
                            TCODConsole::root->setCharForeground(tmpPlrPos[0] - 1, tmpPlrPos[1], colNum());
                        }
                    }
                }
            }
            else {
                TCODConsole::root -> setChar(tmpPlrPos[0] - 1, tmpPlrPos[1], '@');
                TCODConsole::root->setCharBackground(tmpPlrPos[0] - 1, tmpPlrPos[1],colVec[0]);
                TCODConsole::root->setCharForeground(tmpPlrPos[0] - 1, tmpPlrPos[1],colVec[3]);
                TCODConsole::root -> setChar(tmpPlrPos[0], tmpPlrPos[1], ' ');
                TCODConsole::root->setCharBackground(tmpPlrPos[0], tmpPlrPos[1],asd());
                TCODConsole::root->setCharForeground(tmpPlrPos[0], tmpPlrPos[1],asd());
                tmpPlrPos = {tmpPlrPos[0] - 1, tmpPlrPos[1]};
            }
        }
        for (auto k : CharWin) {
            if((tmpPlrPos[0] + 1) == k._i && tmpPlrPos[1] == k._j){
                TCODConsole::root -> setChar(tmpPlrPos[0] + 1, tmpPlrPos[1], '+');
                TCODConsole::root->setCharBackground(tmpPlrPos[0] + 1, tmpPlrPos[1],colVec[3]);
                TCODConsole::root->setCharForeground(tmpPlrPos[0] + 1, tmpPlrPos[1],colNum());
            }
        }
        TCODConsole::root->flush();
    }

    else if ( key.vk == TCODK_RIGHT  || rndmoves == 4) {
        if (tnp7 != '#') {
            if (count7 != newNum.end()){
                if (tnp8 == '#' || (count8 != newNum.end())){}
                else {
                TCODConsole::root->setChar(tmpPlrPos[0] + 1, tmpPlrPos[1], '@');
                TCODConsole::root->setCharBackground(tmpPlrPos[0] + 1, tmpPlrPos[1], colVec[0]);
                TCODConsole::root->setCharForeground(tmpPlrPos[0] + 1, tmpPlrPos[1], colVec[3]);
                TCODConsole::root->setChar(tmpPlrPos[0], tmpPlrPos[1], ' ');
                TCODConsole::root->setCharBackground(tmpPlrPos[0], tmpPlrPos[1], asd());
                TCODConsole::root->setCharForeground(tmpPlrPos[0], tmpPlrPos[1], asd());
                TCODConsole::root->setChar(tmpPlrPos[0] + 2, tmpPlrPos[1], chBox7);
                TCODConsole::root->setCharBackground(tmpPlrPos[0] + 2, tmpPlrPos[1], colVec[2]);
                TCODConsole::root->setCharForeground(tmpPlrPos[0] + 2, tmpPlrPos[1], colNum());
                tmpPlrPos = {tmpPlrPos[0] + 1, tmpPlrPos[1]};
                ++newNum[BoxPos(newNum, chBox7)]._box._i;
                    for (auto m : CharWin){
                        if ((tmpPlrPos[0] + 1) == m._i && tmpPlrPos[1] == m._j ) {
                            int p = BoxPos1(newNum, CharWin);
                            newNum[p]._win = true;
                        }
                    }
                    for (auto m : CharWin){
                        if (tmpPlrPos[0] == m._i &&  (tmpPlrPos[1]) == m._j ) {
                            int p = BoxPos(newNum, chBox7);
                            newNum[p]._win = false;
                            TCODConsole::root->setChar(tmpPlrPos[0] + 1, tmpPlrPos[1], newNum[p]._num);
                            TCODConsole::root->setCharBackground(tmpPlrPos[0] + 1, tmpPlrPos[1], colVec[2]);
                            TCODConsole::root->setCharForeground(tmpPlrPos[0] + 1, tmpPlrPos[1], colNum());
                        }
                    }
            }
            }
            else {
                TCODConsole::root->setChar(tmpPlrPos[0] + 1, tmpPlrPos[1], '@');
                TCODConsole::root->setCharBackground(tmpPlrPos[0] + 1, tmpPlrPos[1], colVec[0]);
                TCODConsole::root->setCharForeground(tmpPlrPos[0] + 1, tmpPlrPos[1], colVec[3]);
                TCODConsole::root->setChar(tmpPlrPos[0], tmpPlrPos[1], ' ');
                TCODConsole::root->setCharBackground(tmpPlrPos[0], tmpPlrPos[1], asd());
                TCODConsole::root->setCharForeground(tmpPlrPos[0], tmpPlrPos[1], asd());
                tmpPlrPos = {tmpPlrPos[0] + 1, tmpPlrPos[1]};
            }
        }
        for (auto k : CharWin) {
            if((tmpPlrPos[0] - 1) == k._i && tmpPlrPos[1] == k._j){
                TCODConsole::root -> setChar(tmpPlrPos[0] - 1, tmpPlrPos[1], '+');
                TCODConsole::root->setCharBackground(tmpPlrPos[0] - 1, tmpPlrPos[1],colVec[3]);
                TCODConsole::root->setCharForeground(tmpPlrPos[0] - 1, tmpPlrPos[1],colNum());
            }
        }
        TCODConsole::root->flush();
    }
    k=0;
    turn++;
    Score = 700-turn*10;

    for(auto num = newNum.begin() ; num != newNum.end(); num++){
        if (num->_win == false){
        num->_num = pos[--(Num[k++]-1)];
        }
       TCODConsole::root->setChar(num->_box._i, num->_box._j, num->_num);
    }
        TCODConsole::root->flush();
}

    char foo(const std::vector<Box>& x, char y){
    for (auto &n : x){
            if (n._num == y){
                return n._num;
            }
        }
    }

void PaintMap(TwoArray<char>& Array, const std::vector<TCODColor>& colVec, std::vector<int>& tmpPlrPos, std::vector<Coord>& CharWin, int timer){
    TCODConsole::root->initRoot(Array.getDimX(), Array.getDimY()+2, "Friendly game!", false);
    TCODSystem::setFps(25);

    k=0;
    Box box;
    Coord chWin;

    std::string strtime = std::to_string(timer/60) + ":" + std::to_string(timer%60);

    TCODConsole::root->setDefaultBackground(asd());
    TCODConsole::root->clear();

    TCODConsole::root->print(1, 1, strtime.c_str());

    for (auto i = 0; i < Array.getDimY(); i++) {
        for (auto j = 0; j < Array.getDimX(); j++) {
            auto result = Array.getObjPos(i, j);
            if (result == 'o'){
                box._num = pos[Num[k++]-1];
                box._box._i = j;
                box._box._j = i+2;
                box._win = false;
                newNum.emplace_back(box);
                result = box._num;

            }
                TCODConsole::root->setChar(j, i+2, result);
        }
    }

    for (auto i = 0; i < Array.getDimY(); i++){
        for (auto j = 0; j < Array.getDimX(); j++){

            char cnp = TCODConsole::root->getChar(j, i);
            MyPred pred(cnp);
            auto count = std::find_if(newNum.begin(), newNum.end(), pred);

            if (count != newNum.end()){
                TCODConsole::root->setCharBackground(j,i,colVec[2]);
                TCODConsole::root->setCharForeground(j,i,colNum());
            }
            if (TCODConsole::root -> getChar(j, i) == '@'){
                TCODConsole::root->setCharBackground(j,i,colVec[0]);
                tmpPlrPos = {j,i};
            }
            else if (TCODConsole::root -> getChar(j, i+2) == '#'){
                TCODConsole::root->setCharBackground(j,i+2,colVec[1]);
            }
            else if (TCODConsole::root -> getChar(j, i) == '+'){
                TCODConsole::root->setCharBackground(j,i,colVec[3]);
                TCODConsole::root->setCharForeground(j,i,colNum());
                chWin._i = j;
                chWin._j = i;
                CharWin.emplace_back(chWin);
            }
        }
    }
    TCODConsole::root->flush();
}

void PrNM(int timer) {
    if ((timer % 60) < 10) {
        std::string strtime = std::to_string(timer / 60) + ":" + "0" + std::to_string(timer % 60);
        TCODConsole::root->print(1, 1, strtime.c_str());

        TCODConsole::root->flush();
    } else {
        int minutes = timer / 60;
        int seconds = timer % 60;
        std::string strtime = std::to_string(minutes) + ":" + std::to_string(seconds);
        TCODConsole::root->print(1, 1, strtime.c_str());

        TCODConsole::root->flush();
    }
}

int LosePos(const std::vector<Box>& x) {
    for (auto i = 0; i < newNum.size(); i++) {
        if (newNum.at(i)._num == '0')
            return i;
    }
}

int BoxPos(const std::vector<Box>& k, char t) {
    for (auto i = 0; i < newNum.size(); i++) {
        if (k[i]._num == t)
            return i;
    }
}

int BoxPos1(const std::vector<Box>& k, const std::vector<Coord>& m) {
    for (auto pos = 0; pos < newNum.size(); pos++) {
        for (auto p : m){
            if(k[pos]._win != true){
        if ( k[pos]._box._i == p._i && k[pos]._box._j == p._j)
            return pos;
    }
        }
    }
}

bool WinPos(const std::vector<Box>& m){
    int sw = 0;
    for (auto &s : newNum){
        if (s._win == true) {
            sw++;
        }
    }
    if (sw == newNum.size()){
            return true;
        } else {
            return  false;
        }
}