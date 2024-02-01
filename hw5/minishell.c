#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <limits.h>
#define BLUE "\x1b[34;1m"
#define DEFAULT "\x1b[0m"
#define PATH_MAX 4096

//Alisha Patel
//I pledge my honor that I have abided by the Stevens Honor System. 



int main(int argc, char* argv[]){

    while(1){

        char path[PATH_MAX];
        getcwd(path, PATH_MAX);
        //error check path
        if (getcwd(path, PATH_MAX) == NULL) {
            fprintf(stderr, "Error: Cannot get current working directory. %s.\n", strerror(errno));
        }
        else {
            printf("[%s%s%s]> ", BLUE, path, DEFAULT);
        }


        //if the command is cd
        if (argv[0] == "cd"){
            struct passwd* pw;
            pw = getpwuid(getuid());

            if (argv[1] == "~" || argv[1] == NULL){
                if (pw == NULL){
                    fprintf(stderr, "Cannot get passwd entry. %s.\n", strerror(errno));
                    memset(argv, 0, strlen(argv));
                    continue;
                }
                else{
                    chdir(pw->pw_dir);
                }
            }
            else if (argv[2] != NULL){
                fprintf(stderr, "Too many arguments to cd.\n", strerror(errno));
                memset(argv, 0, strlen(argv));
                continue;
            }

            else{
                if (chdir(argv[1]) == -1){
                    fprintf(stderr, "Error: Cannot change directory to %s. %s.\n", sterror(errno));
                    memset(argv, 0, strlen(argv));
                    continue;

                }
            }
        }
        else if (argv[0] == "exit"){
            exit(EXIT_SUCCESS);
        }
        else{

        }
    }

}

