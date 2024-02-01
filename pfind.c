#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <stdbool.h>

//Alisha Patel
//I pledge my honor that I have abided by the Stevens Honor System. 

//validating input
//verify and resolve permission string
//recursively navigate the directory tree



void navigate_dir(char* dir,char* pstring){
    //for comparing the perm_str and pstring
    bool valid = false;

    DIR* dp = opendir(dir);
    char base[1024];

    //if it's null -> stderr an error
    if (dp == NULL){
        realpath(dir, base);
        fprintf(stderr, "Error: Cannot open directory '%s%s\n",dir,"'. Permission denied.");

        
    }

    else{

        struct dirent* dirp;
        struct stat fileinfo;
        char perm_str[10] = "";


        while((dirp = readdir(dp)) != NULL){

            //to navigate the directories
            if ((!strcmp(dirp->d_name, ".")) || (!strcmp(dirp->d_name, ".."))){
                continue;
            }

            //mallocing for the length of the dir and then adding 2 (or else it won't print for sm reason)
            char* path = malloc(strlen(dir) + strlen(dirp->d_name) + 2);
            snprintf(path, strlen(dir) + strlen(dirp->d_name) + 2, "%s/%s", dir, dirp->d_name);

            //finalpath to print
            char* finalpath = realpath(path, NULL);

            stat(finalpath, &fileinfo);

        

            //if it's a regular file -> get the permission string from the file stat
            //iterates through str to get each bit and perform binary operations to get the bit
            if(S_ISREG(fileinfo.st_mode)){
                if ((fileinfo.st_mode & S_IRUSR) != 0){
                    perm_str[0] = 'r';
                }
                else{
                    perm_str[0] = '-';
                }
                if ((fileinfo.st_mode & S_IWUSR) != 0){
                    perm_str[1] = 'w';
                }
                else{
                    perm_str[1] = '-';
                }
                if ((fileinfo.st_mode & S_IXUSR) != 0){
                    perm_str[2] = 'x';
                }
                else{
                    perm_str[2] = '-';
                }
                if ((fileinfo.st_mode & S_IRGRP) != 0){
                    perm_str[3] = 'r';
                }
                else{
                    perm_str[3] = '-';
                }
                if ((fileinfo.st_mode & S_IWGRP) != 0){
                    perm_str[4] = 'w';
                }
                else{
                    perm_str[4] = '-';
                }
                if ((fileinfo.st_mode & S_IXGRP) != 0){
                    perm_str[5] = 'x';
                }
                else{
                    perm_str[5] = '-';
                }
                if ((fileinfo.st_mode & S_IROTH) != 0){
                    perm_str[6] = 'r';
                }
                else{
                    perm_str[6] = '-';
                }
                if ((fileinfo.st_mode & S_IWOTH) != 0){
                    perm_str[7] = 'w';
                }
                else{
                    perm_str[7] = '-';
                }
                if ((fileinfo.st_mode & S_IXOTH) != 0){
                    perm_str[8] = 'x';
                }
                else{
                    perm_str[8] = '-';
                }
            }

            //if the perm str matches and it's file
            if((strcmp(perm_str, pstring) == 0) && (S_ISREG(fileinfo.st_mode))){
                printf("%s\n", finalpath);
            }

            if ((S_ISDIR(fileinfo.st_mode)) && (perm_str[0] != 'r')){
                
            }

            //if a dir, then recurse again
            if (S_ISDIR(fileinfo.st_mode)){
                navigate_dir(path, pstring);
            }
            
            //free the char*
            free(path);
            free(finalpath);           

        }
    }
    closedir(dp);

}

int main(int argc, char* argv[]){
    //to track the validity 
    int pvalid = 0;

    if (argc != 3){
        fprintf(stderr, "./pfind <directory> <pstring>\n");
        exit (EXIT_FAILURE);
    }

    if (strlen(argv[2]) != 9){
        pvalid = 1;
    }

    //the second argument will be permission string
    char pstring[9];
    for(int i = 0; i < 9; i++){
        pstring[i] = argv[2][i];
    }


    //if the pstring contains any characters other than r, w, x, or - then invalid
    //the offset value measures for the placement of the characters
    for (int i = 0; i < 9; i+=3){
        if((pstring[i] != 'r') && (pstring[i] != '-')){
            pvalid = 1;
        }
        if((pstring[i+1] != 'w') && (pstring[i+1] != '-')){
            pvalid = 1;
        }
        if ((pstring[i+2] != 'x') && (pstring[i+2] != '-')){
            pvalid = 1;
        }
    }

    //return error for invalid input
    if (pvalid != 0){
        fprintf(stderr, "Error: Permissions string '%s' is invalid\n", argv[2]);
        exit (EXIT_FAILURE);
    }

    //char buf[strlen(argv[1])];
    
    //calls the navigate helper
    navigate_dir(argv[1], argv[2]);

    return 0;
}



