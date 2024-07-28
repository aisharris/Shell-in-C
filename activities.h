#ifndef __ACTIVITIES_H
#define __ACTIVITIES_H

struct ProcInfo {
    int pid;
    char* cmd_name;
    char* state;
};

typedef struct ProcInfo * ProcInfo;
void activities();


#endif