#include "headers.h"

int prompt(char* invokeddir, const int timetaken, char* process) 
{
    // Do not hardcode the prmopt

    //getting name of the system
    char* hostname = malloc(sizeof(char)*101);
    if(gethostname(hostname, sizeof(hostname)) != 0)
    {
        perror(RED"gethostname"reset);
        return 0;
    }

    //getting user name
    char* username = getlogin();
    if(username == NULL)
    {
        perror(RED"getlogin"reset);
        return 0;
    }

    //get current working directory
    char cwd[MAX_PATH_LEN];
    if(getcwd(cwd, sizeof(cwd)) == NULL)
    {
        perror(RED"getcwd"reset);
        return 0;
    }

    //if youre in the exact same directory you invoked from, display ~
    //if you go further in, ie the invoked directory is a substring of your current cwd, display ~ inplace of the original and add the relative path
    //if you step out, ie invoked dir is not a substring, the display the whole path instead of ~
    
    if(strcmp(cwd, invokeddir) == 0)
    {
        if(timetaken > 2)
        {
            //round off to closest int
            printf(MAGENTA"<%s@%s:~"reset, username, hostname);
            printf(GREEN" %s : %ds>"reset,  process, timetaken);
        }
        else
        {
            printf(MAGENTA"<%s@%s:~>"reset, username, hostname);
        }
    }
    else if(strncmp(cwd, invokeddir, strlen(invokeddir)) == 0)
    {
        //display only relative path
        int len = strlen(invokeddir);
        char* relpath = malloc(sizeof(char)*MAX_PATH_LEN);
        for(int i = len; i < MAX_PATH_LEN; i ++) 
        //um??????youre putting the rest of the entire instruction as the path there could be redirects or pipes. apparently not.
        {
            relpath[i - len] = cwd[i];
            if(cwd[i] == '\0')
            {
                break;
            }
        }
        if(timetaken > 2)
        {
            //round off to closest int
            printf(MAGENTA"<%s@%s:~"reset, username, hostname);
            printf(BLUE"%s"reset, relpath);
            printf(GREEN" %s : %ds>"reset, process, timetaken);
        }
        else
        {
            printf(MAGENTA"<%s@%s:~"reset, username, hostname);
            printf(BLUE"%s>"reset, relpath);
        }
    }
    else
    {
        if(timetaken > 2)
        {
            //round off to closest int
            printf(MAGENTA"<%s@%s:"reset, username, hostname);
            printf(CYAN"%s "reset, cwd);
            printf(GREEN"%s :"reset, process);
            printf(GREEN" %ds>"reset, timetaken);
        }
        else
        {
            printf(MAGENTA"<%s@%s:"reset, username, hostname);
            printf(CYAN"%s>"reset, cwd);
        }
        
    }
}
