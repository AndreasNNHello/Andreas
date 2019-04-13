#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string>
#include <iostream>
#include <ostream>
#include <fstream>
#include <vector>
#include<stdio.h>
#include <netdb.h>
#include <cstring>
#include <dirent.h>
#include "libtcod.hpp"
#include "libtcod.h"
#include "game_proj.h"
#include <map>
#include <algorithm>

void func(int x, struct sockaddr *cl_addr, std::string map);
std::string RandMap(const std::map<int, std::string> &x);
void *get_approp_addr(struct sockaddr *sock_a);
void SendMap(std::string map, int sock, char *buf);
//char foo(const std::vector<Box>& x, char y);
//int BoxPos1(const std::vector<Box>& k, const std::vector<Coord>& m);
//int BoxPos(const std::vector<Box>& k, char t);
void Moving(char *buf, int x, const std::vector<TCODColor>& colVec, const std::vector<Coord>& CharWin, int timer, TCOD_key_t key);

int main() {
    int sock, newsock1, newsock2, sz, rndmap;
    struct sockaddr_in addr, cl_addr;
    struct sockaddr *cp;
    struct hostent *rhost;
    char *host;


    char buf2[1024] = {0};
    char address_pres[INET6_ADDRSTRLEN];
    std::string nameMap;
    std::map<int, std::string> maps;
    socklen_t cladrrsz = sizeof(cl_addr);
    cp = (struct sockaddr *) &cl_addr;
    sz = sizeof(addr);


    const TCODColor player{0, 255, 0};
    const TCODColor wall{255, 0, 0};
    const TCODColor box{0, 255, 255};
    const TCODColor winCross{255, 255, 255};
    const std::vector<TCODColor> colourVec = {player, wall, box, winCross};
    std::vector<Coord> CharWin;


    sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock == -1) {
        perror("sock not created");
        exit(1);
    }

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(51002);

    if (bind(sock, (struct sockaddr *) &addr, sz) == -1) {
        perror("no binding");
    }

    if (listen(sock, 1) == -1) {
        perror("listen");
    }



        int n = -1;
        std::string it;

        DIR *dir;
        struct dirent *ent;
        if ((dir = opendir("/home/andreas/CLionProjects/echo/cmake-build-debug/Maps/")) != NULL) {
            /* print all the files and directories within directory */
            while ((ent = readdir(dir)) != NULL) {
                it = ent->d_name;
                maps.insert(std::pair<int, std::string>(n++, it));
            }
            closedir(dir);
        } else {
            /* could not open directory */
            perror("");
            return EXIT_FAILURE;
        }


        maps.erase(-1);
        maps.erase(0);
        std::string tmp2 = "Maps/" + RandMap(maps);


    newsock1 = accept(sock, (struct sockaddr *) &cl_addr, &cladrrsz);
    newsock2 = accept(sock, (struct sockaddr *) &cl_addr, &cladrrsz);

   /*printf("\nHi,Iam running server.Some Client hit me\n");

   newsock2 = accept(sock, (struct sockaddr *) &cl_addr, &cladrrsz);

   if (newsock2 == -1) {
       perror("no accept Cl2");
   }

   inet_ntop(cl_addr.sin_family,
             get_approp_addr((struct sockaddr *) &cl_addr),
             address_pres, sizeof address_pres);
   printf("server: Client2 connection from %s\n", address_pres);

   if ((recv(newsock2, (char *) &buf2, SIZE_MAX, 0)) == -1) {
       perror("Client2 doesn't want starting play");
   } else {
       if (buf2[0] == 's') {
           SendMap(tmp2, newsock2, buf);
           printf("Map message sent\n");
       }
   }*/
    printf("\nHi,Iam running server.Some Client hit me\n");
    func(newsock1, (sockaddr *) &cl_addr, tmp2);
    printf("\nHi,Iam running server.Some Client hit me\n");
    func(newsock2, (sockaddr *) &cl_addr, tmp2);

    while (1) {

    }
    return 0;
}

