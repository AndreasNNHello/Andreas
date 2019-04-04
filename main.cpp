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
    Num = {num1, num2, num3, num4, num5};
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

TCODColor asd(){
    TCODColor color(100,10,200);
    return color;
}

TCODColor colNum(){
    TCODColor color(0,0,0);
    return color;
}
char pok;
int n=10;
int turn = 0;
int Score = 1000;
int interval;
int count;
int timer;
int sttime;
int rndmoves;
//int rndNum;
int k;

char foo(std::vector<Box> x);
int LosePos(std::vector<Box> x);
void Moving(std::vector<int>& tmpPlrPos, const std::vector<TCODColor>& colourVec, int x, TCOD_key_t key);
void updateWinPositions(std::map<std::vector<int>,char>& mapOfWinPositions, const std::vector<TCODColor>& colVec);
void PaintMap(TwoArray<char>& Array, const std::vector<TCODColor>& colourVec, std::vector<int>& tmpPlrPos, std::map<std::vector<int>,char>& mapCharWin, int x);
bool ifWin(std::map<std::vector<int>,char>& mapOfWinPositions);
void PrNM(int timer);
void init(Box box, int i, int j);

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
    int count = 11;
    int sttime;
    //arr.PrintMap(Array);
    //arr.Moving(arr);

    PaintMap(arr, colourVec, tempPlrPos, mapCharWin, timer);

    TCOD_key_t key = TCODConsole::checkForKeypress(TCOD_KEY_PRESSED);///waitForKeypress(true);

    while (!ifWin(mapCharWin) && (newNum.at(LosePos(newNum))._num != '0') ) {
            updateWinPositions(mapCharWin, colourVec);
            sttime = TCODSystem::getElapsedSeconds();
            TCOD_key_t key = TCODConsole::checkForKeypress(TCOD_KEY_PRESSED);
            if (key.vk == TCODK_UP || key.vk == TCODK_DOWN || key.vk == TCODK_LEFT || key.vk == TCODK_RIGHT) {
                Moving(tempPlrPos, colourVec, timer, key);
                timer = interval + count - sttime;
                interval = sttime;
            } else {
                timer = interval + count - sttime;
                PrNM(timer);
                //PaintMap(arr, colourVec, tempPlrPos, mapCharWin, timer);
            }
            if (timer == 0) {
                rndmoves = TCODRandom::getInstance()->getInt(1, 4, 0);
                Moving(tempPlrPos, colourVec, timer, key);
                interval = sttime;
            }
            rndmoves = 0;
        }

    std::string yourScore = "Name: " + std::to_string(Score);

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

    if(ifWin(mapCharWin)) {
        TCODConsole::initRoot(arr.getDimX() + 15, arr.getDimY() + 15, "You win!", false, TCOD_RENDERER_GLSL);
        TCODConsole::root->setDefaultBackground(asd());
        TCODConsole::root->clear();
        //TCODConsole::root->printEx(15,15, TCOD_BKGND_BURN, TCOD_CENTER, "Congratulation!", TCOD_COLOR_BLUE);
        TCODConsole::root->print(5, 5, yourScore.c_str());
        TCODConsole::root->flush();
    }

    while(1){}
    return 0;
}

