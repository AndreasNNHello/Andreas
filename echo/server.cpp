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
#include <fcntl.h>

int main() {
    int sock, newsock1, newsock2, sz, count, turnf = 0, scoref = 700,
        timer1, scores = 700, interval = 0, turns = 0, timer, sttime;
    int firstPlrPos, secondPlrPos;
    struct sockaddr_in addr, cl_addr;
    struct sockaddr *cp;
    char buf[SIZE_MAX] = {0};
    char buf2[SIZE_MAX] = {0};
    char nbuf[SIZE_MAX] = {0};
    std::string nameMap;
    std::map<int, std::string> maps;

    cp = (struct sockaddr *) &cl_addr;
    sz = sizeof(addr);

    std::vector<int> Plus;
    std::vector<Box> newNum;

    socklen_t cladrrsz = sizeof(cl_addr);
    char address_pres[INET6_ADDRSTRLEN];

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
        close(sock);
    }

    if (listen(sock, 1) == -1) {
        perror("listen");
    }

    CreateMap(maps, buf);
    int h = std::atoi(&buf[0]);
    int w = std::atoi(&buf[3]);
    CreateBoxAndPlus(newNum, buf, w, h, Plus, &count);
    printf("\nHi,Iam running server.Some Client hit me\n");
    newsock1 = accept(sock, (struct sockaddr *) &cl_addr, &cladrrsz);
    fcntl(newsock1,F_SETFL, O_NONBLOCK);
    if (newsock1 == -1) {
        perror("no accept Cl1");
    }

    inet_ntop(cl_addr.sin_family,
              get_approp_addr((struct sockaddr *) &cl_addr),
              address_pres, sizeof address_pres);
    printf("server: Client connection from %s\n", address_pres);
    Convert(buf, nbuf, newNum, count, scoref);
    send(newsock1, nbuf, strlen(nbuf), 0);
    printf("Map message sent\n");

    printf("\nHi,Iam running server.Some Client hit me\n");
    newsock2 = accept(sock, (struct sockaddr *) &cl_addr, &cladrrsz);
    fcntl(newsock2,F_SETFL, O_NONBLOCK);
    if (newsock2 == -1) {
        perror("no accept Cl2");
    }

    inet_ntop(cl_addr.sin_family,
              get_approp_addr((struct sockaddr *) &cl_addr),
              address_pres, sizeof address_pres);
    printf("server: Client connection from %s\n", address_pres);
    Convert(buf, nbuf, newNum, count, scores);
    send(newsock2, nbuf, strlen(nbuf), 0);
    printf("Map message sent\n");

    timer1 = count;
    interval = 0;
    timer = count;
    sttime = time(0);
    Player pl(true);
    Time timer2(count);
    while (1) {
        if (newsock1 == -1 || newsock2 == -1) {
            perror("no accept");
        }

        int z;

        if (timer2.GetCurrentTime(interval, timer1, sttime) != timer){
            if (pl.getBrake()) {
                if (z = (recv(newsock1, (char *) &buf2, SIZE_MAX, MSG_DONTWAIT)) == -1) {
                    perror("recv1");
                 }
                pl.WhoIsWho('@', '$', buf, &firstPlrPos, &secondPlrPos);
                timer = timer2.GetCurrentTime(interval, timer1, sttime);
                if (timer == 0){
                    pl.RandomTurn(buf, firstPlrPos, secondPlrPos, Plus, newNum, w, h, &timer, &timer1, &interval, &sttime, timer2, &turnf, &scoref);
                    pl.setBrake(false);
                    std::cout << pl.getBrake() << std::endl;
                    std::cout << "first rand" << std::endl;
                } else {
                    if (buf2[0] == '1'){
                    pl.Turn(buf2, buf, firstPlrPos, secondPlrPos, Plus, newNum, w, h, &timer, &timer1, &interval, &sttime, timer2, &turnf, &scoref);
                    pl.setBrake(false);
                    std::cout << "turn" << std::endl;
                    }
                }
                pl.Send(buf, nbuf, newNum, newsock1, newsock2, timer, scoref, scores);
                std::cout << "first send" << std::endl;
            } else {
                if (z = (recv(newsock2, (char *) &buf2, SIZE_MAX, MSG_DONTWAIT)) == -1) {
                    perror("recv2");
                }
                pl.WhoIsWho('$', '@', buf, &firstPlrPos, &secondPlrPos);
                timer = timer2.GetCurrentTime(interval, timer1, sttime);
                if (timer == 0){
                    pl.RandomTurn(buf, firstPlrPos, secondPlrPos, Plus, newNum, w, h, &timer, &timer1, &interval, &sttime, timer2, &turns, &scores);
                    std::cout << "second rand" << std::endl;
                    pl.setBrake(true);
                } else {
                    if (buf2[0] == '1'){
                    pl.Turn(buf2, buf, firstPlrPos, secondPlrPos, Plus, newNum, w, h, &timer, &timer1, &interval, &sttime, timer2, &turns, &scores);
                    pl.setBrake(true);
                    std::cout << "second turn" << std::endl;
                    }
                }
                pl.Send(buf, nbuf, newNum, newsock1, newsock2, timer, scoref, scores);
                std::cout << "second send" << std::endl;
            }
            Player::Lose(buf, newNum, pl, newsock1, newsock2);
            Player::Win(buf, newNum, pl, newsock1, newsock2);
        }
    }
