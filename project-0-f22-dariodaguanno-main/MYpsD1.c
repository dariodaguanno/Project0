//
// Created by dario on 9/8/2022.
//
//The first delivery is simply to get arguments through linux and use getopts()

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>
#include <stdbool.h>

//parse arguments
struct opts parseArgs(int argc, char *argv[]);

//Using a struct seems to be the best way to hold arguments
struct opts{
    char *pid;
    bool state;
    bool time;
    bool sysTime;
    bool virtMem;
    bool cmdLine;
};
//main will use the getopt() function to parse the arguments passed to it. It will look for -p with
//a PID number, -s, -U, -S, -V, -C
int main(int argc, char *argv[]){
    struct opts thestruct = parseArgs(argc, argv);
    printf("The PID is %s\n", thestruct.pid);
    return 0;
}

struct opts parseArgs(int argc, char *argv[]){
    struct opts MYstruct;
    int optNum;
    MYstruct.pid = NULL;
    MYstruct.state = false;
    MYstruct.time = true;
    MYstruct.sysTime = false;
    MYstruct.virtMem = false;
    MYstruct.cmdLine = true;
    while((optNum = getopt(argc, argv, ":-p:-s-U-S-V-C")) != -1){
        switch(optNum) {
            case 'p':
                printf("you have chose p\n");
                MYstruct.pid = optarg;
                break;
            case 's':
                printf("you have chose s\n");
                MYstruct.state = true;
                break;
            case 'U':
                printf("you have chose U\n");
                MYstruct.time = false;
                break;
            case 'S':
                printf("you have chose S\n");
                MYstruct.sysTime = true;
                break;
            case 'V':
                printf("you have chose V\n");
                MYstruct.virtMem = true;
                break;
            case 'C':
                printf("you have chose C\n");
                MYstruct.cmdLine = false;
                break;
            case '?':
                printf("No such argument %c\n", optopt);
                break;
        }
    }
    return MYstruct;
}

