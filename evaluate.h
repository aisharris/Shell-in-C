#ifndef __EVALUATE_H
#define __EVALUATE_H

#define MAX_FILE_NAME 200
#define MAX_ENTRIES 100

void Peek(char* path, int a_flag, int l_flag);
void Warp(char* dest, char* invokeddir, char** pwd);
void PrintFileData(int fd, char* path);
 
#endif