#pragma clang diagnostic pop
     return 0;
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

int CreateMap(std::map<int, std::string> maps, char *buf) {
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

    std::ifstream in(tmp2);
    std::string str;
    std::string str1;
    while (!in.eof()) {
        getline(in, str);
        str.append("\n");
        str1 += str;
    }
    std::strcpy(buf, str1.c_str());
    return 0;
}



void CreateBoxAndPlus(std::vector<Box>& b, char *buf, int w, int h, std::vector<int>& plus, int* c_ptr) {
    Box box;
    int nmb = 6;
    for (int i = 0; i < strlen(buf); i++) {
        if (buf[i] == 'o') {
            box._box = i;
            int t = (w+1)*h;
            int p = std::atoi(&buf[t + nmb]);
            box._num = p;
            box._win = false;
            b.emplace_back(box);
            nmb +=3;
        }
        else if (buf[i] == '+') {
            plus.emplace_back(i);
        }
    }
    *c_ptr = std::atoi(&buf[(w+1)*(h)+nmb]);
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

void Moving(char *buf, int fp, int sp, const std::vector<int>& plus, TCOD_key_t key, std::vector<Box>& boxes, int rnd, int w, int h) {
    int rand;

    int tnp = fp - w - 1;
    int tnp2 = fp + w + 1;
    int tnp3 = fp - 1;
    int tnp4 = fp + 1;
    int tnp5 = fp - w * 2 - 2;
    int tnp6 = fp + w * 2 + 2;
    int tnp7 = fp - 2;
    int tnp8 = fp + 2;

    if (rnd != 0) {
        rand = ControlRand(buf, tnp, tnp2, tnp3, tnp4, tnp5, tnp6, tnp7, tnp8, sp, rnd);
    }

    if (key.vk == TCODK_UP || rand == 1) {
        if (buf[tnp] != '#' && buf[tnp] != buf[sp]){
            if (buf[tnp] == 'o') {
                if (buf[tnp5] != '#' && buf[tnp5] != 'o' && buf[tnp5] != buf[sp]) {
                    boxes[BoxPos(boxes, tnp)]._box -= (w+1);
                    buf[tnp5] = buf[tnp];
                    buf[tnp] = buf[fp];
                    buf[fp] = ' ';
                    fp -= (w+1);
                    for (auto a : plus) {
                        if(a == tnp5){
                            boxes[BoxPos1(boxes, plus)]._win = true;
                        }
                        if ((fp+w+1) == a) {
                            buf[fp+w+1] = '+';
                        }
                        if (tnp == a){
                            boxes[BoxPos(boxes, tnp5)]._win = false;
                        }
                    }
                    
                }
            }
            else  {
                buf[tnp] = buf[fp];
                buf[fp] = ' ';
                fp -= (w+1);
                for (auto a : plus) {
                    if ((fp+w+1) == a) {
                        buf[fp+w+1] = '+';
                    }
                }
            }
        }
    }

    else if (key.vk == TCODK_DOWN || rand == 2) {
        if (buf[tnp2] != '#' && buf[tnp2] != buf[sp]) {
            if (buf[tnp2] == 'o') {
                if (buf[tnp6] != '#' && buf[tnp6] != 'o' && buf[tnp6] != buf[sp]) {
                    boxes[BoxPos(boxes, tnp2)]._box += (w + 1);
                    buf[tnp6] = buf[tnp2];
                    buf[tnp2] = buf[fp];
                    buf[fp] = ' ';
                    fp += (w + 1);
                    for (auto a : plus) {
                        if (a == tnp6) {
                            boxes[BoxPos1(boxes, plus)]._win = true;
                        }
                        if ((fp - w - 1) == a) {
                            buf[fp - w - 1] = '+';
                        }
                        if (tnp2 == a) {
                            boxes[BoxPos(boxes, tnp6)]._win = false;
                        }
                    }
                }
            } else {
                buf[tnp2] = buf[fp];
                buf[fp] = ' ';
                fp += (w + 1);
                for (auto a : plus) {
                    if ((fp - w - 1) == a) {
                        buf[fp - w - 1] = '+';
                    }
                }
            }
        }
    }

    else if (key.vk == TCODK_LEFT || rand == 3) {
            if (buf[tnp3] != '#' && buf[tnp3] != buf[sp]) {
                if (buf[tnp3] == 'o') {
                    if (buf[tnp7] != '#' && buf[tnp7] != 'o' && buf[tnp7] != buf[sp]) {
                        boxes[BoxPos(boxes, tnp3)]._box -= 1;
                        buf[tnp7] = buf[tnp3];
                        buf[tnp3] = buf[fp];
                        buf[fp] = ' ';
                        fp -= 1;
                        for (auto a : plus) {
                            if (a == tnp7) {
                                boxes[BoxPos1(boxes, plus)]._win = true;
                            }
                            if ((fp + 1) == a) {
                                buf[fp + 1] = '+';
                            }
                            if (tnp3 == a) {
                                boxes[BoxPos(boxes, tnp7)]._win = false;
                            }
                        }
                    }
                } else {
                        buf[tnp3] = buf[fp];
                        buf[fp] = ' ';
                        fp -= 1;
                        for (auto a : plus) {
                            if ((fp + 1) == a) {
                                buf[fp + 1] = '+';
                            }
                        }
                    }
                }
            }
    else if (key.vk == TCODK_RIGHT || rand == 4) {
                if (buf[tnp4] != '#' && buf[tnp4] != buf[sp]) {
                    if (buf[tnp4] == 'o') {
                        if (buf[tnp8] != '#' && buf[tnp8] != 'o' && buf[tnp8] != buf[sp]) {
                            boxes[BoxPos(boxes, tnp4)]._box += 1;
                            buf[tnp8] = buf[tnp4];
                            buf[tnp4] = buf[fp];
                            buf[fp] = ' ';
                            fp += 1;
                            for (auto a : plus) {
                                if (a == tnp8) {
                                    boxes[BoxPos1(boxes, plus)]._win = true;
                                }
                                if ((fp - 1) == a) {
                                    buf[fp - 1] = '+';
                                }
                                if (tnp4 == a) {
                                    boxes[BoxPos(boxes, tnp8)]._win = false;
                                }
                            }
                        }
                    } else {
                            buf[tnp4] = buf[fp];
                            buf[fp] = ' ';
                        fp += 1;
                            for (auto a : plus) {
                                if ((fp - 1) == a) {
                                    buf[fp - 1] = '+';
                                }
                            }
                        }
                    }
                }

                for (auto num = boxes.begin(); num != boxes.end(); num++) {
                    if (num->_win == false) {
                        num->_num--;
                    }
                }
}

int LosePos(const std::vector<Box>& x) {
    for (auto i = 0; i < x.size(); i++) {
        if (x.at(i)._num == 0)
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

int BoxPos(const std::vector<Box>& k, int t) {
    for (auto i = 0; i < k.size(); i++) {
        if (k[i]._box == t)
            return i;
    }
}

int ControlRand(char *buf, int tnp, int tnp2, int tnp3, int tnp4, int tnp5, int tnp6, int tnp7, int tnp8, int sp, int rand){
    bool condition = false;
    while (condition == false){
        if (rand == 1) {
            if (buf[tnp] == '#' || (buf[tnp] == buf[sp])) {
                rand = TCODRandom::getInstance()->getInt(2, 4, 0);
            } else if (buf[tnp] == 'o' && buf[tnp5] == '#') {
                rand = TCODRandom::getInstance()->getInt(2, 4, 0);
            } else if (buf[tnp] == 'o' && buf[tnp5] == 'o') {
                rand = TCODRandom::getInstance()->getInt(2, 4, 0);
            } else if (buf[tnp] == 'o' && buf[tnp5] == buf[sp]) {
                rand = TCODRandom::getInstance()->getInt(2, 4, 0);
            } else {
                condition = true;
            }
        }

        if (rand == 2) {
            if (buf[tnp2] == '#' || (buf[tnp2] == buf[sp])) {
                rand = TCODRandom::getInstance()->getInt(1, 4, 0);
            } else if (buf[tnp2] == 'o' && buf[tnp6] == '#') {
                rand = TCODRandom::getInstance()->getInt(1, 4, 0);
            } else if (buf[tnp2] == 'o' && buf[tnp6] == 'o') {
                rand = TCODRandom::getInstance()->getInt(1, 4, 0);
            } else if (buf[tnp2] == 'o' && buf[tnp6] == buf[sp]) {
                rand = TCODRandom::getInstance()->getInt(1, 4, 0);
            } else {
                condition = true;
            }
        }

        if (rand == 3) {
            if (buf[tnp3] == '#' || (buf[tnp3] == buf[sp])) {
                rand = TCODRandom::getInstance()->getInt(1, 4, 0);
            } else if (buf[tnp3] == 'o' && buf[tnp7] == '#') {
                rand = TCODRandom::getInstance()->getInt(1, 4, 0);
            } else if (buf[tnp3] == 'o' && buf[tnp7] == 'o') {
                rand = TCODRandom::getInstance()->getInt(1, 4, 0);
            } else if (buf[tnp3] == 'o' && buf[tnp7] == buf[sp]) {
                rand = TCODRandom::getInstance()->getInt(1, 4, 0);
            } else {
            condition = true;
            }
        }

        if (rand == 4) {
            if (buf[tnp4] == '#' || (buf[tnp4] == buf[sp])) {
                rand = TCODRandom::getInstance()->getInt(1, 3, 0);
            } else if (buf[tnp4] == 'o' && buf[tnp8] == '#') {
                rand = TCODRandom::getInstance()->getInt(1, 3, 0);
            } else if (buf[tnp4] == 'o' && buf[tnp8] == 'o') {
                rand = TCODRandom::getInstance()->getInt(1, 3, 0);
            } else if (buf[tnp4] == 'o' && buf[tnp8] == buf[sp]) {
                rand = TCODRandom::getInstance()->getInt(1, 3, 0);
            } else {
                condition = true;
            }
        }
    }
    return rand;
}

int Time::GetCurrentTime(int interval, int timer1, int sttime){
    return interval + timer1 - time(0) + sttime;
}

void Time::Reset(int* timer, int timer1, int* interval, int sttime){
    *timer = timer1;
    *interval = time(0) - sttime;
}

void Player::Turn(char *b2, char *buf, int fp, int sp, const std::vector<int>& plus, std::vector<Box>& newNum, int w, int h, int* timer, int* timer1, int* interval, int* sttime, Time t, int* turn, int* score){
    TCOD_key_t key;
    int rndmoves;
    if (b2[0] == '1') {
        switch (b2[1]) {
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

        Moving(buf, fp, sp, plus, key, newNum, rndmoves, w, h);
        t.Reset(timer, *timer1, interval, *sttime);
        *turn += 1;
        *score = 700 - (*turn * 10);
        key.vk = TCODK_NONE;
        memset(b2, 0, sizeof(b2));
    }
}

void Player::RandomTurn(char *buf, int fp, int sp, const std::vector<int>& plus, std::vector<Box>& newNum, int w, int h, int* timer, int* timer1, int* interval, int* sttime, Time t, int* turn, int* score){
    int rndmoves;
    TCOD_key_t key;
    std::cout << "rand turn" << std::endl;
    rndmoves = TCODRandom::getInstance()->getInt(1, 4, 0);
    Moving(buf, fp, sp, plus, key, newNum, rndmoves, w, h);
    *turn += 1;
    *score = 700-*turn*20;
    t.Reset(timer, *timer1, interval, *sttime);
    rndmoves = 0;
}

void Player::WhoIsWho(char f, char s, char *buf, int *fp, int *sp){
    for (int i = 6; i < strlen(buf); i++){
        if (buf[i] == f) {
            *fp = i;
        }
        if (buf[i] == s) {
             *sp = i;
        }
    }
}

void Convert(char* b, char* nb, const std::vector<Box>& boxes, int timer, int score){
    char chArray[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
                      'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
                      'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't',
                      'u', 'v', 'w', 'x', 'y', 'z', 'A', 'B', 'C', 'D',
                      'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N',
                      'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
                      'Y', 'Z'};
    char *pos = chArray;
    std::strcpy(nb, b);
    for (int i = 6; i < strlen(nb); i++){
        for (auto p : boxes){
            if (i == p._box){
                nb[i] = pos[p._num-1];
            }
        }
    }
    std::string nstr = std::to_string(timer) + "\n";
    std::strcpy(& nb[strlen(nb) - 1], nstr.c_str());
    std::string nstr1 = std::to_string(score);
    std::strcpy(& nb[strlen(nb)], nstr1.c_str());
}

void Player::Send(char *buf, char *nbuf, const std::vector<Box>& box, int ns1, int ns2, int timer, int score1, int score2){
    Convert(buf, nbuf, box, timer, score1);
    send(ns1, nbuf, SIZE_MAX, 0);
    Convert(buf, nbuf, box, timer, score2);
    send(ns2, nbuf, SIZE_MAX, 0);
    printf("Map message sent\n");
}

void Player::Lose(char *buf, const std::vector<Box>& box, Player pl, int ns1, int ns2) {
    if (box[LosePos(box)]._num == 0) {
        if (pl.getBrake()) {
            buf[0] = 'L';
            send(ns1, buf, strlen(buf), 0);
            buf[0] = 'W';
            send(ns2, buf, strlen(buf), 0);
        } else {
            buf[0] = 'W';
            send(ns1, buf, strlen(buf), 0);
            buf[0] = 'L';
            send(ns2, buf, strlen(buf), 0);
        }
    }
}

void Player::Win(char *buf, const std::vector<Box>& box, Player pl, int ns1, int ns2){
    if (WinPos(box)) {
        if (pl.getBrake()) {
            buf[0] = 'W';
            send(ns1, buf, strlen(buf), 0);
            buf[0] = 'L';
            send(ns2, buf, strlen(buf), 0);
        } else {
            buf[0] = 'L';
            send(ns1, buf, strlen(buf), 0);
            buf[0] = 'W';
            send(ns2, buf, strlen(buf), 0);
        }
    }
}