void func(int x, struct sockaddr *cl_addr, std::string map){
    char buf[1024] = {0};
    char buf1[1024] = {0};
    std::ifstream in(map);
    int firstPlrPos, secondPlrPos;

    //printf("\nHi,Iam running server.Some Client hit me\n");


    if (x == -1) {
        perror("no accept Cl1");
    }

    inet_ntop(cl_addr->sa_family,
              get_approp_addr((struct sockaddr *) &cl_addr),
              address_pres, sizeof address_pres);
    printf("server: Client1 connection from %s\n", address_pres);

    if ((recv(x, (char *) &buf1, SIZE_MAX, 0)) == -1) {
        perror("Client1 doesn't want starting play");
    } else {
        if (buf1[0] == '1') {
            TCOD_key_t key;
            int timer;
            switch (buf1[1]) {
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
                    continue;
            }

            for (int i = 6; i < x; i++)
                if (buf[i] == '@') {
                    firstPlrPos = i;
                } else if (buf[i] == '$') {
                    secondPlrPos = i;
                }

            Moving(buf, firstPlrPos, colourVec, CharWin, timer, key);
            send(sock, buf, strlen(buf), 0);
        } else if (buf1[0] == 's') {
            SendMap(tmp2, x, buf);
            //SendMap(tmp2, newsock2, buf);
            printf("Map message sent\n");
        }
    }

}

std::string RandMap(const std::map<int, std::string> &x)
{
    std::string tmp;
    int rndmap = TCODRandom::getInstance()->getInt((x.begin()->first) - 1, x.size() - 1, 0);
    for (auto p : x) {
        if (p.first == (rndmap+1)){
            tmp = p.second;
            return tmp;
        }
    }
}


void *get_approp_addr(struct sockaddr *sock_a)
{
    if (sock_a->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sock_a)->sin_addr);
    }
    return &(((struct sockaddr_in6*)sock_a)->sin6_addr);
}

void SendMap(std::string map, int sock, char *buf){
    std::ifstream in(map);
    std::string str;
    while (!in.eof()) {
        getline(in, str);
        str.append("\n");
        std::strcpy(buf, str.c_str());
        send(sock, buf, strlen(buf), 0);
    }
}
int rndmoves = TCODRandom::getInstance()->getInt(1, 4, 0);
std::vector<Box> newNum;
int k, turn = 0, Score;
std::vector<int> Num;
char chArray[]= {'0','1','2','3','4','5','6','7','8','9',
                 'a','b','c','d','e','f','g','h','i','j',
                 'k','l','m','n','o','p','q','r','s','t',
                 'u','v','w','x','y','z','A','B','C','D',
                 'E','F','G','H','I','J','K','L','M','N',
                 'O','P','Q','R','S','T','U','V','W','X',
                 'Y','Z'};
char* pos = chArray;