void updateWinPositions(std::map<std::vector<int>,char>& mapOfWinPositions, const std::vector<TCODColor>& colVec){
    for (auto &cross : mapOfWinPositions) {
        if (TCODConsole::root->getChar(cross.first[0], cross.first[1]) == 'x') {
            TCODConsole::root->setCharBackground(cross.first[0], cross.first[1], colVec[3]);
            TCODConsole::root->setCharForeground(cross.first[0], cross.first[1], colNum());
            cross.second = 'x';
        } else if (TCODConsole::root->getChar(cross.first[0], cross.first[1]) == pok) {
            TCODConsole::root->setCharBackground(cross.first[0], cross.first[1], colVec[2]);
            TCODConsole::root->setCharForeground(cross.first[0], cross.first[1], colNum());
            cross.second = pok;
        } else if (TCODConsole::root->getChar(cross.first[0], cross.first[1]) == '@') {
            TCODConsole::root->setCharBackground(cross.first[0], cross.first[1], colVec[0]);
            TCODConsole::root->setCharForeground(cross.first[0], cross.first[1], colVec[3]);
            cross.second = '@';
        } else if (TCODConsole::root->getChar(cross.first[0], cross.first[1]) == ' ') {
            TCODConsole::root->setChar(cross.first[0], cross.first[1], 'x');
            TCODConsole::root->setCharBackground(cross.first[0], cross.first[1], colVec[3]);
            TCODConsole::root->setCharForeground(cross.first[0], cross.first[1], colNum());
        }
    }
}

char foo(std::vector<Box> x){
    for (auto n = 0; n<newNum.size(); n++){
        if (newNum.at(n)._num == 'c'){
            return newNum.at(n)._num;
            }
    }
}

