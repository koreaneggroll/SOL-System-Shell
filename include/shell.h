//HEADER GUARD
#ifndef SHELL_H
#define SHELL_H

#ifdef _WIN32
    #include <windows.h>
#elif __linux__
    #include <unistd.h>
#else
    #error "OS not supported..."
#endif


#include <iostream>
#include <cmath>
#include <fstream>

using namespace std;


class User{
    private:
        string name;
    public:
        User(){
            //Change this to get your name
            name = "arin";
        }

        string get_name();
            
    };

        

void bear_evaluate();
void bear_loop();

#endif //SHELL_H
//HEADER GUARD END