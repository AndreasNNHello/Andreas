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


std::ifstream& operator>>(std::ifstream &file, TwoArray<char> &Array){
    unsigned height, width;
    int num1, num2, num3, num4, num5;
    file >> height >> width;
    TwoArray<char> mapLocal(height, width);
    Array = mapLocal;
    std::string buf;
    std::string bufNum;
    getline(file, buf);
    /*for (auto va = buf.begin(); va < buf.end(); va++){
        Num.std::push_back(buf);
        std::sort(Num.begin(), Num.end());
    }*/
    for (auto i = 0; i < height; i++){
        for (auto j = 0; j <= width; j++) {
   /*         if (Array.getObjPos(i,j) == 'o')
                {Array.setboxPos(bn,hj,file.get());}
            else
                */Array.setObjPos(i, j, file.get());
        }
    }
    file >> num1 >> num2 >> num3 >> num4 >> num5;
    std::vector<int> Num = {num1, num2, num3, num4, num5};
    getline(file, bufNum);
}
/*
std::ostream& operator<<(std::ostream& stream, TwoArray<char>& Array){
    for (auto i = 1; i <= Array.getDimY(); i++) {
        for (auto j = 1; j <= Array.getDimX(); j++) {
            stream << Array.getObjPos(i, j);
        }
        std::cout << std::endl;
    }
    return stream;
}*/
int rndmoves(){
    auto rndmove = TCODRandom::getInstance()->getInt(1,4,0);
    return rndmove;
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

void Moving(std::vector<int>& tmpPlrPos, const std::vector<TCODColor>& colourVec);
void updateWinPositions(std::map<std::vector<int>,char>& mapOfWinPositions, const std::vector<TCODColor>& colVec);
void PaintMap(TwoArray<char>& Array, const std::vector<TCODColor>& colourVec, std::vector<int>& tmpPlrPos, std::map<std::vector<int>,char>& mapCharWin, int x);
bool ifWin(std::map<std::vector<int>,char>& mapOfWinPositions);
int sad(int x);

int main() {
    TwoArray<char> arr;
    std::ifstream in("Field.txt");
    in >> arr;
    const TCODColor player{0, 255, 0};
    const TCODColor wall{255, 0, 0};
    const TCODColor box{0, 255, 255};
    const TCODColor winCross{255, 255, 255};
    const std::vector<TCODColor> colourVec = {player, wall, box, winCross};
    std::map<std::vector<int>, char> mapCharWin;
    std::vector<int> tempPlrPos;
    std::vector<int> Pth;
    int interval = 0;
    int timer = 10;
    TCOD_key_t p;
    TCOD_mouse_t mouse;
    int sttime = 1;
    //arr.PrintMap(Array);
    //arr.Moving(arr);

    PaintMap(arr, colourVec, tempPlrPos, mapCharWin, timer);
    TCOD_event_t ev = TCODSystem::waitForEvent(TCOD_EVENT_ANY, &p, &mouse, false);
    //TCOD_key_t key = TCODConsole::waitForKeypress(true);

    while (!ifWin(mapCharWin)) {
        updateWinPositions(mapCharWin, colourVec);
        if (timer != 0) {
            while (ev != TCOD_EVENT_KEY_PRESS &&
                   (p.vk == TCODK_UP || p.vk == TCODK_LEFT || p.vk == TCODK_DOWN || p.vk == TCODK_RIGHT)) {
                Moving(tempPlrPos, colourVec);
            }//sttime = TCODSystem::getElapsedSeconds();
            timer = interval + timer - sttime;
            PaintMap(arr, colourVec, tempPlrPos, mapCharWin, timer);
            interval++;
            sttime++;


        } else {
            rndmoves();
            Moving(tempPlrPos, colourVec);
            timer = 10;
        }

    }



    TCODConsole::initRoot(arr.getDimX()+15,arr.getDimY()+15,"You win!", false, TCOD_RENDERER_GLSL);
    TCODConsole::root->setDefaultBackground(asd());
    TCODConsole::root->clear();
    //TCODConsole::root->printEx(15,15, TCOD_BKGND_BURN, TCOD_CENTER, "Congratulation!", TCOD_COLOR_BLUE);
    TCODConsole::root->setColorControl(TCOD_COLCTRL_1,TCODColor::red,TCODColor::black);
    TCODConsole::root->print(15, 15, "Congratulation!", TCOD_COLCTRL_1, TCOD_COLCTRL_STOP);
    TCODConsole::root->flush();

    while(1){}
    return 0;
}

void updateWinPositions(std::map<std::vector<int>,char>& mapOfWinPositions, const std::vector<TCODColor>& colVec){
    for (auto &cross : mapOfWinPositions) {
        if (TCODConsole::root->getChar(cross.first[0], cross.first[1]) == 'x') {
            TCODConsole::root->setCharBackground(cross.first[0], cross.first[1], colVec[3]);
            cross.second = 'x';
        } else if (TCODConsole::root->getChar(cross.first[0], cross.first[1]) == 'o') {
            TCODConsole::root->setCharBackground(cross.first[0], cross.first[1], colVec[2]);
            cross.second = 'o';
        } else if (TCODConsole::root->getChar(cross.first[0], cross.first[1]) == '@') {
            TCODConsole::root->setCharBackground(cross.first[0], cross.first[1], colVec[0]);
            cross.second = '@';
        } else if (TCODConsole::root->getChar(cross.first[0], cross.first[1]) == ' ') {
            TCODConsole::root->setChar(cross.first[0], cross.first[1], 'x');
            TCODConsole::root->setCharBackground(cross.first[0], cross.first[1], colVec[3]);
        }
    }
}

void Moving(std::vector<int>& tmpPlrPos, const std::vector<TCODColor>& colVec){
    TCOD_key_t key = TCODConsole::checkForKeypress(TCOD_KEY_PRESSED);
    TCODConsole::root->flush();


    if ( key.vk == TCODK_UP  || rndmoves() == 1 ) {
        if (TCODConsole::root -> getChar(tmpPlrPos[0], tmpPlrPos[1] - 1) != '#'){
            if (TCODConsole::root -> getChar(tmpPlrPos[0], tmpPlrPos[1] - 1) != 'o'){
                TCODConsole::root -> setChar(tmpPlrPos[0], tmpPlrPos[1] - 1, '@');
                TCODConsole::root->setCharBackground(tmpPlrPos[0], tmpPlrPos[1] - 1,colVec[0]);
                TCODConsole::root-> setChar(tmpPlrPos[0], tmpPlrPos[1], ' ');
                TCODConsole::root->setCharBackground(tmpPlrPos[0], tmpPlrPos[1],asd());
                tmpPlrPos = {tmpPlrPos[0], tmpPlrPos[1] - 1};
            }

            else if ((TCODConsole::root -> getChar(tmpPlrPos[0], tmpPlrPos[1] - 1) == 'o')
                     && (TCODConsole::root -> getChar(tmpPlrPos[0], tmpPlrPos[1] - 2) != '#')
                     && (TCODConsole::root -> getChar(tmpPlrPos[0], tmpPlrPos[1] - 2) != 'o')){
                TCODConsole::root -> setChar(tmpPlrPos[0], tmpPlrPos[1] - 1, '@');
                TCODConsole::root->setCharBackground(tmpPlrPos[0], tmpPlrPos[1] - 1,colVec[0]);
                TCODConsole::root -> setChar(tmpPlrPos[0], tmpPlrPos[1] - 2, 'o');
                TCODConsole::root->setCharBackground(tmpPlrPos[0], tmpPlrPos[1] - 2,colVec[2]);
                TCODConsole::root-> setChar(tmpPlrPos[0], tmpPlrPos[1], ' ');
                TCODConsole::root->setCharBackground(tmpPlrPos[0], tmpPlrPos[1],asd());
                tmpPlrPos = {tmpPlrPos[0], tmpPlrPos[1] - 1};
            }
        }
        TCODConsole::root->flush();
    }

    else if ( key.vk == TCODK_LEFT || rndmoves() == 3 ) {
        if (TCODConsole::root -> getChar(tmpPlrPos[0] - 1, tmpPlrPos[1]) != '#'){
            if (TCODConsole::root -> getChar(tmpPlrPos[0] - 1, tmpPlrPos[1]) != 'o'){
                TCODConsole::root -> setChar(tmpPlrPos[0] - 1, tmpPlrPos[1], '@');
                TCODConsole::root->setCharBackground(tmpPlrPos[0] - 1, tmpPlrPos[1],colVec[0]);
                TCODConsole::root -> setChar(tmpPlrPos[0], tmpPlrPos[1], ' ');
                TCODConsole::root->setCharBackground(tmpPlrPos[0], tmpPlrPos[1],asd());
                tmpPlrPos = {tmpPlrPos[0] - 1, tmpPlrPos[1]};
            }
            else if ((TCODConsole::root -> getChar(tmpPlrPos[0] - 1, tmpPlrPos[1]) == 'o')
                     && (TCODConsole::root -> getChar(tmpPlrPos[0] - 2, tmpPlrPos[1]) != '#')
                     && (TCODConsole::root -> getChar(tmpPlrPos[0] - 2, tmpPlrPos[1]) != 'o')){
                TCODConsole::root -> setChar(tmpPlrPos[0] - 1, tmpPlrPos[1], '@');
                TCODConsole::root->setCharBackground(tmpPlrPos[0] - 1, tmpPlrPos[1],colVec[0]);
                TCODConsole::root-> setChar(tmpPlrPos[0], tmpPlrPos[1], ' ');
                TCODConsole::root->setCharBackground(tmpPlrPos[0], tmpPlrPos[1],asd());
                TCODConsole::root -> setChar(tmpPlrPos[0] - 2, tmpPlrPos[1], 'o');
                TCODConsole::root->setCharBackground(tmpPlrPos[0] - 2, tmpPlrPos[1],colVec[2]);
                tmpPlrPos = {tmpPlrPos[0] - 1, tmpPlrPos[1]};
            }
        }
        TCODConsole::root->flush();
    }

    else if ( key.vk == TCODK_DOWN  || rndmoves() == 2 ) {
        if (TCODConsole::root -> getChar(tmpPlrPos[0], tmpPlrPos[1] + 1) != '#'){
            if (TCODConsole::root -> getChar(tmpPlrPos[0], tmpPlrPos[1] + 1) != 'o'){
                TCODConsole::root -> setChar(tmpPlrPos[0], tmpPlrPos[1] + 1, '@');
                TCODConsole::root->setCharBackground(tmpPlrPos[0], tmpPlrPos[1] + 1,colVec[0]);
                TCODConsole::root-> setChar(tmpPlrPos[0], tmpPlrPos[1], ' ');
                TCODConsole::root->setCharBackground(tmpPlrPos[0], tmpPlrPos[1],asd());
                tmpPlrPos = {tmpPlrPos[0], tmpPlrPos[1] + 1};
            }
            else if ((TCODConsole::root -> getChar(tmpPlrPos[0], tmpPlrPos[1] + 1) == 'o')
                     && (TCODConsole::root -> getChar(tmpPlrPos[0], tmpPlrPos[1] + 2) != '#')
                     && (TCODConsole::root -> getChar(tmpPlrPos[0], tmpPlrPos[1] + 2) != 'o')){
                TCODConsole::root -> setChar(tmpPlrPos[0], tmpPlrPos[1] + 1, '@');
                TCODConsole::root->setCharBackground(tmpPlrPos[0], tmpPlrPos[1] + 1,colVec[0]);
                TCODConsole::root-> setChar(tmpPlrPos[0], tmpPlrPos[1], ' ');
                TCODConsole::root->setCharBackground(tmpPlrPos[0], tmpPlrPos[1],asd());
                TCODConsole::root -> setChar(tmpPlrPos[0], tmpPlrPos[1] + 2, 'o');
                TCODConsole::root->setCharBackground(tmpPlrPos[0], tmpPlrPos[1] + 2,colVec[2]);
                tmpPlrPos = {tmpPlrPos[0], tmpPlrPos[1] + 1};
            }
        }
        TCODConsole::root->flush();
    }

    else if ( key.vk == TCODK_RIGHT  || rndmoves() == 4) {
        if (TCODConsole::root -> getChar(tmpPlrPos[0] + 1, tmpPlrPos[1]) != '#'){
            if (TCODConsole::root -> getChar(tmpPlrPos[0] + 1, tmpPlrPos[1]) != 'o'){
                TCODConsole::root -> setChar(tmpPlrPos[0] + 1, tmpPlrPos[1], '@');
                TCODConsole::root->setCharBackground(tmpPlrPos[0] + 1, tmpPlrPos[1],colVec[0]);
                TCODConsole::root -> setChar(tmpPlrPos[0], tmpPlrPos[1], ' ');
                TCODConsole::root->setCharBackground(tmpPlrPos[0], tmpPlrPos[1],asd());
                tmpPlrPos = {tmpPlrPos[0] + 1, tmpPlrPos[1]};
            }

            else if ((TCODConsole::root -> getChar(tmpPlrPos[0] + 1, tmpPlrPos[1]) == 'o')
                     && (TCODConsole::root -> getChar(tmpPlrPos[0] + 2, tmpPlrPos[1]) != '#')
                     && (TCODConsole::root -> getChar(tmpPlrPos[0] + 2, tmpPlrPos[1]) != 'o')){
                TCODConsole::root -> setChar(tmpPlrPos[0] + 1, tmpPlrPos[1], '@');
                TCODConsole::root->setCharBackground(tmpPlrPos[0] + 1, tmpPlrPos[1],colVec[0]);
                TCODConsole::root-> setChar(tmpPlrPos[0], tmpPlrPos[1], ' ');
                TCODConsole::root->setCharBackground(tmpPlrPos[0], tmpPlrPos[1], asd());
                TCODConsole::root -> setChar(tmpPlrPos[0] + 2, tmpPlrPos[1], 'o');
                TCODConsole::root->setCharBackground(tmpPlrPos[0] + 2, tmpPlrPos[1],colVec[2]);
                tmpPlrPos = {tmpPlrPos[0] + 1, tmpPlrPos[1]};
            }
        }
        TCODConsole::root->flush();
    }
    turn++;
    Score -= turn*10;

    /*for (auto p : Num){
        &Num.at(p)--;
    if (Num.at(p) == 0)
        TCODConsole::root->printEx(15,15, TCOD_BKGND_BURN, TCOD_CENTER, "You Lose", TCOD_COLOR_BLUE);
        TCOD_console_is_window_closed();
    }*/
}

bool ifWin(std::map<std::vector<int>,char>& mapOfWinPositions){
    decltype(mapOfWinPositions.size()) winCnt = 0;
    for (auto cross : mapOfWinPositions){
        if (cross.second == 'o'){
            winCnt++;
        }
    }
    if (winCnt == mapOfWinPositions.size()) {
        return true;
    }
    else{
        return false;
    }
}

void PaintMap(TwoArray<char>& Array, const std::vector<TCODColor>& colVec, std::vector<int>& tmpPlrPos, std::map<std::vector<int>,char>& mapOfWinPositions, int timer){
    TCODConsole::root->initRoot(Array.getDimX(), Array.getDimY()+2, "Friendly game!", false);
    TCODSystem::setFps(25);
    /*std::unordered_map<size_t, size_t> maps;
    (*maps).first
    (*maps).second*/
    std::string strtime = std::to_string(timer/60) + ":" + std::to_string(timer%60);

    TCODConsole::root->setDefaultBackground(asd());
    TCODConsole::root->clear();

    TCODConsole::root->print(1, 1, strtime.c_str());
    for (auto i = 0; i < Array.getDimY(); i++) {
        for (auto j = 0; j < Array.getDimX(); j++) {
            auto result = Array.getObjPos(i, j);
            TCODConsole::root->setChar(j, i+2, result);
        }
    }

    for (auto i = 0; i < Array.getDimY(); i++){
        for (auto j = 0; j < Array.getDimX(); j++){
            if (TCODConsole::root -> getChar(j, i) == '@'){
                TCODConsole::root->setCharBackground(j,i,colVec[0]);
                tmpPlrPos = {j,i};
            }
            else if (TCODConsole::root -> getChar(j, i+2) == '#'){
                TCODConsole::root->setCharBackground(j,i+2,colVec[1]);
            }
            else if (TCODConsole::root -> getChar(j, i) == 'o'){
                TCODConsole::root->setCharBackground(j,i,colVec[2]);
                //N = (TCODConsole::root->getChar(j,i) == 'o');
                //TCODConsole::root->setCharForeground(j,i,colNum());
            }
            else if (TCODConsole::root -> getChar(j, i) == 'x'){
                TCODConsole::root->setCharBackground(j,i,colVec[3]);
                mapOfWinPositions[{j,i}] = 'x';
            }
        }
    }
    TCODConsole::root->flush();
}