void Moving(std::vector<int>& tmpPlrPos, const std::vector<TCODColor>& colVec, int timer, TCOD_key_t key){
    //TCOD_key_t key = TCODConsole::checkForKeypress(TCOD_KEY_PRESSED);
    std::string strtime = std::to_string(timer/60) + ":" + std::to_string(timer%60);
    TCODConsole::root->print(1, 1, strtime.c_str());
    TCODConsole::root->flush();

    char tnp = TCODConsole::root -> getChar(tmpPlrPos[0], tmpPlrPos[1] - 1);
    MyPred pred1(tnp);
    auto count1 = std::find_if_not(newNum.begin(), newNum.end(), pred1);
    char tnp2 = TCODConsole::root -> getChar(tmpPlrPos[0], tmpPlrPos[1] - 2);
    MyPred pred2(tnp2);
    auto count2 = std::find_if(newNum.begin(), newNum.end(), pred2);

    if ( key.vk == TCODK_UP  || rndmoves == 1 ) {
        if (TCODConsole::root -> getChar(tmpPlrPos[0], tmpPlrPos[1] - 1) != '#'){
            if (count2 != newNum.end()){
                TCODConsole::root -> setChar(tmpPlrPos[0], tmpPlrPos[1] - 1, '@');
                TCODConsole::root->setCharBackground(tmpPlrPos[0], tmpPlrPos[1] - 1,colVec[0]);
                TCODConsole::root->setCharForeground(tmpPlrPos[0], tmpPlrPos[1] - 1,colVec[3]);
                TCODConsole::root-> setChar(tmpPlrPos[0], tmpPlrPos[1], ' ');
                TCODConsole::root->setCharBackground(tmpPlrPos[0], tmpPlrPos[1],asd());
                TCODConsole::root->setCharForeground(tmpPlrPos[0], tmpPlrPos[1],asd());
                tmpPlrPos = {tmpPlrPos[0], tmpPlrPos[1] - 1};
            }

            else if ((TCODConsole::root -> getChar(tmpPlrPos[0], tmpPlrPos[1] - 2) != '#')
                     && count1 != newNum.end()){
                TCODConsole::root -> setChar(tmpPlrPos[0], tmpPlrPos[1] - 1, '@');
                TCODConsole::root->setCharBackground(tmpPlrPos[0], tmpPlrPos[1] - 1,colVec[0]);
                TCODConsole::root->setCharForeground(tmpPlrPos[0], tmpPlrPos[1] - 1,colVec[3]);
                TCODConsole::root -> setChar(tmpPlrPos[0], tmpPlrPos[1] - 2, foo(newNum));
                TCODConsole::root->setCharBackground(tmpPlrPos[0], tmpPlrPos[1] - 2,colVec[2]);
                TCODConsole::root->setCharForeground(tmpPlrPos[0], tmpPlrPos[1] - 2,colNum());
                TCODConsole::root-> setChar(tmpPlrPos[0], tmpPlrPos[1], ' ');
                TCODConsole::root->setCharBackground(tmpPlrPos[0], tmpPlrPos[1],asd());
                TCODConsole::root->setCharForeground(tmpPlrPos[0], tmpPlrPos[1],asd());
                tmpPlrPos = {tmpPlrPos[0], tmpPlrPos[1] - 1};
           }
        }
        TCODConsole::root->flush();
    }

    else if ( key.vk == TCODK_LEFT || rndmoves == 3 ) {
        if (TCODConsole::root -> getChar(tmpPlrPos[0] - 1, tmpPlrPos[1]) != '#'){
            if (TCODConsole::root -> getChar(tmpPlrPos[0] - 1, tmpPlrPos[1]) != pok){
                TCODConsole::root -> setChar(tmpPlrPos[0] - 1, tmpPlrPos[1], '@');
                TCODConsole::root->setCharBackground(tmpPlrPos[0] - 1, tmpPlrPos[1],colVec[0]);
                TCODConsole::root->setCharForeground(tmpPlrPos[0] - 1, tmpPlrPos[1],colVec[3]);
                TCODConsole::root -> setChar(tmpPlrPos[0], tmpPlrPos[1], ' ');
                TCODConsole::root->setCharBackground(tmpPlrPos[0], tmpPlrPos[1],asd());
                TCODConsole::root->setCharForeground(tmpPlrPos[0], tmpPlrPos[1],asd());
                tmpPlrPos = {tmpPlrPos[0] - 1, tmpPlrPos[1]};
            }
            else if ((TCODConsole::root -> getChar(tmpPlrPos[0] - 2, tmpPlrPos[1]) != '#')
                     && (TCODConsole::root -> getChar(tmpPlrPos[0] - 2, tmpPlrPos[1]) != pok)){
                TCODConsole::root -> setChar(tmpPlrPos[0] - 1, tmpPlrPos[1], '@');
                TCODConsole::root->setCharBackground(tmpPlrPos[0] - 1, tmpPlrPos[1],colVec[0]);
                TCODConsole::root->setCharForeground(tmpPlrPos[0] - 1, tmpPlrPos[1],colVec[3]);
                TCODConsole::root-> setChar(tmpPlrPos[0], tmpPlrPos[1], ' ');
                TCODConsole::root->setCharBackground(tmpPlrPos[0], tmpPlrPos[1],asd());
                TCODConsole::root->setCharForeground(tmpPlrPos[0], tmpPlrPos[1],asd());
                TCODConsole::root -> setChar(tmpPlrPos[0] - 2, tmpPlrPos[1], pok);
                TCODConsole::root->setCharBackground(tmpPlrPos[0] - 2, tmpPlrPos[1],colVec[2]);
                TCODConsole::root->setCharForeground(tmpPlrPos[0] - 2, tmpPlrPos[1],colNum());
                tmpPlrPos = {tmpPlrPos[0] - 1, tmpPlrPos[1]};
            }
        }
        TCODConsole::root->flush();
    }

    else if ( key.vk == TCODK_DOWN  || rndmoves == 2 ) {
        if (TCODConsole::root -> getChar(tmpPlrPos[0], tmpPlrPos[1] + 1) != '#'){
            if (TCODConsole::root -> getChar(tmpPlrPos[0], tmpPlrPos[1] + 1) != pok){
                TCODConsole::root -> setChar(tmpPlrPos[0], tmpPlrPos[1] + 1, '@');
                TCODConsole::root->setCharBackground(tmpPlrPos[0], tmpPlrPos[1] + 1,colVec[0]);
                TCODConsole::root->setCharForeground(tmpPlrPos[0], tmpPlrPos[1] + 1,colVec[3]);
                TCODConsole::root-> setChar(tmpPlrPos[0], tmpPlrPos[1], ' ');
                TCODConsole::root->setCharBackground(tmpPlrPos[0], tmpPlrPos[1],asd());
                TCODConsole::root->setCharForeground(tmpPlrPos[0], tmpPlrPos[1],asd());
                tmpPlrPos = {tmpPlrPos[0], tmpPlrPos[1] + 1};
            }
            else if ((TCODConsole::root -> getChar(tmpPlrPos[0], tmpPlrPos[1] + 2) != '#')
                     && (TCODConsole::root -> getChar(tmpPlrPos[0], tmpPlrPos[1] + 2) != pok)){
                TCODConsole::root -> setChar(tmpPlrPos[0], tmpPlrPos[1] + 1, '@');
                TCODConsole::root->setCharBackground(tmpPlrPos[0], tmpPlrPos[1] + 1,colVec[0]);
                TCODConsole::root->setCharForeground(tmpPlrPos[0], tmpPlrPos[1] + 1,colVec[3]);
                TCODConsole::root-> setChar(tmpPlrPos[0], tmpPlrPos[1], ' ');
                TCODConsole::root->setCharBackground(tmpPlrPos[0], tmpPlrPos[1],asd());
                TCODConsole::root->setCharForeground(tmpPlrPos[0], tmpPlrPos[1],asd());
                TCODConsole::root -> setChar(tmpPlrPos[0], tmpPlrPos[1] + 2, pok);
                TCODConsole::root->setCharBackground(tmpPlrPos[0], tmpPlrPos[1] + 2,colVec[2]);
                TCODConsole::root->setCharForeground(tmpPlrPos[0], tmpPlrPos[1] + 2,colNum());
                tmpPlrPos = {tmpPlrPos[0], tmpPlrPos[1] + 1};
            }
        }
        TCODConsole::root->flush();
    }

    else if ( key.vk == TCODK_RIGHT  || rndmoves == 4) {
        if (TCODConsole::root -> getChar(tmpPlrPos[0] + 1, tmpPlrPos[1]) != '#') {
            if (TCODConsole::root->getChar(tmpPlrPos[0] + 1, tmpPlrPos[1]) != pok) {
                TCODConsole::root->setChar(tmpPlrPos[0] + 1, tmpPlrPos[1], '@');
                TCODConsole::root->setCharBackground(tmpPlrPos[0] + 1, tmpPlrPos[1], colVec[0]);
                TCODConsole::root->setCharForeground(tmpPlrPos[0] + 1, tmpPlrPos[1], colVec[3]);
                TCODConsole::root->setChar(tmpPlrPos[0], tmpPlrPos[1], ' ');
                TCODConsole::root->setCharBackground(tmpPlrPos[0], tmpPlrPos[1], asd());
                TCODConsole::root->setCharForeground(tmpPlrPos[0], tmpPlrPos[1], asd());
                tmpPlrPos = {tmpPlrPos[0] + 1, tmpPlrPos[1]};
            } else if ((TCODConsole::root->getChar(tmpPlrPos[0] + 2, tmpPlrPos[1]) != '#')
                       && (TCODConsole::root->getChar(tmpPlrPos[0] + 2, tmpPlrPos[1]) != pok)) {
                TCODConsole::root->setChar(tmpPlrPos[0] + 1, tmpPlrPos[1], '@');
                TCODConsole::root->setCharBackground(tmpPlrPos[0] + 1, tmpPlrPos[1], colVec[0]);
                TCODConsole::root->setCharForeground(tmpPlrPos[0] + 1, tmpPlrPos[1], colVec[3]);
                TCODConsole::root->setChar(tmpPlrPos[0], tmpPlrPos[1], ' ');
                TCODConsole::root->setCharBackground(tmpPlrPos[0], tmpPlrPos[1], asd());
                TCODConsole::root->setCharForeground(tmpPlrPos[0], tmpPlrPos[1], asd());
                TCODConsole::root->setChar(tmpPlrPos[0] + 2, tmpPlrPos[1], pok);
                TCODConsole::root->setCharBackground(tmpPlrPos[0] + 2, tmpPlrPos[1], colVec[2]);
                TCODConsole::root->setCharForeground(tmpPlrPos[0] + 2, tmpPlrPos[1], colNum());
                tmpPlrPos = {tmpPlrPos[0] + 1, tmpPlrPos[1]};
            }
        }
    }
    turn++;
    Score = 1000-turn*10;
    for(auto &num : newNum)
    {
        num._num-= 1;
        TCODConsole::root->setChar(num._box._i, num._box._j, num._num);
    }
    //for (auto i = 0; i < Num.size(); i++){
      //  Num.at(i)--;
      //for (auto m = 0; m<newNum.size(); n++){
        //  newNum.at(n)._num--;
           // TCODConsole::root->setChar(newNum.at(n)._box._i, newNum.at(n)._box._j, newNum.at(n)._num);
        //}
        TCODConsole::root->flush();
   }
    /*for (auto p : Num){
        &Num.at(p)--;
    if (Num.at(p) == 0)
        TCODConsole::root->printEx(15,15, TCOD_BKGND_BURN, TCOD_CENTER, "You Lose", TCOD_COLOR_BLUE);
        TCOD_console_is_window_closed();
    }*/

