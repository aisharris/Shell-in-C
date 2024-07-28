#include "headers.h"
Node bgCheckList;

volatile sig_atomic_t cntc = 0;
volatile sig_atomic_t cntz = 0;

void handle_sig(int sig)
{
    if(sig == SIGINT)
    {
        cntc = 1;
        return;
    }
    else if(sig == SIGTSTP)
    {
        cntz = 1;
        return;
    }
}


int main()
{

    struct sigaction ctrlc = {0};
    struct sigaction ctrlz = {0};

    ctrlc.sa_handler = handle_sig;
    ctrlz.sa_handler = handle_sig;

    sigaction(SIGINT, &ctrlc, NULL);
    sigaction(SIGTSTP, &ctrlz, NULL);

    char* invokeddir = getcwd(NULL, 0);

    if (invokeddir == NULL) {
        perror(RED"getcwd"reset);
        return 1;
    }
    char** pwd = malloc(sizeof(char*));
    // *pwd = malloc(sizeof(char)*(MAX_PATH_LEN));
    *pwd = NULL;

    //initialize time and process variable
    int* timetaken = malloc(sizeof(int*));
    *timetaken = 0;

    char* process = calloc(MAX_WORD_LENGTH, 1);
    process[0] = '\0';
    // Keep accepting commands

    char** laststrings = NULL;
    int lastcount = 0;
    //pastevents: open file to write
    FILE* pastfile;

    pastfile = fopen("pastevents.txt", "a");
    char abs_path[MAX_PATH_LEN];
    if (realpath("pastevents.txt", abs_path) == NULL) 
    {
        perror(RED"realpath"reset);
        return 0;
    }
    
    fclose(pastfile);
    bgCheckList = InitList();
    // if(bgCheckList != NULL)
    // {
    //     printf("ITS CREATED\n");
    // }


    while (1)
    {
        // Print appropriate prompt with username, systemname and directory before accepting input
        int status = prompt(invokeddir, *timetaken, process);

        cntc = 0;
        cntz = 0;

        *timetaken = 0;
        if(status == 1)
        {
            perror(RED"prompt"reset);
            return 0;
        }
        //use fgets to put command into array 'input'
        char input[4096];
        char* fpter;

        fpter = fgets(input, 4096, stdin);

        int len = strlen(input);

        input[strcspn(input, "\n")] = '\0';                 //add null char to end

        if(cntc || cntz)
        {
            printf("\n");
            cntc = 0;
            cntz = 0;
            continue;
        }
        
        if(fpter == NULL)
        {
            //kill all processes mwhahahahaha
            List temp = bgCheckList;
            while(temp->next != NULL)
            {
                kill(temp->next->pid, SIGKILL);
                temp = temp->next;
            }
            printf("\n");
            exit(EXIT_SUCCESS);
        }

        char** strings;

        //  = malloc(sizeof(char*)*MAX_INPUT_WORDS);
        // for(int i = 0; i < MAX_INPUT_WORDS; i++)
        // {
        //     strings[i] = malloc(MAX_WORD_LENGTH);
        // }

        int count = 0;

        strings = tokenize(input, &count);

        List temp = bgCheckList;

        if(temp->next == NULL)
        {
            // printf("LIST EMPTY\n");
        }
        // while(temp->next != NULL)
        // {
        //     printf("list pid: %d\n", temp->next->pid);
        //     temp= temp->next;
        // }
        // temp = bgCheckList;

        while(temp->next != NULL)
        {
            int flag = 0;
            //printf("in list: %d\n", temp->next->pid);
            int terminated_pid = waitpid(temp->next->pid, &status, WNOHANG);
            if (terminated_pid == -1) {
                perror(RED"waitpid"reset);
                return 0;
            }
            // if(temp->next->next == NULL)
            // {
            //     printf("yes its null after\n");
            // }
            if (terminated_pid != 0 && WIFEXITED(status)) 
            {
                //printf("gonna remove %d\n", temp->next->pid);
                printf("%s exited normally(%d)\n", temp->next->p_name, temp->next->pid);
                RemoveNode(bgCheckList, temp->next->pid);
                //printf("exited normally\n");
                flag = 1;

                // Node tempo = bgCheckList;
                // while(tempo != NULL)
                // {
                //     printf("NEW list pid: %d\n", tempo->pid);
                //     tempo= tempo->next;
                // }
            }
            else if (terminated_pid != 0 && WIFSIGNALED(status)) 
            {
                if(WCOREDUMP(status) == 1)
                {
                    printf("%s produced a core dump(%d)\n", temp->next->p_name, temp->next->pid);
                }
                else
                {
                    printf("%s terminated by signal with signal %d(%d)\n", temp->next->p_name, WTERMSIG(status), temp->next->pid);
                    RemoveNode(bgCheckList, temp->next->pid);
                    flag = 1;
                }

            }
            //printf("left loop\n");
            
            if(flag == 0)
            {
                temp = temp->next;
            }
        }



        
        if(strings != NULL)
        {
            for(int i = 0; i < count; i++)
            {
                // printf("'%s'\n", strings[i]);
            }

            //pwd = getcwd(NULL, MAX_PATH_LEN);
            //printf("gonna parse\n");
            // printf("in main, process: %s, address: %p\n", process, &process);
            Parse(strings, &count, abs_path, invokeddir, pwd, timetaken, process);
            // printf("in main, process: %s, address: %p\n", process, &process);
        }

        // for(int q = 0; q < count; q++)
        // {
        //     printf("strings right before addpastevent%d: '%s'\n", q, strings[q]);
        // }
        
        AddPastEvent(strings, count, laststrings, lastcount, abs_path);


        for(int i = 0; i < MAX_INPUT_WORDS; i++)
        {
            if(laststrings != NULL)
            {
                if(laststrings[i] != NULL)
                {
                    free(laststrings[i]);
                }
            }
        }
        if(laststrings != NULL)
        {
            free(laststrings);
        }

        laststrings = strings;
        lastcount = count;
        // for(int q = 0; q < lastcount; q++)
        // {
        //     printf("%s\n", (laststrings[q]));
        // }
    }

    //!free bg linked list

    return 0;
}
