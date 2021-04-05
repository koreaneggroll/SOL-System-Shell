#include "shell.h"

string User::get_name(){
    return name;
}

void bear_evaluate(){

}


void bear_main(){
    char *buffer;
    User *user = new User();
    string name = user->get_name();

    //Clears the screen before the shell starts
    clear();


    while(true){
        cout << "\x1B[33m" << user->get_name() << FRED("  ⟹   ");
        fgets(buffer, MAX_BUF_LEN, stdin);
    }
}



void clear(){
    cout << "\e[1;1H\e[2J";
}