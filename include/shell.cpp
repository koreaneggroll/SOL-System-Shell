#include "shell.h"


//DATA

char *history[100];

string manuals[] = {
    "manual",
    "SOL",
    "cls",
    "cd",
    "fmk",
    "frm",
    "time",
    "exit"
};

string builtin[] = {
    "SOL",
    "cls", //clears the screen
    "cd", //changes directory
    "fmk", //creates a file
    "frm", //removes a file
    "time", //tells the time
    "manual", //manual for the shell
    "his", //reads the command history
    "exit" //exits the shell
};



int (*builtin_func[]) (char **) = {
    &sol_func,
    &sol_clear,
    &sol_cd,
    &sol_fmk,
    &sol_frm,
    &sol_time,
    &sol_manual,
    &sol_his,
    &sol_exit,
};



int builtin_num(){
    return sizeof(builtin)/sizeof(string);
}



int manual_num(){
    return sizeof(manuals)/sizeof(string);
}


int history_num(){
    return sizeof(history)/sizeof(string);
}


string User::get_name(){
    return name;
}



char *get_dir(){
    char *directory;
    getcwd(directory, MAX_DIR_LEN);

    return directory;
}



void sol_main(void){
    char *buffer;
    char **args;
    int status;
    int i = 0;
    User *user = new User();
    //string name = user->get_name();
    
    //Clears the screen before the shell starts
    cout << "\e[1;1H\e[2J";


    do{
        /*
        #define RST  "\x1B[0m"
        #define KRED  "\x1B[31m"
        #define KGRN  "\x1B[32m"
        #define KYEL  "\x1B[33m"
        #define KBLU  "\x1B[34m"
        #define KMAG  "\x1B[35m"
        #define KCYN  "\x1B[36m"
        #define KWHT  "\x1B[37m"
        */

        //Here you can how the prompt looks, do it anyway you like
        cout << "\x1B[33m" << user->get_name() << "\x1B[32m : \x1B[34m" << get_dir() << FRED("  ⟹   ");


        buffer = sol_read_line();

        history[i] = buffer;
        i++;
        


        args = sol_split_line(buffer);
        status = sol_execute(args);

        free(buffer);
        free(args);
    }while(status);
}


