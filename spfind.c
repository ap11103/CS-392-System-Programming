#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/wait.h>

//Alisha Patel
//I pledge my honor that I have abided by the Stevens Honor System.

int main(int argc, char* const argv[]){

    //number of args aren't 3
    if (argc != 3){
        fprintf(stderr, "Usage: ./spfind <directory> <permissions string> \n");
        return EXIT_FAILURE;
    }

    //error checking for the permission string or else it would try to sort the output of pfind
    for (int i = 0; i < strlen(argv[2]); i++){
        if((argv[2][i] != 'r') && (argv[2][i] != 'w') && (argv[2][i] != 'x') && (argv[2][i] != '-')){
        printf("Error: Permissions string '%s' is invalid.\n", argv[2]);
        return EXIT_FAILURE;
        }
    }

    int pfind[2];
    int sort[2];
    pid_t child1, child2;

    if (pipe(pfind) == -1){
        fprintf(stderr, "Error: pipe failed. \n");
        return EXIT_FAILURE;
    }

    if (pipe(sort) == -1){
        fprintf(stderr, "Error: sort failed. \n");
    }

    //fork the first child process for pfind
    child1 = fork();
    if (child1 < 0){
        fprintf(stderr, "Error: fork failed. \n");
        return EXIT_FAILURE;
    }
    if (child1 == 0){

        //if closing read end for pfind fails
        if (close(pfind[0]) < 0){
            fprintf(stderr, "Error: closing pfind stdin failed. \n");
            return EXIT_FAILURE;
        }

        //duplicating stdout for pfind
        if (dup2(pfind[1], fileno(stdout)) < 0){
            fprintf(stderr, "Error: dup2 failed on pfind, \n");
            return EXIT_FAILURE;
        }

        //closing read and write ports for sort
        if (close(sort[0]) < 0){
            fprintf(stderr, "Error: closing sort stdin failed. \n");
            return EXIT_FAILURE;
        }
        if (close(sort[1]) < 0){
            fprintf(stderr, "Error: closing sort stdout failed. \n");
            return EXIT_FAILURE;
        }

        //executing the pfind
        if(execvp("./pfind", argv) < 0){
            fprintf(stderr, "Error: pfind failed. \n");
            return EXIT_FAILURE;
        }
        
    }

    //wait for the first child to finish
    if(wait(NULL) < 0){
        fprintf(stderr, "Error: wait() failed. \n");
        return EXIT_FAILURE;
    }

    //fork second child process for sorting
    child2 = fork();

    if (child2 < 0){
        fprintf(stderr, "Error: fork failed. \n");
        return EXIT_FAILURE;
    }

    //redirect output to sort, and read from pfind
    if (child2 == 0){

        //close write end of pfind and read end of sort
        if (close(pfind[1]) < 0){
            fprintf(stderr, "Error: close pfind stdin failed. \n");
            return EXIT_FAILURE;
        }

        //need to redirect stdin to read the input from pfind pipe
        if (dup2(pfind[0], fileno(stdin)) < 0){
            fprintf(stderr, "Error: dup2 failed on pfind. \n");
            return EXIT_FAILURE;
        }

        if (close(sort[0]) < 0){
            fprintf(stderr, "Error: close sort stdin failed. \n");
            return EXIT_FAILURE;
        }

        //need to redirect stdout to write to sort pipe
        if (dup2(sort[1], fileno(stdout)) < 0){
            fprintf(stderr, "Error: dup2 failed on sort. \n");
            return EXIT_FAILURE;
        }

        //calling sort for sorting the output
        if (execlp("sort", "sort", NULL) < 0){
            fprintf(stderr, "Error: sort failed. \n");
            return EXIT_FAILURE;
        }

        exit(EXIT_SUCCESS);
    }

    
    //closing the all the unnecessary file ends
    if(close(sort[1]) < 0){
        fprintf(stderr, "Error: close failed. \n");
        return EXIT_FAILURE;
    }

    if(close(pfind[0]) < 0){
        fprintf(stderr, "Error: close failed. \n");
        return EXIT_FAILURE;
    }
    
    if(close(pfind[1]) < 0){
        fprintf(stderr, "Error: close failed. \n");
        return EXIT_FAILURE;
    }

    //waiting for the child in the parent
    
    if(wait(NULL) < 0){
        fprintf(stderr, "Error: wait() failed. \n");
        return EXIT_FAILURE;
    }
  
    // if (child1 > 0){
    //     if (wait(NULL) < 0){
    //         fprintf(stderr, "Error: wait() failed. \n");
    //         return EXIT_FAILURE;
    //     }
    // } 

    // if(child2 > 0){
    //     if(wait(NULL) < 0){
    //         fprintf(stderr, "Error: wait() failed. \n");
    //         return EXIT_FAILURE;
    //     }
    // }

    //read from the output of sort arr, and then print to stdout
    int counter  = 0;
    char c;
    //for error checking read
    size_t nbytes;

    while((nbytes = read(sort[0], &c, sizeof(c))) != 0){

        if(nbytes > 0){
            printf("%c", c);
            if(c == '\n'){
                counter++;
            }
        }
        else{
            break;
        }
    }

    //close the read end of sort, so all the ends are closed
    if(close(sort[0]) < 0){
        fprintf(stderr, "Error: close failed. \n");
        return EXIT_FAILURE;
    }

    //print the total matches
    printf("Total matches: %d\n", counter);

    //while(wait(NULL) > 0);
    
    return EXIT_SUCCESS;
        
}

