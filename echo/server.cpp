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
#include <netdb.h>
#include <cstring>
#include <dirent.h>
#include "libtcod.hpp"
#include "libtcod.h"
#include "game_proj.h"
#include <map>
#include <algorithm>

#define SIZE_MAX 1024

int main() {
    int sock, newsock1, newsock2, sz, rndmap;
    struct sockaddr_in addr, cl_addr;
    struct sockaddr *cp;
    struct hostent *rhost;
    char *host;

    char buf1[SIZE_MAX] = {0};
    char buf2[SIZE_MAX] = {0};
    char buf[SIZE_MAX] = {0};
    char buf3[SIZE_MAX] = {0};
    std::string nameMap;
    std::map<int, std::string> maps;
    socklen_t cladrrsz = sizeof(cl_addr);
    cp = (struct sockaddr *) &cl_addr;
    sz = sizeof(addr);
    char lose[20];
    char win[20];

    const TCODColor player{0, 255, 0};
    const TCODColor wall{255, 0, 0};
    const TCODColor box{0, 255, 255};
    const TCODColor winCross{255, 255, 255};
    const std::vector<TCODColor> colourVec = {player, wall, box, winCross};
    std::vector<int> Plus;
    std::vector<Box> newNum;

    int turn = 0, Score;
    std::vector<int> Num;
    char chArray[]= {'0','1','2','3','4','5','6','7','8','9',
                     'a','b','c','d','e','f','g','h','i','j',
                     'k','l','m','n','o','p','q','r','s','t',
                     'u','v','w','x','y','z','A','B','C','D',
                     'E','F','G','H','I','J','K','L','M','N',
                     'O','P','Q','R','S','T','U','V','W','X',
                     'Y','Z'};
    char* pos = chArray;

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



    CreateMap(tmp2, buf);
    int h = std::atoi(&buf[0]);
    int w = std::atoi(&buf[3]);
    CreateBox(newNum, buf, w, h, pos);
    PlusPos(buf, Plus);

    std::string Lose = "Lose";
    std::string Win = "Win";
    strcpy(lose, Lose.c_str());
    strcpy(win, Win.c_str());

    while (!WinPos(newNum) && (newNum.at(LosePos(newNum))._num != '0')) {
        printf("\nHi,Iam running server.Some Client hit me\n");
        newsock1 = accept(sock, (struct sockaddr *) &cl_addr, &cladrrsz);
        func(newsock1, (sockaddr *) &cl_addr, tmp2);
        printf("\nHi,Iam running server.Some Client hit me\n");
        newsock2 = accept(sock, (struct sockaddr *) &cl_addr, &cladrrsz);
        func(newsock2, (sockaddr *) &cl_addr, tmp2);
        //recv(newsock, buf1, SIZE_MAX-1,0);
        ReadAndMoving(sock, newsock1, buf, buf1, buf3, newNum, Plus, tmp2, pos);
        ReadAndMoving(sock, newsock2, buf, buf1, buf3, newNum, Plus, tmp2, pos);

        if (newNum.at(LosePos(newNum))._num == '0') {
            send(sock, lose, sizeof(Lose), 0);
        }
        if(WinPos(newNum)) {
            send(sock, win, sizeof(Win), 0);
        }

    }
    return 0;
}

void func(int x, struct sockaddr *cl_addr, std::string map){

    char address_pres[INET6_ADDRSTRLEN];

    //printf("\nHi,Iam running server.Some Client hit me\n");
    if (x == -1) {
        perror("no accept Cl");
    }

    inet_ntop(cl_addr->sa_family,
              get_approp_addr((struct sockaddr *) &cl_addr),
              address_pres, sizeof(address_pres));
    printf("server: Client connection from %s\n", address_pres);
}

