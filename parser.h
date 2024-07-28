#ifndef __PARSER_H
#define __PARSER_H

#define MAX_ARGS 20
#define MAX_ARG_LEN 50

struct Command {
    char* command;
    char** args;
    int ground;
    int redirect; 
    int index;
    int argcount;
};

typedef struct Command* Command;

void Parse(char** strings, int* count, char* parsefilepath, char* invokeddir, char** pwd, int* timetaken, char* process);
void EvaluateCmd(char** strings, int* count, char* parsefilepath, Command cmd,  int flag, char* invokeddir, char** pwd, int numargs, int* timetaken, char* process, int redirect_flag, char* input_file, char* output_file); //flag = 1 means & run in bg, flag = -1 means ; just run
void SpecialParse(int flag, char** strings, int* count, char* parsefilepath, char* invokeddir, char** pwd, int* timetaken, char* process, int pipe, int red);

#endif