bool ifWin(std::map<std::vector<int>,char>& mapOfWinPositions){
    decltype(mapOfWinPositions.size()) winCnt = 0;
    for (auto cross : mapOfWinPositions){
        if (cross.second == pok){
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

/*void init(Box box, int i, int j){
    int k = 0;
    box._num = Num.at(k++) + '0';
    box._box._i = i;
    box._box._j = j;
}*/




//bool MyPred(const Box& p) {
 //   if (p._num == &cnp){
 //       return true;
//}else
 //   return  false;
//}

void PaintMap(TwoArray<char>& Array, const std::vector<TCODColor>& colVec, std::vector<int>& tmpPlrPos, std::map<std::vector<int>,char>& mapOfWinPositions, int timer){
    TCODConsole::root->initRoot(Array.getDimX(), Array.getDimY()+2, "Friendly game!", false);
    TCODSystem::setFps(25);
    /*std::unordered_map<size_t, size_t> maps;
    (*maps).first
    (*maps).second*/
    std::string strtime = std::to_string(timer/60) + ":" + std::to_string(timer%60);
    k=0;

    TCODConsole::root->setDefaultBackground(asd());
    TCODConsole::root->clear();

    TCODConsole::root->print(1, 1, strtime.c_str());

    Box box;

    for (auto i = 0; i < Array.getDimY(); i++) {
        for (auto j = 0; j < Array.getDimX(); j++) {
            auto result = Array.getObjPos(i, j);
            if (result == 'o'){
                //rndNum = TCODRandom::getInstance()->getInt(0,4,5);
                box._num = (char)Num.at(k++);
                box._num += '0';
                box._box._i = i;
                box._box._j = j;
                //init(box, i, j);
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
            //else if (TCODConsole::root -> getChar(j, i) == box._num){
            else if (TCODConsole::root -> getChar(j, i) == 'x'){
                TCODConsole::root->setCharBackground(j,i,colVec[3]);
                mapOfWinPositions[{j,i}] = 'x';
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
        //std::string strtime = minutes + ":" + seconds;
        TCODConsole::root->print(1, 1, strtime.c_str());

        TCODConsole::root->flush();
    }
}

int LosePos(std::vector<Box> x) {
    for (auto i = 0; i < newNum.size(); i++) {
        if (newNum.at(i)._num == 0)
            return i;
    }
}
/*int LosePos(std::vector<int> x) {
    for (auto i = 0; i < Num.size(); i++) {
        if (Num.at(i) == 0)
            return i;
    }
}*/