void ReadAndMoving(int s, int x, char *b, char *b1, char *b3, std::vector<Box>& boxes, const std::vector<int>& plus, std::string mapname, char *pos){

    int firstPlrPos, secondPlrPos, k = 0;

    if ((recv(x, (char *) &b1, SIZE_MAX - 1, 0)) == -1) {
        perror("Client doesn't want starting play");
    } else if (b1[0] == '1') {
            TCOD_key_t key;
            int timer;
            switch (b1[1]) {
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

            for (int i = 6; i < x; i++)
                if (b[i] == '@') {
                    firstPlrPos = i;
                } else if (b[i] == '$') {
                    secondPlrPos = i;
                }

            Moving(b, firstPlrPos, secondPlrPos, plus, timer, key, boxes, pos);
            for(auto p : boxes) {
                b3[k++] = p._num;
            }
            send(s, b, sizeof(b),0);
            send(s, b3, sizeof(b3), 0);
            //send(sock, buf, strlen(buf), 0);
        } else if (b1[0] == 's') {
            send(s, b, sizeof(b),0);
            //SendMap(tmp2, newsock2, buf);
            printf("Map message sent\n");
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

void CreateMap(std::string map, char *buf) {
    std::ifstream in(map);
    std::string str;
    std::string str1;
    while (!in.eof()) {
        getline(in, str);
        str.append("\n");
        str1 += str;
    }
    std::strcpy(buf, str1.c_str());
    // send(sock, buf, strlen(buf), 0);
}



void CreateBox(std::vector<Box>& b, char *buf, int w, int h, char *pos) {
    Box box;
    int nmb = 7;
    for (int i = 0; i < sizeof(buf); i++) {
        if (buf[i] == 'o') {
            box._box = i;
            box._num = pos[std::atoi(&buf[w * h + nmb])-1];
            box._win = false;
            b.emplace_back(box);
            nmb +=3;
            buf[i] = box._num;
        }
    }
}

void PlusPos(char *b, std::vector<int>& p){
    for (auto i = 0; i < sizeof(b); i++){
        if (b[i] == '+'){
        p.emplace_back(i);
        }
    }
}


int BoxPos1(const std::vector<Box>& k, const std::vector<int>& m){
    for (auto pos = 0; pos < k.size(); pos++) {
        for (auto p : m) {
            if (k[pos]._win != true) {
                if (k[pos]._box == p)
                    return pos;
            }
        }
    }
}

void Moving(char *buf, int x, int y, const std::vector<int>& plus, int timer, TCOD_key_t key, std::vector<Box>& boxes, char *pos) {
    std::string strtime = std::to_string(timer / 60) + ":" + std::to_string(timer % 60);
    TCODConsole::root->print(1, 1, strtime.c_str());
    TCODConsole::root->flush();
    int h = std::atoi(&buf[0]);
    int w = std::atoi(&buf[3]);
    int rndmoves = TCODRandom::getInstance()->getInt(1, 4, 0);
    /*Box box;
    char chBox = foo(newNum, tnp);
    char chBox3 = foo(newNum, tnp3);
    char chBox5 = foo(newNum, tnp5);
    char chBox7 = foo(newNum, tnp7);
    */

    int tnp = x - w;
    MyPred pred1(tnp);
    auto count1 = std::find_if(boxes.begin(), boxes.end(), pred1);
    int tnp2 = x + w;
    MyPred pred2(tnp2);
    auto count2 = std::find_if(boxes.begin(), boxes.end(), pred2);
    int tnp3 = x - 1;
    MyPred pred3(tnp3);
    auto count3 = std::find_if(boxes.begin(), boxes.end(), pred3);
    int tnp4 = x + 1;
    MyPred pred4(tnp4);
    auto count4 = std::find_if(boxes.begin(), boxes.end(), pred4);
    int tnp5 = x - w * 2;
    MyPred pred5(tnp5);
    auto count5 = std::find_if(boxes.begin(), boxes.end(), pred5);
    int tnp6 = x + w * 2;
    MyPred pred6(tnp6);
    auto count6 = std::find_if(boxes.begin(), boxes.end(), pred6);
    int tnp7 = x - 2;
    MyPred pred7(tnp7);
    auto count7 = std::find_if(boxes.begin(), boxes.end(), pred7);
    int tnp8 = x + 2;
    MyPred pred8(tnp8);
    auto count8 = std::find_if(boxes.begin(), boxes.end(), pred8);

    if (rndmoves == 1) {
        if (buf[tnp] == '#' || buf[tnp] == y) {
            rndmoves = TCODRandom::getInstance()->getInt(2, 4, 0);
        } else if (count1 != boxes.end() && buf[tnp2] == '#') {
            rndmoves = TCODRandom::getInstance()->getInt(2, 4, 0);
        } else if (count1 != boxes.end() && count2 != boxes.end()) {
            rndmoves = TCODRandom::getInstance()->getInt(2, 4, 0);
        }
    }

    if (rndmoves == 2) {
        if (buf[tnp3] == '#' || buf[tnp3] == y) {
            rndmoves = TCODRandom::getInstance()->getInt(1, 4, 0);
        } else if (count3 != boxes.end() && buf[tnp4] == '#') {
            rndmoves = TCODRandom::getInstance()->getInt(1, 4, 0);
        } else if (count3 != boxes.end() && count4 != boxes.end()) {
            rndmoves = TCODRandom::getInstance()->getInt(1, 4, 0);
        }
    }

    if (rndmoves == 3) {
        if (buf[tnp5] == '#' || buf[tnp5] == y) {
            rndmoves = TCODRandom::getInstance()->getInt(1, 4, 0);
        } else if (count5 != boxes.end() && buf[tnp6] == '#') {
            rndmoves = TCODRandom::getInstance()->getInt(1, 4, 0);
        } else if (count5 != boxes.end() && count6 != boxes.end()) {
            rndmoves = TCODRandom::getInstance()->getInt(1, 4, 0);
        }
    }

    if (rndmoves == 4) {
        if (buf[tnp7] == '#' || buf[tnp7] == y) {
            rndmoves = TCODRandom::getInstance()->getInt(1, 3, 0);
        } else if (count7 != boxes.end() && buf[tnp8] == '#') {
            rndmoves = TCODRandom::getInstance()->getInt(1, 3, 0);
        } else if (count7 != boxes.end() && count8 != boxes.end()) {
            rndmoves = TCODRandom::getInstance()->getInt(1, 3, 0);
        }
    }

    char tmp;
    if (key.vk == TCODK_UP || rndmoves == 1) {
        if (buf[tnp] != '#' && buf[tnp] != buf[y]) {
            if (buf[tnp] == 'o') {
                if (buf[tnp5] != '#' && buf[tnp5] != 'o' && buf[tnp5] != buf[y]) {
                    buf[tnp5] = buf[tnp];
                    tmp = buf[x];
                    buf[x] = buf[tnp];
                    buf[tnp] = tmp;
                    x -= w;
                    tnp -= w;
                    boxes[BoxPos1(boxes, plus)]._win = true;
                }
            } else {
                tmp = buf[x];
                buf[x] = buf[tnp];
                buf[tnp] = tmp;
                x -= w;
            }
        }
    }
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


    else if (key.vk == TCODK_DOWN || rndmoves == 2) {
        if (buf[tnp2] != '#' && buf[tnp2] != buf[y]) {
            if (buf[tnp2] == 'o') {
                if (buf[tnp6] != '#' && buf[tnp6] != 'o' && buf[tnp6] != buf[y]) {
                    buf[tnp6] = buf[tnp2];
                    tmp = buf[x];
                    buf[x] = buf[tnp2];
                    buf[tnp2] = tmp;
                    x += w;
                    tnp2 += w;
                    boxes[BoxPos1(boxes, plus)]._win = true;
                }
            } else {
                tmp = buf[x];
                buf[x] = buf[tnp2];
                buf[tnp2] = tmp;
                x += w;
            }
        }
    } else if (key.vk == TCODK_LEFT || rndmoves == 3) {
        if (buf[tnp3] != '#' && buf[tnp3] != buf[y]) {
            if (buf[tnp3] == 'o') {
                if (buf[tnp7] != '#' && buf[tnp7] != 'o' && buf[tnp7] != buf[y]) {
                    buf[tnp7] = buf[tnp3];
                    tmp = buf[x];
                    buf[x] = buf[tnp3];
                    buf[tnp3] = tmp;
                    x -= 1;
                    tnp3 -= 1;
                    boxes[BoxPos1(boxes, plus)]._win = true;
                }
            } else {
                tmp = buf[x];
                buf[x] = buf[tnp3];
                buf[tnp3] = tmp;
                x -= 1;
            }
        }
    } else if (key.vk == TCODK_RIGHT || rndmoves == 4) {
        if (buf[tnp4] != '#' && buf[tnp4] != buf[y]) {
            if (buf[tnp4] == 'o') {
                if (buf[tnp8] != '#' && buf[tnp8] != 'o' && buf[tnp8] != buf[y]) {
                    buf[tnp8] = buf[tnp4];
                    tmp = buf[x];
                    buf[x] = buf[tnp4];
                    buf[tnp4] = tmp;
                    x += 1;
                    tnp4 += 1;
                    boxes[BoxPos1(boxes, plus)]._win = true;
                }
            } else {
                tmp = buf[x];
                buf[x] = buf[tnp4];
                buf[tnp4] = tmp;
                x += 1;
            }
        }
    }
    //turn++;
    //Score = 700-turn*10;

    for(auto num = boxes.begin() ; num != boxes.end(); num++){
        if (num->_win == false){
            num->_num = pos[--num->_num-1];
        }
    }
}

int LosePos(const std::vector<Box>& x) {
    for (auto i = 0; i < x.size(); i++) {
        if (x.at(i)._num == '0')
            return i;
    }
}

bool WinPos(const std::vector<Box>& m){
    int sw = 0;
    for (auto &s : m){
        if (s._win == true) {
            sw++;
        }
    }
    if (sw == m.size()){
        return true;
    } else {
        return  false;
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
*/