int sol_launch(char **args){
    pid_t pid, wpid;
    int status;

    pid = fork();
    if (pid == 0) {
        // Child process
        if (execvp(args[0], args) == -1) {
            cout << args[0] << ": command not found" << endl;
        }
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        // Error forking
        perror("sol");
    } else {
        // Parent process
        do {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;

}




int sol_execute(char **args){

    if (args[0] == NULL) {
        // An empty command was entered.
        return 1;
    }

    //loops through every function in the array
    for (int i = 0; i < builtin_num(); i++) {
        if (args[0] == builtin[i]) {
            return (*builtin_func[i])(args);
        }
    }

    return sol_launch(args);

}



char *sol_read_line(void){
    int bufsize = 1024;
    int position = 0;
    char *buffer = (char*)malloc(sizeof(char) * bufsize);
    int c;

    if (!buffer) {
        cout << "\n\nsol: " << strerror(errno) << "\n" << endl;
        exit(EXIT_FAILURE);
    }

    while (true) {

        c = getchar();

        if (c == EOF || c == '\n') {
            buffer[position] = '\0';
            return buffer;
        } else {
            buffer[position] = c;
        }

        position++;

        if (position >= bufsize) {
            bufsize += 1024;
            buffer = (char*)realloc(buffer, bufsize);
            if (!buffer) {
                cout << "\n\nsol: " << strerror(errno) << "\n" << endl;
                exit(EXIT_FAILURE);
            }
        }
    }
}

#define sol_TOK_DELIM " \t\r\n\a"

char **sol_split_line(char *line){
    int bufsize = 64, position = 0;
    char **tokens = (char**)malloc(bufsize * sizeof(char*));
    char *token;

    if (!tokens) {
        cout << "\n\nsol: " << strerror(errno) << "\n" << endl;
        exit(EXIT_FAILURE);
    }

    token = strtok(line, sol_TOK_DELIM);
    while (token != NULL) {
        tokens[position] = token;
        position++;

        if (position >= bufsize) {
            bufsize += 64;
            tokens = (char**)realloc(tokens, bufsize * sizeof(char*));
            if (!tokens) {
                cout << "\n\nsol: " << strerror(errno) << "\n" << endl;
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, sol_TOK_DELIM);
    }
    tokens[position] = NULL;
    return tokens;
}





int sol_func(char **args){


    cout << "The following are all builtin functions of sol shell\n";

    for(int i = 0; i < builtin_num(); i++){
        cout << "\t" << builtin[i] << endl;
    }

    return 1;
}


int sol_clear(char **args){
    cout << "\e[1;1H\e[2J";

    return 1;
}


int sol_cd(char **args){
    if (!args[1]) {
        if(!chdir("/home/") == -1){
            cout << "\nCouldn't cd to that directory\n" << endl;
            return 1; 
        }
        return 1;
    } else {
        if (chdir(args[1]) == -1) {
            cout << "\nCouldn't cd to that directory\n" << endl;
            return 1;
        }
    }
    return 1;
}



int sol_fmk(char **args){
    if(!args[1]){
        cout << "\nsol: please specify a name for the file\n" << endl;
        return 1;
    }

    FILE *fptr = fopen(args[1], "w");

    if(!fptr){
        cout << "\nsol: couldn't create file\n" << endl;
        return 1;
    }

    fclose(fptr);

    return 1;
}



int sol_frm(char **args){
    if(!args[1]){
        cout << "\nsol: please specify the name of the file you want to delete\n" << endl;
        return 1;
    }

    if(remove(args[1]) == 0){
        cout << "\nFile deleted succesfully\n" << endl;
        return 1;
    }

    else{
        cout << "\nFile couldn't be deleted\n" << endl;
        return 1;
    }


    return 1;
}



int sol_time(char **args){

    time_t my_time = time(NULL);

    cout << ctime(&my_time);

    return 1;
}


int sol_manual(char **args){
    if(!args[1]){
        cout << "What part of the manual do you want?\nFor example try `manual manual`" << endl;
        return 1;
    }
    if(strcmp(args[1], "manual") == 0){
        man_manual();
        return 1;
    }
    else if(strcmp(args[1], "SOL") == 0){
        man_SOL();
        return 1;
    }
    else if(strcmp(args[1], "cls") == 0){
        man_cls();
        return 1;
    }
    else if(strcmp(args[1], "cd") == 0){
        man_cd();
        return 1;
    }
    else if(strcmp(args[1], "fmk") == 0){
        man_fmk();
        return 1;
    }
    else if(strcmp(args[1], "frm") == 0){
        man_frm();
        return 1;
    }
    else if(strcmp(args[1], "time") == 0){
        man_time();
        return 1;
    }
    else if(strcmp(args[1], "exit") == 0){
        man_exit();
        return 1;
    }
    else{
        cout << "\nCommand not supported by manual\n" << endl;
        return 1;
    }

    return 1;
}


int sol_his(char **args){
    cout << "Your command history: ";
    
    for(int i = 0; history[i] != NULL; i++){
        cout << "\t" << history[i] << endl;
    }

    return 1;
}


int sol_exit(char **args){
    exit(0);
}




//FUNCTIONS FOR THE MANUALS
void man_manual(){
    FILE *fptr;

    fptr = fopen("manual.txt", "w");

    fprintf(fptr, "NAME\n\n\tmanual - an interface to the system refferences\n\n");
    fprintf(fptr, "USE\n\n\t> manual ");

    for(int i = 0; i < manual_num(); i++){
        fprintf(fptr, "%s/", manuals[i].c_str());
    }

    fclose(fptr);

    system("nano ./manual.txt");


    system("rm ./manual.txt");

}


void man_SOL(){
    FILE *fptr;

    fptr = fopen("SOL.txt", "w");

    fprintf(fptr, "NAME\n\n\tSOL - an interface for all commands to be shown\n\n");
    fprintf(fptr, "USE\n\n\t> SOL\n\tOutput: \n");

    for(int i = 0; i < builtin_num(); i++){
        fprintf(fptr, "\t%s\n", builtin[i].c_str());
    }

    fclose(fptr);

    system("nano ./SOL.txt");

    system("rm ./SOL.txt");

}


void man_cls(){
    FILE *fptr;

    fptr = fopen("cls.txt", "w");

    fprintf(fptr, "NAME\n\n\tcls - clears the terminal screen\n\n");
    fprintf(fptr, "USE\n\n\t> cls -> clears the screen\n\n");

    fclose(fptr);

}



void man_cd(){
    FILE *fptr;

    fptr = fopen("cd.txt", "w");

    fprintf(fptr, "NAME\n\n\tcd - changes directory\n\n");
    fprintf(fptr, "USE\n\n\t> cd [directory] -> goes into the specified directory\n\n");

    fclose(fptr);

    system("nano ./cd.txt");

    system("rm ./cd.txt");
}


void man_fmk(){
	FILE *fptr;

	fptr = fopen("fmk.txt", "w");

	fprintf(fptr, "NAME\n\n\tfmk - creates a file in the current directory\n\n");
	fprintf(fptr, "USE\n\n\t> fmk [file name.*] -> creates a file with the specified name");

	fclose(fptr);

	system("nano ./fmk.txt");

	system("rm ./fmk.txt");
}


void man_frm(){
	FILE *fptr;
	
	fptr = fopen("frm.txt", "w");

	fprintf(fptr, "NAME\n\n\tfrm - removes a file from the current directory\n\n");
	fprintf(fptr, "USE\n\n\t> frm [file name.*] -> removes the specified file if it was found");

	fclose(fptr);

	system("nano ./frm.txt");

	system("rm ./frm.txt");
}


void man_time(){
	FILE *fptr;

	fptr = fopen("time.txt", "w");

	fprintf(fptr, "NAME\n\n\ttime - tells the current date and time\n\n");
	fprintf(fptr, "USE\n\n\t> time -> returns the current date and time");

	fclose(fptr);

	system("nano ./time.txt");

	system("rm ./time.txt");
}


void man_exit(){
	FILE *fptr;

	fptr = fopen("exit.txt", "w");
	
	fprintf(fptr, "NAME\n\n\texit - exits the shell completely");
	fprintf(fptr, "USE\n\n\t> exit -> exits the program and returns to your default shell");

	fclose(fptr);

	system("nano ./exit.txt");

	system("rm ./exit.txt");
}


