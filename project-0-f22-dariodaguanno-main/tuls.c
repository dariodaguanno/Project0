//
// Created by Dario Daguanno on 8/24/2022.
//tuls.c will take 0 directory paths, 1 directory path or multiple directory paths and print the files
//in that directory path. if 0 paths are given, it will print the files in the current directory.
//If there is a directory in the given directory, it will recursively print the child directory files
//How to distinguish between file and directory*****
//INODE(7) man states: "to test for a regular file... one could write
//stat(pathname, &sb);
//if((sb.st_mode & S_IFMT) == S_IFREG){
//  *handle the file*
//}

//Eventually we will print out using dir->d_name from the dirent struct

#define _GNU_SOURCE

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
//Adding important libraries

//dirent man shows I should prob use this: DIR *opendir(const char *);
/*
DIR *d;//declare directory stream
struct dirent **dir;
struct stat fileStat;
*/
void recursive_dir(int indent, char *pathName);//prototype for the recursive

int main(int argc/*argument count*/, char**argv/*array of the whole argument*/) {

    DIR *d;//declare directory stream
    struct dirent **dir;
    struct stat fileStat;

    //printf("1st arg is %s\n2nd arg is %s\n", argv[0], argv[1]);
    //Tested that args were read correctly

    int numberOfDir;

    if(argc==1){
        d = opendir(".");
        if(d == NULL){
            printf("tuls: cannot open directory\n");
            exit(1);
        }

        numberOfDir = scandir(".", &dir, NULL, alphasort);//Cannot compile in CLion, but works in UBUNTU
        if(numberOfDir == -1){
            printf("tuls: cannot scan directory\n");
            exit(1);
        }

        printf("**********************************************************\n");
        printf("[[Current Directory]]\n");

        //printf("Total number of directories and files in this directory: %d\n", numberOfDir);
        //Check to see if all directories and folders were scanned
        while(numberOfDir--) {
            if(strcmp(dir[numberOfDir]->d_name, ".") == 0){
                printf(".\n");
                return 0;
            }
            else if(strcmp(dir[numberOfDir]->d_name, "..") == 0){
                printf("..\n");
                continue;
            }
            else {
                printf("%s\n", dir[numberOfDir]->d_name);
                stat(dir[numberOfDir]->d_name, &fileStat);

                if (S_ISDIR(fileStat.st_mode)) {
                    //printf(" This is a directory\n");
                    //do directory recursive print
                    recursive_dir(1, dir[numberOfDir]->d_name);
                }
                else {
                    //printf(" This is a file\n");
                }
            }
        }
        int success = closedir(d);
        if(success == -1){
            printf("tuls: cannot close directory\n");
            exit(1);
        }
    }
    else if(argc==2){

        d = opendir(argv[1]);
        if(d == NULL){
            printf("tuls: cannot open directory\n");
            exit(1);
        }

        numberOfDir = scandir(argv[1], &dir, NULL, alphasort);//Cannot compile in CLion, but works in UBUNTU
        if(numberOfDir == -1){
            printf("tuls: cannot scan directory\n");
            exit(1);
        }

        printf("**********************************************************\n");
        printf("[[%s]]\n",argv[1]);

        while(numberOfDir--) {
            if(strcmp(dir[numberOfDir]->d_name, ".") == 0){
                printf(".\n");
                return 0;
            }
            else if(strcmp(dir[numberOfDir]->d_name, "..") == 0){
                printf("..\n");
                continue;
            }
            else {
                printf("%s\n", dir[numberOfDir]->d_name);

                char pathCopy[PATH_MAX];
                strcpy(pathCopy, argv[1]);
                strcat(pathCopy, dir[numberOfDir]->d_name);

                stat(pathCopy, &fileStat);

                if (S_ISDIR(fileStat.st_mode)) {
                    //printf(" This is a directory\n");
                    //do directory recursive print
                    recursive_dir(1, pathCopy);
                }
                else{
                    //printf(" This is a file\n");
                }
            }
        }
    }

    else{
        for(int i=1; i<argc; i++){
            d = opendir(argv[i]);
            if(d == NULL){
                printf("tuls: cannot open directory\n");
                exit(1);
            }

            numberOfDir = scandir(argv[i], &dir, NULL, alphasort);//Cannot compile in CLion, but works in UBUNTU

            if(numberOfDir == -1){
                printf("tuls: cannot scan directory\n");
                exit(1);
            }

            printf("***************************************************************\n");
            printf("[[%s]]\n", argv[i]);

            while(numberOfDir--){
                if(strcmp(dir[numberOfDir]->d_name, ".") == 0){
                    printf(".\n");
                    continue;
                }
                else if(strcmp(dir[numberOfDir]->d_name, "..") == 0){
                    printf("..\n");
                    continue;
                }
                else {
                    printf("%s\n", dir[numberOfDir]->d_name);

                    char pathCopy[PATH_MAX];
                    strcpy(pathCopy, argv[i]);
                    strcat(pathCopy, dir[numberOfDir]->d_name);

                    stat(pathCopy, &fileStat);

                    if (S_ISDIR(fileStat.st_mode)) {
                        //printf(" This is a directory\n");
                        //do directory recursive print
                        recursive_dir(1, pathCopy);
                    }
                    else{
                        //printf(" This is a file\n");
                    }
                }
            }
        }
    }
    return 0;
}

void recursive_dir(int indent, char *pathName){
    indent = indent*2;
    DIR *d2;//declare directory stream
    struct dirent *dir2;

    d2 = opendir(pathName);
    if(d2 == NULL){
        printf("tuls: cannot open directory\n");
        exit(1);
    }

    while((dir2=readdir(d2))){

        if(strcmp(dir2->d_name, ".") == 0){
            continue;
        }

        else if(strcmp(dir2->d_name, "..") == 0){
            continue;
        }

        else {
            for(int j = 0; j<indent; j++){
                printf(" ");
            }
            printf("->%s\n", dir2->d_name);

            if ((dir2->d_type) == DT_DIR) {
                //printf("  This is a directory\n");
                //do directory recursive print
                char pathCopy[PATH_MAX];
                char slash[PATH_MAX] = "/";
                char currDir[PATH_MAX];

                getcwd(currDir, PATH_MAX);

                strcpy(pathCopy, currDir);
                strcat(pathCopy, slash);
                strcat(pathCopy, pathName);
                strcat(pathCopy, slash);
                strcat(pathCopy, dir2->d_name);
                strcat(pathCopy, slash);

                recursive_dir(indent+1, pathCopy);
            }
            else {
                //printf("  This is a file\n");
            }
        }
    }
    rewinddir(d2);
    int success2 = closedir(d2);
    if(success2 == -1){
        printf("tuls: cannot close directory\n");
        exit(1);
    }
    printf("\n");
}
