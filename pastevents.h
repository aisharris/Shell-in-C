#ifndef __PASTEVENTS_H
#define __PASTEVENTS_H

#define MAX_LINES 16

void AddPastEvent(char** input, int count, char** laststrings, int lastcount, char* abs_path);
void FindPastEvent(char** strings, int* count, Command cmd, int address, char* abs_path, char* invokeddir, char** pwd, int* timetaken, char* process);
void PastPrint(char* path);
void PastPurge(char* path);


#endif 