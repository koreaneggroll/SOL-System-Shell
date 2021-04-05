#include "../include/shell.h"


int main(void){
    User *user = new User();

    cout << user->get_name() << endl;
}