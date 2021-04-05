//HEADER GUARD
#ifndef SHELL_H
#define SHELL_H


//CHECKS OPERATING SYSTEM
#ifdef _WIN32
    #include <windows.h>
#elif __linux__
    #include <unistd.h>
#else
    #error "OS not supported..."
#endif

//INCLUDES
#include <iostream>
#include <cmath>
#include <fstream>
#include <filesystem>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include "colors.h"


//DEFINES
#define MAX_BUF_LEN 200
#define MAX_DIR_LEN 200


//namespace
using namespace std;




class User{
    private:
        string name;
    public:
        User(){
            //Change this to get your name in the shell
            name = "arin";
        }

        string get_name();
            
    };

        
char *get_dir();
void bear_evaluate();
void bear_main();
int bear_launch(char **args);
int bear_execute(char **args);
char *bear_read_line(void);
char **bear_split_line(char *line);


//BUILTIN FUNCTIONS
int bear_func(char **args);
int bear_clear(char **args);
int bear_exit(char **args);

#endif //SHELL_H
//HEADER GUARD END