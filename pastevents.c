#include "headers.h"
// extern char** process;

void AddPastEvent(char** strings, int count, char** laststrings, int lastcount, char* abs_path)
{
    //open fiel for write, check:
    //printf("abs: %s\n", abs_path);

    // printf("Entered AddPastEvents:\n");
    // for(int g = 0; g < count; g++)
    // {
    //     printf("to add_%d: %s\n", g, strings[g]);
    // }
    FILE* readfile = fopen(abs_path, "r");
    if(readfile == NULL)
    {
        perror(RED"fopen for pastevents"reset);
    }

    //array to store file content
    char line[MAX_LINES][MAX_INPUT_LENGTH];

    //read file
    int i;
    for(i  = 0; fgets(line[i], MAX_INPUT_LENGTH, readfile); i++) 
	{
        line[i][strlen(line[i]) - 1] = '\0';
    }

    int total = i;
	// printf("\n File content: \n");    

    // for(i = 0; i < total; i++)
    // {
    //     printf("%s\n", line[i]);
    // }

    //printf("\ni: %d\n", i);
    
    fclose(readfile);

    char** filecommand;
    int* newcount = malloc(sizeof(int*));
    *newcount = 0;

    filecommand = tokenize(line[i - 1], newcount);
    //printf("line[%d]: %s\nnewcount: %d\ncount: %d\n", i - 1, line[i - 1], *newcount, count);

    // for(int q = 0; q < count; q++)
    // {
    //     printf("strings%d: '%s'\n", q, strings[q]);
    // }
    FILE* appfile = fopen(abs_path, "a+");

    if(i >= 0) //file not empty
    {
        int diff = 0;
        //check if last command is same
        if(i > 0 && *newcount == count)
        {
            //printf("ROMANIANNNN\n");
            for(int p = 0; p < count; p++)
            {
                if(strcmp(strings[p], filecommand[p]) != 0)
                {
                    diff = 1;
                }
            }
            if(diff == 0) //same
            {
             //   printf("returned\n");
                return;
            }
        }
    }

    if(i == 15) //append the lasat fourteen to the empty file
    {
        fclose(appfile);

        FILE* wrfile = fopen(abs_path, "w");

        for(int p = 1; p < 15; p++)
        {
            // if(strcmp(strings[p], "pastevents") != 0)
            // {
                int ret = fprintf(wrfile, "%s\n", line[p]);
                //printf("ret val:%d\n", ret);
                if(ret < 0)
                {
                    perror(RED"fprintf append"reset);
                }
            //}
        }
        int flag = 0;
        for(int p = 0; p < count; p++)
        {
            if(strcmp(strings[p], "pastevents") == 0)
            {
                flag = 1;
            }
        }
        for(int p = 0; p < count; p++)
        {
            if(flag == 0)
            {
                int ret = fprintf(wrfile, "%s ", strings[p]);
                //printf("ret val:%d\n", ret);
                if(ret < 0)
                {
                    perror(RED"fprintf append"reset);
                }
            }
        }

        if(flag == 0)
        {
            fprintf(wrfile, "\n");
        }
        fclose(wrfile);
        return;

    }

    //check if pastevents dont add
    for(int f = 0; f < count; f++)
    {
        if(strcmp(strings[f], "pastevents") == 0)
        {
            ////add edited string to pastfile
            //ill add it earlier if its an execute so itll only come here as pastevents if its a purge or normal in which case just return nothing to add to file
            //printf("pastevent alert!\n");
            return;
        }
    }
    
    //add the strings space separated
    //printf("reached append\n");
    int flag = 0;
    for(int p = 0; p < count; p++)
    {
        if(strcmp(strings[p], "pastevents") == 0)
            {
                flag = 1;
            }
    }
    for(int p = 0; p < count; p++)
    {
        if(flag == 0)
        {
            int ret = fprintf(appfile, "%s ", strings[p]);
            //printf("ret val:%d\n", ret);
            if(ret < 0)
            {
                perror(RED"fprintf append"reset);
            }
        }
        //}
    }

    if(flag == 0)
    {
        fprintf(appfile, "\n");
    }
        
    fclose(appfile);

    for(int g = count; g < MAX_INPUT_WORDS; g++)
    {
        if(filecommand[g] != NULL)
        {
            free(filecommand[g]);
        }
    }
    free(filecommand);
}

