#include "shell.h"


//DATA
const char *builtin[] = {
    "bear",
    "clear", //clears the screen
    "exit" //exits the shell
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
    char **args;
    int status;
    User *user = new User();
    string name = user->get_name();
    
    //Clears the screen before the shell starts
    cout << "\e[1;1H\e[2J";


    do{
        cout << "\x1B[33m" << user->get_name() << "\x1B[32m : \x1B[34m" << get_dir() << FRED("  ⟹   ");
        args = bear_split_line(buffer);
        status = bear_execute(args);

        free(buffer);
        free(args);
    }while(status);
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



int bear_execute(char **args){
      int i;

    if (args[0] == NULL) {
        // An empty command was entered.
        return 1;
    }

    //loops through every function in the array
    for (i = 0; i < builtin_num(); i++) {
        if (strcmp(args[0], builtin[i]) == 0) {
            return (*builtin_func[i])(args);
        }
    }

    return bear_launch(args);

}



char *bear_read_line(void){
  int bufsize = 1024;
  int position = 0;
  char *buffer = (char*)malloc(sizeof(char) * bufsize);
  int c;

  if (!buffer) {
    fprintf(stderr, "\n\nshell: %s\n\n", strerror(errno));
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
        fprintf(stderr, "\n\nshell: %s\n\n", strerror(errno));
        exit(EXIT_FAILURE);
      }
    }
  }
}



char **bear_split_line(char *line){
  int bufsize = 64, position = 0;
  char **tokens = (char**)malloc(bufsize * sizeof(char*));
  char *token;

  if (!tokens) {
    fprintf(stderr, "\n\nshell: %s\n\n", strerror(errno));
    exit(EXIT_FAILURE);
  }

  token = strtok(line, " \t\r\n\a");
  while (token != NULL) {
    tokens[position] = token;
    position++;

    if (position >= bufsize) {
      bufsize += 64;
      tokens = (char**)realloc(tokens, bufsize * sizeof(char*));
      if (!tokens) {
        fprintf(stderr, "\n\nshell: %s\n\n", strerror(errno));
        exit(EXIT_FAILURE);
      }
    }

    token = strtok(NULL, " \t\r\n\a");
  }
  tokens[position] = NULL;
  return tokens;
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