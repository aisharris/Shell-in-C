#ifndef __TOKENIZE_H
#define __TOKENIZE_H

//#define __USE_ATFILE
//#define _DEFAULT_SOURCE

// if ndef 

#define MAX_INPUT_LENGTH 200
#define MAX_INPUT_WORDS 50
#define MAX_WORD_LENGTH 200

/*
split up input string into words or special symbols, ignore weird spaces and tabs, and store it in an array of strings
*/

void AddToStringsWithoutThisChar(char** strings, char* input, int last_added, int index, int i, int* count);

void AddToStringsWithThisChar(char** strings, char* input, int last_added, int index, int i, int* count);

int OutsideQuotes(char** strings, char* input, int* last_added, int* index, int* i, int* len, int* count);

void SpaceorTab(char** strings, char* input, int* last_added, int* index, int* i, int* count);

int SpecialCharaters(char** strings, char* input, int* last_added, int* index, int* i, int* len, int* count);

int LastLetterandAmpersand(char** strings, char* input, int* last_added, int* index, int* i, int* len, int* count);


char** tokenize(char* input, int* count);

#endif