void Moving(char *buf, int x, const std::vector<TCODColor>& colVec, const std::vector<Coord>& CharWin, int timer, TCOD_key_t key){
    std::string strtime = std::to_string(timer/60) + ":" + std::to_string(timer%60);
    TCODConsole::root->print(1, 1, strtime.c_str());
    TCODConsole::root->flush();
    int h = std::atoi(&buf[0]);
    int w = std::atoi(&buf[3]);
    /*Box box;

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
    }*/
    char  tmp;
    if ( key.vk == TCODK_UP  || rndmoves == 1 ) {
       buf[x] = tmp;
       buf[x] = buf[x-w];
       buf[x-w] = tmp;
       x -= w;
        /*if (tnp != '#') {
            if (count1 != newNum.end()) {
                if (tnp2 == '#' || (count2 != newNum.end())) {}
                else{
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
                        }
                    }
                }
            } else {
                TCODConsole::root -> setChar(tmpPlrPos[0], tmpPlrPos[1] - 1, '@');

                TCODConsole::root-> setChar(tmpPlrPos[0], tmpPlrPos[1], ' ');
                tmpPlrPos = {tmpPlrPos[0], tmpPlrPos[1] - 1};
            }
        }
        for (auto k : CharWin) {
            if(tmpPlrPos[0] == k._i && (tmpPlrPos[1] + 1) == k._j){
                TCODConsole::root -> setChar(tmpPlrPos[0], tmpPlrPos[1] + 1, '+');

            }
        }*/
    }

    else if ( key.vk == TCODK_DOWN  || rndmoves == 2 ) {
        buf[x] = tmp;
        buf[x] = buf[x+w];
        buf[x+w] = tmp;
        x += w;
        /*if (tnp3 != '#'){
            if (count3 != newNum.end()){
                if (tnp4 == '#' || (count4 != newNum.end())){}
                else {
                    TCODConsole::root -> setChar(tmpPlrPos[0], tmpPlrPos[1] + 1, '@');

                    TCODConsole::root-> setChar(tmpPlrPos[0], tmpPlrPos[1], ' ');

                    TCODConsole::root -> setChar(tmpPlrPos[0], tmpPlrPos[1] + 2, chBox3);

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

                        }
                    }
                }
            }
            else {
                TCODConsole::root -> setChar(tmpPlrPos[0], tmpPlrPos[1] + 1, '@');

                TCODConsole::root-> setChar(tmpPlrPos[0], tmpPlrPos[1], ' ');

                tmpPlrPos = {tmpPlrPos[0], tmpPlrPos[1] + 1};
            }
        }
        for (auto k : CharWin) {
            if(tmpPlrPos[0] == k._i && (tmpPlrPos[1] - 1) == k._j){
                TCODConsole::root -> setChar(tmpPlrPos[0], tmpPlrPos[1] - 1, '+');

            }
        }*/
    }

    else if ( key.vk == TCODK_LEFT || rndmoves == 3 ) {
        buf[x] = tmp;
        buf[x] = buf[x-1];
        buf[x-1] = tmp;
        x -= 1;
        /*if (tnp5 != '#'){
            if (count5 != newNum.end()){
                if (tnp6 == '#' || (count6 != newNum.end())){}
                else {
                    TCODConsole::root -> setChar(tmpPlrPos[0] - 1, tmpPlrPos[1], '@');

                    TCODConsole::root-> setChar(tmpPlrPos[0], tmpPlrPos[1], ' ');

                    TCODConsole::root -> setChar(tmpPlrPos[0] - 2, tmpPlrPos[1], chBox5);

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

                        }
                    }
                }
            }
            else {
                TCODConsole::root -> setChar(tmpPlrPos[0] - 1, tmpPlrPos[1], '@');

                TCODConsole::root -> setChar(tmpPlrPos[0], tmpPlrPos[1], ' ');

                tmpPlrPos = {tmpPlrPos[0] - 1, tmpPlrPos[1]};
            }
        }
        for (auto k : CharWin) {
            if((tmpPlrPos[0] + 1) == k._i && tmpPlrPos[1] == k._j){
                TCODConsole::root -> setChar(tmpPlrPos[0] + 1, tmpPlrPos[1], '+');

            }
        }*/
    }

    else if ( key.vk == TCODK_RIGHT  || rndmoves == 4) {
        buf[x] = tmp;
        buf[x] = buf[x+1];
        buf[x+1] = tmp;
        x += 1;
       /* if (tnp7 != '#') {
            if (count7 != newNum.end()){
                if (tnp8 == '#' || (count8 != newNum.end())){}
                else {
                    TCODConsole::root->setChar(tmpPlrPos[0] + 1, tmpPlrPos[1], '@');

                    TCODConsole::root->setChar(tmpPlrPos[0], tmpPlrPos[1], ' ');

                    TCODConsole::root->setChar(tmpPlrPos[0] + 2, tmpPlrPos[1], chBox7);

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
                        }
                    }
                }
            }
            else {
                TCODConsole::root->setChar(tmpPlrPos[0] + 1, tmpPlrPos[1], '@');

                TCODConsole::root->setChar(tmpPlrPos[0], tmpPlrPos[1], ' ');

                tmpPlrPos = {tmpPlrPos[0] + 1, tmpPlrPos[1]};
            }
        }
        for (auto k : CharWin) {
            if((tmpPlrPos[0] - 1) == k._i && tmpPlrPos[1] == k._j){
                TCODConsole::root -> setChar(tmpPlrPos[0] - 1, tmpPlrPos[1], '+');

            }
        }
    }*/
    int k=0;
    turn++;
    Score = 700-turn*10;

    /*for(auto num = newNum.begin() ; num != newNum.end(); num++){
        if (num->_win == false){
            num->_num = pos[--Num[k++]-1];
        }
        TCODConsole::root->setChar(num->_box._i, num->_box._j, num->_num);
    }
    TCODConsole::root->flush();
}*/
}
}

/*char foo(const std::vector<Box>& x, char y){
    for (auto &n : x){
        if (n._num == y){
            return n._num;
        }
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
}*/