void FindPastEvent(char** strings, int* count, Command cmd, int address, char* abs_path, char* invokeddir, char** pwd, int* timetaken, char* process)

{
    //printf("Indside Find Past Event:\ncmd: %s\narg1: %s\narg2: %s\nindex: %d\nargcount: %d\n", cmd->command, cmd->args[0], cmd->args[1], cmd->index, cmd->argcount);
    for(int q = 0; q < cmd->index + cmd->argcount + 1; q++)
    {
        //printf("strings%d: '%s'\n", q, strings[q]);
    }

    FILE* pfile = fopen(abs_path, "r");
    if(pfile == NULL)
    {
        perror(RED"fopen for pastevents"reset);
    }

    //array to store file content
    char line[MAX_LINES][MAX_INPUT_LENGTH];

    //read file
    int i;
    for(i  = 0; fgets(line[i], MAX_INPUT_LENGTH, pfile); i++) 
	{
        line[i][strlen(line[i]) - 1] = '\0';
        //printf("line%d: '%s'\n", i, line[i]);
    }

    int total = i;

    fclose(pfile);

    //now execute line[address]
    char** filecommand;

    // for(int h = 0; h < i; h++)
    // {
    //     printf("line%d : %s\n", h, line[h]);
    // }


    // printf("address: %d\n", address);

    // printf("line add:%s\n", line[i - address]);

    int * subcount = malloc(sizeof(int*));
    *subcount = 0;

    filecommand = tokenize(line[i - address], subcount);
    //printf("subcount: %d\n", *subcount);


    //dup the first part of the command, dup the second part, dup the new command, strcat them

    char** concatenated = malloc(sizeof(char*)*(*subcount + *count - cmd->argcount - 1));
    
    for(int d = 0; d < cmd->index; d++)
    {
        concatenated[d] = strings[d];
        // printf("%d: %s ", d, concatenated[d]);
    }

    for(int d = cmd->index; d < cmd->index + *subcount; d++)
    {
        free(strings[d]);
        concatenated[d] = filecommand[d - cmd->index];
        // printf("%d: %s ", d, concatenated[d]);
    }

    for(int d = cmd->index + cmd->argcount + 1; d < *count; d++)
    {
        // printf("ind: %d str: %s\n", d - cmd->argcount - 1 + *subcount, strings[d]);

        concatenated[d - cmd->argcount - 1 + *subcount] = strings[d];

        // printf("%s ", concatenated[d - cmd->argcount - 1 + *subcount]);
    }
    // printf("\n");

    for(int k = 0; k < *count; k++)
    {
        strings[k] = NULL;
    }

    //printf("\nConcatenated:\n");
    for(int k = 0; k < *subcount + *count - cmd->argcount - 1; k++)
    {
        strings[k] = concatenated[k];
        //printf("%d: %s\n", k, concatenated[k]);
    }


    // for(int k = 0; k < *subcount + *count - cmd->argcount - 1; k++)
    // {
    //     free(concatenated[k]);
    // }
    // free(concatenated);

    *count = *count + *subcount - cmd->argcount - 1;

    for(int g = *subcount; g < MAX_INPUT_WORDS; g++)
    {
        if(filecommand[g] != NULL)
        {
            free(filecommand[g]);
        }
    }
    free(filecommand);



}

void PastPurge(char* path)
{
    FILE* file = fopen(path, "w");
    fclose(file);
}

void PastPrint(char* path)
{
    //printf("Indside Print Past\n");

    FILE* file = fopen(path, "r");

    if(file == NULL)
    {
        perror(RED"fopen for pastevents"reset);
    }

    //array to store file content
    char line[MAX_LINES][MAX_INPUT_LENGTH];

    //read file
    int i;
    for(i  = 0; fgets(line[i], MAX_INPUT_LENGTH, file); i++) 
	{
        line[i][strlen(line[i]) - 1] = '\0';
    }

    int total = i;

    for(int i = 0; i < total; i++)
    {
        printf("%s\n", line[i]);
    }
    //printf("\n");
    
    fclose(file);
}