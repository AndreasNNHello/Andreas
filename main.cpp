#include "libtcod.hpp"
#include "libtcod.h"
#include "main.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <string>


TCOD_console_t TCOD_console_from_file(const char *filename);
/*Class Point{
    int x,y;
    void Po(asd x, sad y){
        return ;
    }
};*/

template <typename T> class Field {
public:
    Field (T* w, T* h, std::vector<T*> a)
{
    width = w;
    height = h;
    std::vector<T>arr = a;
};

~Field();

Field dsf();
Field fas();

private:
    T width, height;
    std::vector<T>a;
};


//void TCODConsole::setWindowTitle(const char *title){}
//bool TCODConsole::isWindowClosed(){}
//void TCODConsole::setDefaultBackground(TCODColor back){}
//void TCODConsole::setDefaultForeground(TCODColor fore) {}
//TCODConsole::printRect(int x,int y, int w, int h, char){}
class T{

};

int main(){
    char buf[50];
    std::ifstream loc("field.txt(path)");
    // takes string from File and compute value w,h
    loc.getline(buf,50);
    std::vector<T> arr;
    int h = std::string::length(buf);
    int w = (int)(arr / h);
    // push charac.. into vector
    for (auto n = arr.begin(); n != arr.end(); n++){
        for(auto i = 0; i < w; i++){
            for(auto j = 0; j < h; j++){
                  arr.push_back();
            }
        }
    //return std::vector<T> arr;
    };


    for (auto p:arr){

    }
    //arr.emplace(arr.begin(), (char*)"#")
    std::cout<< arr.capacity() <<std::endl;
    TCOD_console_init_root(50,50,"Hello, players!",false, TCOD_RENDERER_OPENGL);
    TCODSystem::setFps(25);
    //TCOD_console_from_file("home/andreas/CLionProjects/game_proj/Field.txt");

    //TCOD_console_get_char_foreground(30, 30);
    //TCODConsole::setWindowTitle(Game_of_hell);
    //TCODConsole::root->setDefaultBackground(TCODColor::green);
    //TCODConsole::root->setDefaultForeground(TCODColor::red);
    //void TCOD_console_rect(30,30,20,20, true, TCOD_BKGND_BURN);
    //TCOD_console_print_frame(30,30,20, true, TCOD_BKGND_BURN);
    //TCOD_console_t *ptr = CON_W;
    //Field::TCOD_console_set_default_background(ptr,);
    while(1){
        TCODConsole::flush();
        //TCOD_key_t key = TCOD_console_wait_for_keypress('k');
    }

    return 0;
    }
