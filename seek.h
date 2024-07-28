#ifndef __SEEK_H
#define __SEEK_H

#define MAX_FILE_LINES 1000
#define MAX_LINE_LENGTH 2000
int FileFind(const char* path, const struct stat * filestat, int typeflag, struct FTW * nftstruct);
void Seek(Command cmd, char* target_name, char* searchdirectory, char* invokeddir, char** pwd, int f_flag, int d_flag, int e_flag);

#endif