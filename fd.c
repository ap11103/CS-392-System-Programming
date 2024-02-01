#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main1(){
    //gets the current working directory
    //automatically allocates some space; so free it
    char* temp;
    if (temp == getcwd(NULL, 0)){
        perror("getcwd");
        exit(1);
    }

    puts(temp);
    free(temp);
    exit(0);
   
}

//-----------------------------------------------------------------------------

int main2(){

    //suppose we closed the standard input file descriptor
    fclose(stdout);
    char c = getchar();

    FILE* fp = fopen("1.txt", "w");
    c = getchar();
    stdout = fopen("/dev/tty", "r");
    c = getchar();

    //if done "cd proc" -> it will show all the proccess
    //if done "cd fd" -> it will show all the open fd under the current process
    //open it in ll
    //the new file descriptor will always open to the lowest integer possible

}

#include <fcntl.h>

int main3(){
    FILE* fp = fopen("path", "w");
    //path write only -> macro: WRONLY
    //O_RDONLY: read only; O_RDWR: read/write
    //O_CREAT: to create a file; O_APPEND: to append to the file
    int fd = open("path", O_WRONLY);

    if (fd < 0){
        //if fd is less than 0, because fd has to be a non-negative number
        //it won't open successfully
        perror(stdout);
    }

    //relative open: first open a directory (has a file descriptor)
    int dirfd = open("/usr/sh/", O_RDWR);
    //and then create a fd
    int fd = openat(dirfd, "myfile", O_RDWR);

    //open() and close()
    // -> file descriptors and system calls fcntl.h


    //fopen and fclose()
    // -> FILE*; C I/O library high level I/O; call open() and close()


}

//----------------------------------------------------------------------------------------

int main4(){
    //fd using open()
    //to create one if doesn't exist -> | O_CREATE
    int fd = open("filename", O_WRONLY | O_CREAT);

    //if no such file or directory
    //perror("open"); 

    char str[] = "hellohello";

    //want to read 5 bytes
    //so it would write hello
    //if done cat filename -> hello
    write(fd, str, 5);
    //this would write hellohello
    write(fd, str, 10);
    //if done cat filename -> hellohellohello
    //if the file exists, it will append the characters to the end of the file

    //if the int fd = open("filename", O_WRONLY | O_CREAT | O_TRUNC)
    //it will rewrite the file instead of appending at the end of the file

    int fd2 = open("filename2", O_RDWR);

    char src[] = "12345";
    char dst[100] = "";
    read(fd, dst, 3);
    write(fd, dst, 3);
    //it will read HelHel, where the file contents of Hello!
    //f_pos == 0; which will start at H; and it will put Hel to dst
    //then f_pos == 3; then again when you call write, it will write HelHel
    //and then it update the f_pos == 6

    char str2[] = "hello world!\n";
    write(1, str, 13);
    //it will stdout hello world! because fd = 1

    read(0, str, 5);
    //it will take in input which is <= 5 characters, it will output command not found
    //read(0, str, );
    //if you don't know how many characters will it take in?




    close(fd);
}







