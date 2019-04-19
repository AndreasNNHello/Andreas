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

int main() {
    int test = clock()/CLOCKS_PER_SEC;
    int sock, newsock1, newsock2, sz;
    struct sockaddr_in addr, cl_addr;
    struct sockaddr *cp;

    char buf2[SIZE_MAX] = {0};
    char buf1[SIZE_MAX] = {0};
    char buf[SIZE_MAX] = {0};
    char nbuf[SIZE_MAX] = {0};

    std::string nameMap;
    std::map<int, std::string> maps;

    cp = (struct sockaddr *) &cl_addr;
    sz = sizeof(addr);

    const TCODColor player{0, 255, 0};
    const TCODColor wall{255, 0, 0};
    const TCODColor box{0, 255, 255};
    const TCODColor winCross{255, 255, 255};
    const std::vector<TCODColor> colourVec = {player, wall, box, winCross};
    std::vector<int> Plus;
    std::vector<Box> newNum;
    int count;
    int sttime;

    socklen_t cladrrsz = sizeof(cl_addr);
    char address_pres[INET6_ADDRSTRLEN];

    int turn = 0, Score;
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

        CreateMap(tmp2, buf);
    newsock1 = accept(sock, (struct sockaddr *) &cl_addr, &cladrrsz);


    int h = std::atoi(&buf[0]);
    int w = std::atoi(&buf[3]);
    //count = std::atoi(&buf[(w+1)*(h+1)+1]);
    CreateBoxAndPlus(newNum, buf, w, h, Plus, &count);






    printf("\nHi,Iam running server.Some Client hit me\n");
    if (newsock1 == -1) {
        perror("no accept Cl1");
    }

    inet_ntop(cl_addr.sin_family,
              get_approp_addr((struct sockaddr *) &cl_addr),
              address_pres, sizeof address_pres);
    printf("server: Client connection from %s\n", address_pres);

    int timer1 = count;
    int interval = 0;
    int timer = count;
    int rndmoves;
    while (1){
        if(!WinPos(newNum) && (newNum[LosePos(newNum)]._num != 0)) {
            
            /*func(newsock1, (sockaddr *) &cl_addr, sock);
            send(sock, buf, strlen(buf), 0);
            printf("\nHi,Iam running server.Some Client hit me\n");
            //newsock2 = accept(sock, (struct sockaddr *) &cl_addr, &cladrrsz);
            func(newsock2, (sockaddr *) &cl_addr, sock);
            send(sock, buf, strlen(buf), 0);
            *///recv(newsock, buf1, SIZE_MAX-1,0);
            
            //int timer1 = 150;
            
            if ((recv(newsock1, (char *) &buf2, SIZE_MAX, 0)) == -1) {
                perror("Client doesn't want starting play");
            }
            TCOD_key_t key;
            
            
            
            
            sttime = clock()/CLOCKS_PER_SEC;
            
            
            if (timer == 0){
                rndmoves = TCODRandom::getInstance()->getInt(1, 4, 0);
                ReadAndMoving(buf, newNum, Plus, key, rndmoves);
                timer = timer1;
                interval = sttime;
                std::string nstr = std::to_string(timer);
                std::strcpy(&buf[(w + 1) * (h + 1) + 1], nstr.c_str());
                Convert(buf, nbuf, newNum, pos);
                send(newsock1, nbuf, strlen(nbuf), 0);
                rndmoves = 0;
            } else if (buf2[0] == 's'){
                timer = interval + timer1 - sttime;
                std::string nstr = std::to_string(timer);
                std::strcpy(&buf[(w+1)*(h+1)+1], nstr.c_str());
                Convert(buf, nbuf, newNum, pos);
                send(newsock1, nbuf, strlen(nbuf), 0);
                //send(newsock2, buf, strlen(buf), 0);
                printf("Map message sent\n");
            } else if (buf2[0] == '1') {
                switch (buf2[1]) {
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
                
                ReadAndMovingCl(buf, newNum, Plus, key);
                timer = timer1;
                interval = sttime;
                std::string nstr = std::to_string(timer);
                std::strcpy(&buf[(w + 1) * (h + 1) + 1], nstr.c_str());
                Convert(buf, nbuf, newNum, pos);
                send(newsock1, nbuf, strlen(nbuf), 0);
                key.vk = TCODK_NONE;
            }
        } else if (newNum[LosePos(newNum)]._num == 0) {
                    buf[0] = 'L';
                    send(newsock1, buf, strlen(buf), 0);
         } else if (WinPos(newNum)) {
                    buf[0] = 'W';
                    send(newsock1, buf, strlen(buf), 0);
         }
    }
     return 0;
}

/*void func(int x, struct sockaddr *cl_addr, int s){
    socklen_t cladrrsz = sizeof(cl_addr);
    char address_pres[INET6_ADDRSTRLEN];
    printf("\nHi,Iam running server.Some Client hit me\n");
    x = accept(s, (struct sockaddr *) &cl_addr, &cladrrsz);
    //printf("\nHi,Iam running server.Some Client hit me\n");
    if (x == -1) {
        perror("no accept Cl");
    }

    inet_ntop(cl_addr->sa_family,
              get_approp_addr((struct sockaddr *) &cl_addr),
              address_pres, sizeof(address_pres));
    printf("server: Client connection from %s\n", address_pres);
}*/

void ReadAndMoving(char *b, std::vector<Box>& boxes, const std::vector<int>& plus, TCOD_key_t key, int rnd){
    int firstPlrPos, secondPlrPos;

            for (int i = 6; i < strlen(b); i++)
                if (b[i] == '@') {
                    firstPlrPos = i;
                } else if (b[i] == '$') {
                    secondPlrPos = i;
                }

        Moving(b, firstPlrPos, secondPlrPos, plus, key, boxes, rnd);
}

void ReadAndMovingCl(char *b, std::vector<Box>& boxes, const std::vector<int>& plus, TCOD_key_t key){
    int firstPlrPos, secondPlrPos;

    for (int i = 6; i < strlen(b); i++)
        if (b[i] == '@') {
            firstPlrPos = i;
        } else if (b[i] == '$') {
            secondPlrPos = i;
        }
    MovingCl(b, firstPlrPos, secondPlrPos, plus, key, boxes);
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
    //send(sock, buf, strlen(buf), 0);
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
    std::string count1 = &buf[(w+1)*(h)+nmb];
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

void Moving(char *buf, int x, int y, const std::vector<int>& plus, TCOD_key_t key, std::vector<Box>& boxes, int rnd) {
    /*std::string strtime = std::to_string(timer / 60) + ":" + std::to_string(timer % 60);
    TCODConsole::root->print(1, 1, strtime.c_str());
    TCODConsole::root->flush();*/
    int h = std::atoi(&buf[0]);
    int w = std::atoi(&buf[3]);

    /*Box box;
    char chBox = foo(newNum, tnp);
    char chBox3 = foo(newNum, tnp3);
    char chBox5 = foo(newNum, tnp5);
    char chBox7 = foo(newNum, tnp7);
    */

    int tnp = x - w - 1;
    int tnp2 = x + w + 1;
    int tnp3 = x - 1;
    int tnp4 = x + 1;
    int tnp5 = x - w * 2 - 2;
    int tnp6 = x + w * 2 + 2;
    int tnp7 = x - 2;
    int tnp8 = x + 2;
    /*MyPred pred1(tnp);
    auto count1 = std::find_if(boxes.begin(), boxes.end(), pred1);

    MyPred pred2(tnp2);
    auto count2 = std::find_if(boxes.begin(), boxes.end(), pred2);

    MyPred pred3(tnp3);
    auto count3 = std::find_if(boxes.begin(), boxes.end(), pred3);

    MyPred pred4(tnp4);
    auto count4 = std::find_if(boxes.begin(), boxes.end(), pred4);

    MyPred pred5(tnp5);
    auto count5 = std::find_if(boxes.begin(), boxes.end(), pred5);

    MyPred pred6(tnp6);
    auto count6 = std::find_if(boxes.begin(), boxes.end(), pred6);

    MyPred pred7(tnp7);
    auto count7 = std::find_if(boxes.begin(), boxes.end(), pred7);

    MyPred pred8(tnp8);
    auto count8 = std::find_if(boxes.begin(), boxes.end(), pred8);
*/
    int rand = ControlRand(buf, tnp, tnp2, tnp3, tnp4, tnp5, tnp6, tnp7, tnp8, y, rnd);

    if (key.vk == TCODK_UP || rand == 1) {
        if (buf[tnp] != '#' && buf[tnp] != buf[y]){
            if (buf[tnp] == 'o') {
                if (buf[tnp5] != '#' && buf[tnp5] != 'o' && buf[tnp5] != buf[y]) {
                    boxes[BoxPos(boxes, tnp)]._box -= (w+1);
                    buf[tnp5] = buf[tnp];
                    buf[tnp] = buf[x];
                    buf[x] = ' ';
                    x -= (w+1);
                    for (auto a : plus) {
                        if(a == tnp5){
                            boxes[BoxPos1(boxes, plus)]._win = true;
                        }
                        if ((x+w+1) == a) {
                            buf[x+w+1] = '+';
                        }
                        if (tnp == a){
                            boxes[BoxPos(boxes, tnp5)]._win = false;
                        }
                    }
                    
                }
            }
            else  {
                buf[tnp] = buf[x];
                buf[x] = ' ';
                x -= (w+1);
                for (auto a : plus) {
                    if ((x+w+1) == a) {
                        buf[x+w+1] = '+';
                    }
                }
            }
        }
    }

    else if (key.vk == TCODK_DOWN || rand == 2) {
        if (buf[tnp2] != '#' && buf[tnp2] != buf[y]) {
            if (buf[tnp2] == 'o') {
                if (buf[tnp6] != '#' && buf[tnp6] != 'o' && buf[tnp6] != buf[y]) {
                    boxes[BoxPos(boxes, tnp2)]._box += (w + 1);
                    buf[tnp6] = buf[tnp2];
                    buf[tnp2] = buf[x];
                    buf[x] = ' ';
                    x += (w + 1);
                    for (auto a : plus) {
                        if (a == tnp6) {
                            boxes[BoxPos1(boxes, plus)]._win = true;
                        }
                        if ((x - w - 1) == a) {
                            buf[x - w - 1] = '+';
                        }
                        if (tnp2 == a) {
                            boxes[BoxPos(boxes, tnp6)]._win = false;
                        }
                    }
                }
            } else {
                buf[tnp2] = buf[x];
                buf[x] = ' ';
                x += (w + 1);
                for (auto a : plus) {
                    if ((x - w - 1) == a) {
                        buf[x - w - 1] = '+';
                    }
                }
            }
        }
    }

    else if (key.vk == TCODK_LEFT || rand == 3) {
            if (buf[tnp3] != '#' && buf[tnp3] != buf[y]) {
                if (buf[tnp3] == 'o') {
                    if (buf[tnp7] != '#' && buf[tnp7] != 'o' && buf[tnp7] != buf[y]) {
                        boxes[BoxPos(boxes, tnp3)]._box -= 1;
                        buf[tnp7] = buf[tnp3];
                        buf[tnp3] = buf[x];
                        buf[x] = ' ';
                        x -= 1;
                        for (auto a : plus) {
                            if (a == tnp7) {
                                boxes[BoxPos1(boxes, plus)]._win = true;
                            }
                            if ((x + 1) == a) {
                                buf[x + 1] = '+';
                            }
                            if (tnp3 == a) {
                                boxes[BoxPos(boxes, tnp7)]._win = false;
                            }
                        }
                    }
                } else {
                        buf[tnp3] = buf[x];
                        buf[x] = ' ';
                        x -= 1;
                        for (auto a : plus) {
                            if ((x + 1) == a) {
                                buf[x + 1] = '+';
                            }
                        }
                    }
                }
            }
    else if (key.vk == TCODK_RIGHT || rand == 4) {
                if (buf[tnp4] != '#' && buf[tnp4] != buf[y]) {
                    if (buf[tnp4] == 'o') {
                        if (buf[tnp8] != '#' && buf[tnp8] != 'o' && buf[tnp8] != buf[y]) {
                            boxes[BoxPos(boxes, tnp4)]._box += 1;
                            buf[tnp8] = buf[tnp4];
                            buf[tnp4] = buf[x];
                            buf[x] = ' ';
                            x += 1;
                            for (auto a : plus) {
                                if (a == tnp8) {
                                    boxes[BoxPos1(boxes, plus)]._win = true;
                                }
                                if ((x - 1) == a) {
                                    buf[x - 1] = '+';
                                }
                                if (tnp4 == a) {
                                    boxes[BoxPos(boxes, tnp8)]._win = false;
                                }
                            }
                        }
                    } else {
                            buf[tnp4] = buf[x];
                            buf[x] = ' ';
                            x += 1;
                            for (auto a : plus) {
                                if ((x - 1) == a) {
                                    buf[x - 1] = '+';
                                }
                            }
                        }
                    }
                }
                //turn++;
                //Score = 700-turn*10;
                /* for (int i = 6; i < (w*h+6); i++){
                 int tnp9 = buf[i];
                 MyPred pred9(buf[tnp9]);
                 auto count9 = std::find_if(boxes.begin(), boxes.end(), pred9);*/
                for (auto num = boxes.begin(); num != boxes.end(); num++) {
                    if (num->_win == false) {
                        //int tms = num->_num;
                        num->_num--;
                        //num->_num = pos[num->_num-1];
                        /*if (count9 != boxes.end()){
                            buf[i] = pos[--buf[i]-1];
                        }*/
                    }
                }
}

void MovingCl(char *buf, int x, int y, const std::vector<int>& plus, TCOD_key_t key, std::vector<Box>& boxes) {
    /*std::string strtime = std::to_string(timer / 60) + ":" + std::to_string(timer % 60);
    TCODConsole::root->print(1, 1, strtime.c_str());
    TCODConsole::root->flush();*/
    int h = std::atoi(&buf[0]);
    int w = std::atoi(&buf[3]);

    /*Box box;
    char chBox = foo(newNum, tnp);
    char chBox3 = foo(newNum, tnp3);
    char chBox5 = foo(newNum, tnp5);
    char chBox7 = foo(newNum, tnp7);
    */

    int tnp = x - w - 1;
    int tnp2 = x + w + 1;
    int tnp3 = x - 1;
    int tnp4 = x + 1;
    int tnp5 = x - w * 2 - 2;
    int tnp6 = x + w * 2 + 2;
    int tnp7 = x - 2;
    int tnp8 = x + 2;
    /*MyPred pred1(tnp);
    auto count1 = std::find_if(boxes.begin(), boxes.end(), pred1);

    MyPred pred2(tnp2);
    auto count2 = std::find_if(boxes.begin(), boxes.end(), pred2);

    MyPred pred3(tnp3);
    auto count3 = std::find_if(boxes.begin(), boxes.end(), pred3);

    MyPred pred4(tnp4);
    auto count4 = std::find_if(boxes.begin(), boxes.end(), pred4);

    MyPred pred5(tnp5);
    auto count5 = std::find_if(boxes.begin(), boxes.end(), pred5);

    MyPred pred6(tnp6);
    auto count6 = std::find_if(boxes.begin(), boxes.end(), pred6);

    MyPred pred7(tnp7);
    auto count7 = std::find_if(boxes.begin(), boxes.end(), pred7);

    MyPred pred8(tnp8);
    auto count8 = std::find_if(boxes.begin(), boxes.end(), pred8);
*/

    if (key.vk == TCODK_UP) {
        if (buf[tnp] != '#' && buf[tnp] != buf[y]){
            if (buf[tnp] == 'o') {
                if (buf[tnp5] != '#' && buf[tnp5] != 'o' && buf[tnp5] != buf[y]) {
                    boxes[BoxPos(boxes, tnp)]._box -= (w+1);
                    buf[tnp5] = buf[tnp];
                    buf[tnp] = buf[x];
                    buf[x] = ' ';
                    x -= (w+1);
                    for (auto a : plus) {
                        if(a == tnp5){
                            boxes[BoxPos1(boxes, plus)]._win = true;
                        }
                        if ((x+w+1) == a) {
                            buf[x+w+1] = '+';
                        }
                        if (tnp == a){
                            boxes[BoxPos(boxes, tnp5)]._win = false;
                        }
                    }

                }
            }
            else  {
                buf[tnp] = buf[x];
                buf[x] = ' ';
                x -= (w+1);
                for (auto a : plus) {
                    if ((x+w+1) == a) {
                        buf[x+w+1] = '+';
                    }
                }
            }
        }
    }

    else if (key.vk == TCODK_DOWN) {
        if (buf[tnp2] != '#' && buf[tnp2] != buf[y]) {
            if (buf[tnp2] == 'o') {
                if (buf[tnp6] != '#' && buf[tnp6] != 'o' && buf[tnp6] != buf[y]) {
                    boxes[BoxPos(boxes, tnp2)]._box += (w + 1);
                    buf[tnp6] = buf[tnp2];
                    buf[tnp2] = buf[x];
                    buf[x] = ' ';
                    x += (w + 1);
                    for (auto a : plus) {
                        if (a == tnp6) {
                            boxes[BoxPos1(boxes, plus)]._win = true;
                        }
                        if ((x - w - 1) == a) {
                            buf[x - w - 1] = '+';
                        }
                        if (tnp2 == a) {
                            boxes[BoxPos(boxes, tnp6)]._win = false;
                        }
                    }
                }
            } else {
                buf[tnp2] = buf[x];
                buf[x] = ' ';
                x += (w + 1);
                for (auto a : plus) {
                    if ((x - w - 1) == a) {
                        buf[x - w - 1] = '+';
                    }
                }
            }
        }
    }

    else if (key.vk == TCODK_LEFT) {
        if (buf[tnp3] != '#' && buf[tnp3] != buf[y]) {
            if (buf[tnp3] == 'o') {
                if (buf[tnp7] != '#' && buf[tnp7] != 'o' && buf[tnp7] != buf[y]) {
                    boxes[BoxPos(boxes, tnp3)]._box -= 1;
                    buf[tnp7] = buf[tnp3];
                    buf[tnp3] = buf[x];
                    buf[x] = ' ';
                    x -= 1;
                    for (auto a : plus) {
                        if (a == tnp7) {
                            boxes[BoxPos1(boxes, plus)]._win = true;
                        }
                        if ((x + 1) == a) {
                            buf[x + 1] = '+';
                        }
                        if (tnp3 == a) {
                            boxes[BoxPos(boxes, tnp7)]._win = false;
                        }
                    }
                }
            } else {
                buf[tnp3] = buf[x];
                buf[x] = ' ';
                x -= 1;
                for (auto a : plus) {
                    if ((x + 1) == a) {
                        buf[x + 1] = '+';
                    }
                }
            }
        }
    }
    else if (key.vk == TCODK_RIGHT) {
        if (buf[tnp4] != '#' && buf[tnp4] != buf[y]) {
            if (buf[tnp4] == 'o') {
                if (buf[tnp8] != '#' && buf[tnp8] != 'o' && buf[tnp8] != buf[y]) {
                    boxes[BoxPos(boxes, tnp4)]._box += 1;
                    buf[tnp8] = buf[tnp4];
                    buf[tnp4] = buf[x];
                    buf[x] = ' ';
                    x += 1;
                    for (auto a : plus) {
                        if (a == tnp8) {
                            boxes[BoxPos1(boxes, plus)]._win = true;
                        }
                        if ((x - 1) == a) {
                            buf[x - 1] = '+';
                        }
                        if (tnp4 == a) {
                            boxes[BoxPos(boxes, tnp8)]._win = false;
                        }
                    }
                }
            } else {
                buf[tnp4] = buf[x];
                buf[x] = ' ';
                x += 1;
                for (auto a : plus) {
                    if ((x - 1) == a) {
                        buf[x - 1] = '+';
                    }
                }
            }
        }
    }
    //turn++;
    //Score = 700-turn*10;
    /* for (int i = 6; i < (w*h+6); i++){
     int tnp9 = buf[i];
     MyPred pred9(buf[tnp9]);
     auto count9 = std::find_if(boxes.begin(), boxes.end(), pred9);*/
    for (auto num = boxes.begin(); num != boxes.end(); num++) {
        if (num->_win == false) {
            //int tms = num->_num;
            num->_num--;
            //num->_num = pos[num->_num-1];
            /*if (count9 != boxes.end()){
                buf[i] = pos[--buf[i]-1];
            }*/
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

void Convert(char* b, char* nb, const std::vector<Box>& boxes, char *pos){
    std::strcpy(nb, b);
    for (int i = 6; i < strlen(nb); i++){
        for (auto p : boxes){
            if (i == p._box){
                nb[i] = pos[p._num-1];
}
    }
    }
}

/*char foo(const std::vector<Box>& x, char y){
    for (auto &n : x){
        if (n._num == y){
            return n._num;
        }
    }
}
*/
int BoxPos(const std::vector<Box>& k, int t) {
    for (auto i = 0; i < k.size(); i++) {
        if (k[i]._box == t)
            return i;
    }
}

int ControlRand(char *buf, int tnp, int tnp2, int tnp3, int tnp4, int tnp5, int tnp6, int tnp7, int tnp8, int y, int rand){
    bool condition = false;
    while (condition == false){
        if (rand == 1) {
            if (buf[tnp] == '#' || (buf[tnp] == buf[y])) {
                rand = TCODRandom::getInstance()->getInt(2, 4, 0);
            } else if (buf[tnp] == 'o' && buf[tnp5] == '#') {
                rand = TCODRandom::getInstance()->getInt(2, 4, 0);
            } else if (buf[tnp] == 'o' && buf[tnp5] == 'o') {
                rand = TCODRandom::getInstance()->getInt(2, 4, 0);
            } else if (buf[tnp] == 'o' && buf[tnp5] == buf[y]) {
                rand = TCODRandom::getInstance()->getInt(2, 4, 0);
            } else {
                condition = true;
            }
        }

        if (rand == 2) {
            if (buf[tnp2] == '#' || (buf[tnp2] == buf[y])) {
                rand = TCODRandom::getInstance()->getInt(1, 4, 0);
            } else if (buf[tnp2] == 'o' && buf[tnp6] == '#') {
                rand = TCODRandom::getInstance()->getInt(1, 4, 0);
            } else if (buf[tnp2] == 'o' && buf[tnp6] == 'o') {
                rand = TCODRandom::getInstance()->getInt(1, 4, 0);
            } else if (buf[tnp2] == 'o' && buf[tnp6] == buf[y]) {
                rand = TCODRandom::getInstance()->getInt(1, 4, 0);
            } else {
                condition = true;
            }
        }

        if (rand == 3) {
            if (buf[tnp3] == '#' || (buf[tnp3] == buf[y])) {
                rand = TCODRandom::getInstance()->getInt(1, 4, 0);
            } else if (buf[tnp3] == 'o' && buf[tnp7] == '#') {
                rand = TCODRandom::getInstance()->getInt(1, 4, 0);
            } else if (buf[tnp3] == 'o' && buf[tnp7] == 'o') {
                rand = TCODRandom::getInstance()->getInt(1, 4, 0);
            } else if (buf[tnp3] == 'o' && buf[tnp7] == buf[y]) {
                rand = TCODRandom::getInstance()->getInt(1, 4, 0);
            } else {
            condition = true;
            }
        }

        if (rand == 4) {
            if (buf[tnp4] == '#' || (buf[tnp4] == buf[y])) {
                rand = TCODRandom::getInstance()->getInt(1, 3, 0);
            } else if (buf[tnp4] == 'o' && buf[tnp8] == '#') {
                rand = TCODRandom::getInstance()->getInt(1, 3, 0);
            } else if (buf[tnp4] == 'o' && buf[tnp8] == 'o') {
                rand = TCODRandom::getInstance()->getInt(1, 3, 0);
            } else if (buf[tnp4] == 'o' && buf[tnp8] == buf[y]) {
                rand = TCODRandom::getInstance()->getInt(1, 3, 0);
            } else {
                condition = true;
            }
        }
    }
    return rand;
}