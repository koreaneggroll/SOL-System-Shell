#include "shell.h"


//DATA
string builtin[] = {
    "bear",
    "clear", //clears the screen
    "exit", //exits the shell
};



int (*builtin_func[])(char **){
    &bear_func,
    &bear_clear,
    &bear_exit,
};

int builtin_num(){
    return sizeof(builtin)/sizeof(string);
}


string User::get_name(){
    return name;
}


char *get_dir(){
    char *directory;
    getcwd(directory, MAX_DIR_LEN);

    return directory;
}


void bear_evaluate(){

}


void bear_main(){
    char *buffer;
    User *user = new User();
    string name = user->get_name();
    
    //Clears the screen before the shell starts
    cout << "\e[1;1H\e[2J";


    while(true){
        cout << "\x1B[33m" << user->get_name() << "\x1B[32m : \x1B[34m" << get_dir() << FRED("  ⟹   ");
        fgets(buffer, MAX_BUF_LEN, stdin);
    }
}


int bear_launch(char **args){
    pid_t pid, wpid;
    int status;

    pid = fork();
    if (pid == 0) {
        // Child process
        if (execvp(args[0], args) == -1) {
        perror("shell");
        }
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        // Error forking
        perror("shell");
    } else {
        // Parent process
        do {
        wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;

}


int bear_func(char **args){


    cout << "The following are all builtin functions of bear shell\n";

    for(int i = 0; i < builtin_num(); i++){
        cout << builtin[i] << endl;
    }

    return 1;
}


int bear_clear(char **args){
    cout << "\e[1;1H\e[2J";

    return 1;
}


int bear_exit(char **args){
    return 0;

}