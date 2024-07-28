#include "headers.h"

char* nametarget;
char* sdirectory;

int fflag;
int dflag;
int eflag;

int filecount;
char firstfile[MAX_FILE_NAME];

//DIDNT COLOR

int FileFind(const char* path, const struct stat * filestat, int typeflag, struct FTW * nftstruct)
{
    //printf("In filefind path: %s\n", path);
    int mydirlen = strlen(sdirectory);
    //printf("strcmp: %d\n", strncmp(nametarget, path + nftstruct->base, sizeof(nametarget)));
    if (strncmp(nametarget, path + nftstruct->base, strlen(nametarget)) == 0) 
    {
        //printf("%s\n", path + nftstruct->base);
        // printf("%s\n", nametarget);
        // printf("name length: %ld\n", strlen(nametarget));
        int curlen = strlen(path);
        // printf("mydirlen: %d\n", mydirlen);
        // printf("curlen: %d\n", curlen);
        if (filecount == 0) 
        {
            firstfile[0] = '\0';
            strcat(firstfile, path);
            //printf("first files path: %s\n", firstfilefound);
        }
        filecount++;

        if(!S_ISDIR(filestat->st_mode) && dflag == 0) //file, check d flag 0 COLOR GREEN
        {
            printf(GREEN"."reset);

            printf(GREEN"%s"reset, (path)+mydirlen);
        
            printf(GREEN"\n"reset);
        }
        if(S_ISDIR(filestat->st_mode) && fflag == 0 && nftstruct->level != 0) //dir, check f flag 0 COLOR BLUE
        {
            printf(BLUE"."reset);
            
            printf(BLUE"%s"reset, (path)+mydirlen);
            
            printf(BLUE"\n"reset);
        }
        
    }
    return 0;
}

void Seek(Command cmd, char* target_name, char* searchdirectory, char* invokeddir, char** pwd, int f_flag, int d_flag, int e_flag)
{

    nametarget = target_name;
    sdirectory = searchdirectory;

    fflag = f_flag;
    dflag = d_flag;
    eflag = e_flag;

    filecount = 0;

    if (nftw(searchdirectory, FileFind, 10, FTW_PHYS | FTW_ACTIONRETVAL) == -1) 
    {
        perror(RED"nftw"reset);
    }

    if(filecount == 0)
    {
        fprintf(stderr, RED"No match found!\n"reset);
        return; 
    }

    //printf("%d\n", filecount);

    if(filecount == 1 && eflag == 1)
    {
        struct stat* fstat = malloc(sizeof(struct stat));
        stat(firstfile, fstat);

        
        if(!S_ISDIR(fstat->st_mode) && dflag == 1) //file, check d flag 0 COLOR GREEN
        {
            fprintf(stderr, RED"No matches found!\n"reset);
            return;
        }
        else if(S_ISREG(fstat->st_mode)) //execute file
        {
            
            //printf("HI\n");
            FILE* fileptr = fopen(firstfile, "r");
            if(fileptr == NULL)
            {
                fprintf(stderr, RED"Missing permissions for task!\n"reset);
                return;
            }

            char *line = NULL;
            size_t len = 0;
            ssize_t read;

            while(read = getline(&line, &len, fileptr) != -1)
            {
                printf("%s", line);
            }

        }
        else if(S_ISDIR(fstat->st_mode) && fflag == 1)
        {
            fprintf(stderr, RED"No matches found!\n"reset);
            return;
        }
        else if(S_ISDIR(fstat->st_mode)) //chdir
        {
            *pwd = getcwd(NULL, MAX_PATH_LEN);

            if(chdir(firstfile) != 0)
            {
                perror(RED"chdir"reset);
                return;
            }
        }
    }   


}