#include "stdiox.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <limits.h>
#define PATH_MAX 4096

//#include <stdio.h>

//Alisha Patel
//I pledge my honor that I have abided by the Stevens Honor System.


//to convert integer to string
char* toString(int value, char* buff){
 
    //works with the absoluate value
    int n = abs(value);
 
    int i = 0;
    while (n)
    {
        int mod = n%10;
        if (mod >= 10) {
            buff[i++] = 65 + (mod- 10);
        }
        else {
            buff[i++] = 48 + mod;
        }
        n =(n/10);
    }
    // if the number is 0
    if (i == 0) {
        buff[i++] = '0';
    }
    // If the value is negative, the string starts with -
    if (value < 0) {
        buff[i++] = '-';
    }
 
    buff[i] = '\0'; // null terminate string
 
    // reverse the string and return it
    //return reverse of string by swapping
    char temp;
    for(int j = 0; j < strlen(buff)-1; j++){
        for(int k = j+1; k < strlen(buff); k++){

            temp = buff[j];
            buff[j] = buff[k];
            buff[k] = temp;
        }
    }
    return buff;
    
}

int fprintfx(char* filename, char format, void* data){

    //to keep track of fd
    int fd;
    
    struct stat info;
    stat(filename, &info);

    if (data == NULL){
        errno = EIO;
        return -1;
    }

    if (strcmp(filename, "") == 0){
        //write to the terminal
        fd = 1;

        //if it's integers..
        if (format == 'd'){

            int d = *(int*) data;
            char buffer[sizeof(int)];

            //turning it to string and then writing it
            char* dstr = toString(d, buffer);
            write(fd, dstr, strlen(dstr));
            char null = '\n';
            write(fd, &null, 1);
        }

        else if (format == 's'){
            //char str = *(char*) data;
            write(fd, data, strlen(data));
            char null = '\n';
            write(fd, &null, 1);
        }

        else{
            errno = EIO;
            return -1;
        }

    }

    //if writing to a file
    else{

        char tempath[PATH_MAX];
        realpath(filename, tempath);

        //for file with rw-r-----
        fd = open(tempath, O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP);
        
        if (format == 'd'){
            int d = *(int*) data;
            char buffer[sizeof(int)];

            char* dstr = toString(d, buffer);
            write(fd, dstr, strlen(dstr));
            char null = '\n';
            write(fd, &null, 1);
        }

        else if (format == 's'){
            //char str = *(char*) data;
            write(fd, data, strlen(data));
            char null = '\n';
            write(fd, &null, 1);
        }

        else{
            errno = EIO;
            return -1;
        }

    }

    return 0;
}



//checks if the file is open for not
int is_open(char* fpath){

    struct stat info;
    struct stat finode;
    struct dirent* dirp;
    char path[PATH_MAX]="/proc/self/fd/";

    stat(fpath,&info);
    int inode= info.st_ino;
    DIR* dp=opendir(path);
    char tempath[PATH_MAX];

    // if ((realpath(path,tempath)) == NULL){
    //     //closedir(dp);
    //     //errno = ENOENT;
    //     return -1;     
    // }


    
    while((dirp=readdir(dp))!=NULL){
        if(strcmp(dirp->d_name,".")!=0 && strcmp(dirp->d_name,"..")!=0){
            char tmp [PATH_MAX]; 
            strcpy(tmp, path); 
            strcat(tmp, dirp->d_name);
            char tmp2[PATH_MAX];
            realpath(tmp,tmp2);
            stat(tmp2, &finode);
            
            if(info.st_ino == finode.st_ino){
                int fd= atoi(dirp->d_name);
                closedir(dp);
                return fd;
            }
        }
    }
    closedir(dp);
    return -1;
    
}


    



int fscanfx(char* filename, char format, void* dst){
    
    int fd;

    if(dst == NULL){
        return 0;
    }

    //if file is specified
    if (strcmp(filename, "") == 0){
        fd = 0;
    }
    else if (access(filename, F_OK) == -1){
        errno = ENOENT;
        return -1;
    }
    //char tempath[PATH_MAX];
    else{

        if((fd = is_open(realpath(filename, NULL))) == -1){
            fd = open(realpath(filename, NULL), O_RDONLY);
        }
        

        //if the file is not open or not a good fd
       
              
    }
    //if the file is not specified, then read from keyboard
    

    int size = 128;
    int i = 0;
    char* buff = (char*)malloc(128);
    char c;

    memset(buff, 0, size);

    int x = 1;


    // off_t start = lseek(fd, 0, SEEK_CUR);
    // off_t end = lseek(fd, 0, SEEK_END);

    // if(start == end){
    //     return -1;

    // }
    // lseek(fd, start, SEEK_SET);
    

    if(fd != 0){
        off_t start = lseek(fd, 0, SEEK_CUR);
        off_t end = lseek(fd, 0, SEEK_END);

        if(start == end){
            return -1;

        }
        lseek(fd, start, SEEK_SET);

    }

    int read_o;

    while((read_o = read(fd, &c, 1)) != 0){
        //int read_o = read(fd, &c, 1);
        if(read_o < 0){
            free(buff);
            errno = EIO;
            return -1;
        }

        if(read_o == 0){
            break;
        }

        if (i >= size){
            size+=128;
            buff = (char*)realloc(buff, size);
        }

        if(c == '\n' || c == '\0'){
            buff[i] = '\0';
            i++;
            break;
        }
        buff[i] = c;
        i++;
        
    }

    if (i == 0 && fd  == 0 || i == 0){
        free(buff);
        return -1;
    }

    
    if (format == 's'){
        // for (int i = 0; i < strlen(buff); i++){
        //     // if (buff[i] == '\0'){
        //     //     buff[i] = '\n';
        //     // }
        // }
        strcpy(dst, buff);
        //free(buff);
    }
    else if(format == 'd'){
        int d = (atoi(buff));
        *((int*)dst) = d;   
    }
    else{
        //free(buff);
        errno = EIO;
        return -1;
    }
    free(buff);
    
    
    return 0;
    
}


int clean(){
    char path[PATH_MAX] = "/proc/self/fd/";
    DIR* dp = opendir(path);
    struct dirent* dirp;
    int fd;

    if((dirp=readdir(dp)) == NULL){
        errno = EIO;
        return -1;
    }

    while((dirp=readdir(dp))!=NULL){        
        if (strcmp(dirp->d_name, ".") == 0 || strcmp(dirp->d_name, "..") == 0){
            continue;
        }
        fd = atoi(dirp->d_name);
        if (fd < 3 || fd >= 1024){
             continue;
        }
        if(close(fd) == -1){
            closedir(dp);
            errno = EIO;
            return -1;
        }
    }
    closedir(dp);
    return 0;
}




