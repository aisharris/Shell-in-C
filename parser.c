#include "headers.h"

//make a struct of a command, which includes the command name, arguments, fg/bg status, input/output redirection

//check for >, >>, &, |, ;, and split up commands based on those
//then put the first string of the command as the command and store the rest as arguments 

void Parse(char** strings, int* count, char* parsefilepath, char* invokeddir, char** pwd, int* timetaken, char* process)
{

    //get current working directory
    int currcount = *count;
    int p_flag = 0;

    //redirection definitions:
    int redirect_flag = 0;
    char* output_file = malloc(MAX_FILE_NAME);
    output_file[0] = '\0';

    char* input_file = malloc(MAX_FILE_NAME);
    input_file[0] = '\0';

    for(int i = 0; i < currcount; i++)
    {
        if(strcmp(strings[i], "pastevents") == 0)
        {
            if(i < *count - 1 && strcmp(strings[i + 1], "execute") == 0)
            {
                p_flag = 1;
            }
        }
    }

    if(p_flag == 1)
    {
        while(p_flag == 1)
        {
            for(int i = 0; i < *count; i++)
            {
                //printf("peloop %d\n", i);
                if(strcmp(strings[i], "pastevents") == 0)
                {
                    if(i < *count - 1 && strcmp(strings[i + 1], "execute") == 0)
                    {
                        Command cmd = malloc(sizeof(struct Command));
                        cmd->command = malloc(11);
                        cmd->command = "pastevents";
                        cmd->args = malloc(sizeof(char*)*(2 + 1)); 
                        cmd->args[0] = "execute";
                        cmd->index = i;
                        
                        if(i < *count - 2)
                        {
                            cmd->args[1] = strings[i+2]; 
                        }
                        cmd->args[2] = malloc(2);
                        cmd->args[2][0] = '\0';
                        cmd->argcount = 2;
                        int numargs = 2;

                        // printf("A gonna evaluate %s\n", cmd->command);
                        strcpy(process, cmd->command);
                        // printf("process: %s, address: %p\n", process, &process);

                        
                        EvaluateCmd(strings, count, parsefilepath, cmd, 1, invokeddir, pwd, numargs, timetaken, process, redirect_flag, input_file, output_file);

                        break;
                    }
                }
            }   

            p_flag = 0;

            for(int i = 0; i < *count; i++)
            {
                if(strcmp(strings[i], "pastevents") == 0)
                {
                    if(i < *count - 1 && strcmp(strings[i + 1], "execute") == 0)
                    {
                        p_flag = 1;
                    }
                }
            }
        }
    }

    if(p_flag == 0)
    {
        int lastadd = 0;
        // printf("count : %d\n", count);

        //printf("to parse:\n");    

        int semioramp = 0;
        int red  = 0;
        int pipe = 0;

        for(int i = 0; i < *count; i++)
        {
            if(strcmp(strings[i], "&") == 0 || strcmp(strings[i], ";") == 0)
            {
                semioramp = 1;
            }
            else if (strcmp(strings[i], ">") == 0 || strcmp(strings[i], "<") == 0 || strcmp(strings[i], ">>") == 0)
            {
                red  = 1;
            }
            else if(strcmp(strings[i], "|") == 0)
            {
                pipe = 1;
            }

        }

        if(semioramp == 1)
        {

            for(int i = 0; i < *count; i++)
            {
                // printf("entered, i: %d, lastadd: %d\n", i, lastadd);  
                //printf("Parse loop index: %d\n", i);

                for(int p = 0; p < *count; p++)
                {
                    //printf("%s ", strings[p]);
                }
                //printf("\n");

                int flag = 0;


                // if(strcmp(strings[i], "<") == 0 || strcmp(strings[i], ">") == 0 || strcmp(strings[i], ">>") == 0 || strcmp(strings[i], "|") == 0 || strcmp(strings[i], "&") == 0 || strcmp(strings[i], ";") == 0) 
                // {
                //     printf("i: %d\n", i);
                //     printf("token: %s\n", strings[i]);

                if(strcmp(strings[i], "&") == 0 || strcmp(strings[i], ";") == 0)
                {
                    // printf("i: %d, lastadd: %d\n", i, lastadd);
                    for(int f = lastadd; f < i + lastadd; f++)
                    {
                        if(strcmp(strings[f], ">") == 0 || strcmp(strings[f], ">>") == 0 || strcmp(strings[f], "|") == 0 || strcmp(strings[f], "<") == 0)
                        {

                            printf("f: %d\n", f);

                            char** newstrings = malloc(sizeof(char*)*(i - lastadd));
                            
                            int* newcount = malloc(sizeof(int*));
                            *newcount = i - lastadd;

                            for(int v = lastadd; v < i; v++)
                            {
                                newstrings[v - lastadd] = strings[v];
                            }

                            if(strcmp(strings[i], "&") == 0)
                            {
                                //put flag for bg
                                flag = 1;
                                SpecialParse(0, newstrings, newcount, parsefilepath, invokeddir, pwd, timetaken, process, pipe, red);
                                lastadd = i + 1;
                                printf("next up: %s\n", strings[i + 1]);
                            }
                            else
                            {
                                //flag for fg
                                flag = 1;
                                SpecialParse(1, newstrings, newcount, parsefilepath, invokeddir, pwd, timetaken, process, pipe, red);
                                lastadd = i + 1;
                                printf("next up: %s\n", strings[i + 1]);
                            }

                            break;
                        }    
                    }

                    if(flag == 1)
                    {
                        continue;
                    }

                    Command cmd = malloc(sizeof(struct Command));
                    cmd->command = malloc(sizeof(char)*MAX_ARG_LEN);

                    int numargs = i - lastadd - 1;
                    if(i == lastadd)
                    {
                        lastadd++;
                        continue;
                    }

                    //printing debugging
                    //printf("index  = %d\nstart = %d\nnumargs = %d\n", i, lastadd, numargs);

                    // for(int j = 0; j < numargs; j++)
                    // {
                    //     cmd->args[j] = malloc(sizeof(char)*MAX_ARG_LEN);
                    // }
                    
                    strcpy(cmd->command, strings[lastadd]);


                    //printing debugging
                    //printf("Cmd: '%s'\n", cmd->command);

                    cmd->args = malloc(sizeof(char*)*(numargs + 1)); //size of whole command is i - lastadd, so args are i - lastadd - 1
                    
                    cmd->index = lastadd;

                    //printing debugging
                    //printf("index added: %d\n", cmd->index);
                    
                    
                    //printf("numargs = %d\n", numargs);
                    // if(numargs == 0)
                    // {
                    //     cmd->args[0] = NULL;
                    // }

                    for(int p = lastadd + 1; p < i; p++)
                    {
                        cmd->args[p - lastadd - 1] = malloc(sizeof(char)*MAX_ARG_LEN);
                        cmd->args[p - lastadd - 1] = strings[p];
                    }

                    cmd->args[numargs] = malloc(2);
                    cmd->args[numargs][0] = '\0';
                    
                    for(int p = 0; p < numargs; p++)
                    {
                        // printf("arg_%d: '%s'\n", p, cmd->args[p]);
                    }
                    cmd->argcount = numargs;

                    //done separating cmds and args

                    //printf("Going to evaluate %s\n", cmd->command);
                    //printf("redflag: %d\n", redirect_flag);
                    if(strcmp(strings[i], "&") == 0)
                    {
                        // printf("B gonna evaluate %s\n", cmd->command);
                        strcpy(process, cmd->command);
                        // printf("process: %s\n", process);
                        EvaluateCmd(strings, count, parsefilepath, cmd, 0, invokeddir, pwd, numargs, timetaken, process, redirect_flag, input_file, output_file); //bg
                    }
                    else
                    {
                        // printf("C gonna evaluate %s\n", cmd->command);
                        strcpy(process, cmd->command);
                        // printf("process: %s\n", process);
                        EvaluateCmd(strings, count, parsefilepath, cmd, 1, invokeddir, pwd, numargs, timetaken, process, redirect_flag, input_file, output_file); //fg
                    }

                    lastadd = i + 1; //ignore the delimiter
                }

                    // else if(strcmp(strings[i], "<") == 0 || strcmp(strings[i], ">") == 0 || strcmp(strings[i], ">>") == 0)
                    // {
                    //     //check if theres another arrow upccomming
                    //     //if there is, do something different
                    //     //if there isnt, continue with this
                    //     //flag = 4 if theres both input and output
                    //     
                // }
                else if (i == *count - 1)
                {
                    if(lastadd <= i)
                    {
                        redirect_flag = 0;
                        // printf("LAST STRING i: %d, lastadd: %d\n", i, lastadd);
                        Command cmd = malloc(sizeof(struct Command));
                        cmd->args = malloc(sizeof(char*)*(i - lastadd + 1)); 

                        int numargs = i - lastadd;
                        
                        //printf("index  = %d\nstart = %d\nnumargs = %d\n", i, lastadd, numargs);

                        cmd->command = strings[lastadd];
                        cmd->index = lastadd;
                        //printf("index added: %d\n", cmd->index);

                        //printf("LALALA\n");
                        //printf("numargs = %d\n", numargs);

                        // if(numargs == 0)
                        // {
                        //     cmd->args = malloc(sizeof(char*));
                        //     cmd->args[0] = NULL;
                        // }

                        for(int p = lastadd + 1; p <= i; p++)
                        {
                            cmd->args[p - lastadd - 1] = malloc(sizeof(char)*MAX_ARG_LEN);
                            cmd->args[p - lastadd - 1] = strings[p];
                            // printf("%s\n", strings[p]);
                        }

                        cmd->argcount = numargs;
                        
                        cmd->args[numargs] = malloc(2);

                        cmd->args[numargs][0] = '\0';

                        //printf("Cmd: '%s'\n", cmd->command);
                        for(int p = 0; p < numargs; p++)
                        {
                            // printf("arg_%d: '%s'\n", p, cmd->args[p]);
                        }

                        // printf("D gonna evaluate %s\n", cmd->command);
                        strcpy(process, cmd->command);
                        // printf("process: %s, address: %p\n", process, &process);
                        EvaluateCmd(strings, count, parsefilepath, cmd, 1, invokeddir, pwd, numargs, timetaken, process,  redirect_flag, input_file, output_file); //fg
                        //printf("%s\n", pwd);
                        // printf("back at parser, process: %s, address: %p\n", process, &process);
                    }
                }
            }
        }
        else if(red == 1 || pipe == 1)
        {
            SpecialParse(0, strings, count, parsefilepath, invokeddir, pwd, timetaken, process, pipe, red);
        }
        else
        {
            for(int i = 0; i < *count; i ++)
            {
                if (i == *count - 1)
                {
                    if(lastadd <= i)
                    {
                        redirect_flag = 0;
                        // printf("LAST STRING i: %d, lastadd: %d\n", i, lastadd);
                        Command cmd = malloc(sizeof(struct Command));
                        cmd->args = malloc(sizeof(char*)*(i - lastadd + 1)); 

                        int numargs = i - lastadd;
                        
                        //printf("index  = %d\nstart = %d\nnumargs = %d\n", i, lastadd, numargs);

                        cmd->command = strings[lastadd];
                        cmd->index = lastadd;
                        //printf("index added: %d\n", cmd->index);

                        //printf("LALALA\n");
                        //printf("numargs = %d\n", numargs);

                        // if(numargs == 0)
                        // {
                        //     cmd->args = malloc(sizeof(char*));
                        //     cmd->args[0] = NULL;
                        // }

                        for(int p = lastadd + 1; p <= i; p++)
                        {
                            cmd->args[p - lastadd - 1] = malloc(sizeof(char)*MAX_ARG_LEN);
                            cmd->args[p - lastadd - 1] = strings[p];
                            // printf("%s\n", strings[p]);
                        }

                        cmd->argcount = numargs;
                        
                        cmd->args[numargs] = malloc(2);

                        cmd->args[numargs][0] = '\0';

                        //printf("Cmd: '%s'\n", cmd->command);
                        for(int p = 0; p < numargs; p++)
                        {
                            // printf("arg_%d: '%s'\n", p, cmd->args[p]);
                        }

                        // printf("D gonna evaluate %s\n", cmd->command);
                        strcpy(process, cmd->command);
                        // printf("process: %s, address: %p\n", process, &process);
                        EvaluateCmd(strings, count, parsefilepath, cmd, 1, invokeddir, pwd, numargs, timetaken, process,  redirect_flag, input_file, output_file); //fg
                        //printf("%s\n", pwd);
                        // printf("back at parser, process: %s, address: %p\n", process, &process);
                    }
                }
            }
        }

    }

    //     }
    //}
}

void SpecialParse(int flag, char** strings, int* count, char* parsefilepath, char* invokeddir, char** pwd, int* timetaken, char* process, int fpipe, int red)
{
    if(fpipe == 1)
    {
        //separate strings based on pipe
        // execute pipe loop and send each command strings to parser to execute

        int lastadd = 0;
        int pipe_count = 0;

        for(int g = 0; g < *count; g++)
        {
            if(strcmp(strings[g], "|") == 0)
            {
                pipe_count++;
            }
        }

        char*** Commands = malloc(sizeof(char**)*(pipe_count + 2));

        for(int h = 0 ; h < pipe_count + 1; h++)
        {
            Commands[h] = calloc(MAX_ARGS, sizeof(char*));
        }

        int cmds = 0;
        int cmdstrngs = 0;

        for(int f = 0; f < *count; f++)
        {
            // printf("f: %d\n", f);
            if(strcmp(strings[f], "|") == 0 || f == *count - 1)
            {
                int p = 0;
                // printf("lastadd: %d, f: %d\n", lastadd, f);
                for(int d = lastadd; d <= f && strcmp(strings[d], "|") != 0 ; d++)
                {
                    // printf("d - lastadd: %d, cmd: %d\n", d - lastadd, cmds);
                    // Commands[cmds][d - lastadd] = calloc(MAX_ARG_LEN, 1);
                    Commands[cmds][d - lastadd] = strings[d];
                    // printf("command string added: %s, index: %d\n", Commands[cmds][d - lastadd], d - lastadd);
                    p = d;
                }
                Commands[cmds][p - lastadd + 1] = NULL;
                cmds++;
                lastadd = f + 1;
            }
        }

        int standin = dup(STDIN_FILENO);
        int standout = dup(STDOUT_FILENO);

        size_t i, n;
        int prev_pipe, pfds[2];

        prev_pipe = STDIN_FILENO;

        int arrcount = 0;

        n = pipe_count + 1;

        for (i = 0; i < n - 1; i++) 
        {
            arrcount = 0;
            for(int p = 0; Commands[i][p] != NULL; p++)
            {
                arrcount++;
                // printf("arrcount: %d, string: %s\n", arrcount, Commands[i][p]);
            }
            int* newcnt = malloc(sizeof(int*));
            *newcnt = arrcount;

            pipe(pfds);

            if (fork() == 0) {
                // Redirect previous pipe to stdin
                if (prev_pipe != STDIN_FILENO) {
                    dup2(prev_pipe, STDIN_FILENO);
                    close(prev_pipe);
                }

                // Redirect stdout to current pipe
                dup2(pfds[1], STDOUT_FILENO);
                close(pfds[1]);

                // Start command
                Parse(Commands[i], newcnt, parsefilepath, invokeddir, pwd, timetaken, process);

                exit(EXIT_SUCCESS);
            }

            close(prev_pipe);
            close(pfds[1]);

            prev_pipe = pfds[0];
        }

        arrcount = 0;
        for(int p = 0; Commands[n - 1][p] != NULL; p++)
        {
            arrcount++;
            // printf("arrcount: %d, string: %s\n", arrcount, Commands[n - 1][p]);
        }
        int* newcnt = malloc(sizeof(int*));
        *newcnt = arrcount;
        
        // Get stdin from last pipe
        if (prev_pipe != STDIN_FILENO) {
            dup2(prev_pipe, STDIN_FILENO);
            close(prev_pipe);
        }

        // Start last command
        Parse(Commands[n - 1], newcnt, parsefilepath, invokeddir, pwd, timetaken, process);

        close(pfds[0]);
        close(pfds[1]);
        dup2(standin, STDIN_FILENO);
        close(standin);
        dup2(standout, STDOUT_FILENO);
        close(standout);
    }
    else if(fpipe == 0 && red == 1) 
    {

        Command cmd = malloc(sizeof(struct Command));
        cmd->command = malloc(sizeof(char)*MAX_ARG_LEN);
        int evaled_both = 0;
        flag = 1;

        //redirection definitions:
        int redirect_flag = 0;
        char* output_file = malloc(MAX_FILE_NAME);
        output_file[0] = '\0';

        char* input_file = malloc(MAX_FILE_NAME);
        input_file[0] = '\0';

        int lastadd = 0;

        int redirectpos = 0;

        for(int i = 0; i < *count; i++)
        {

            if(strcmp(strings[i], ">") == 0 || strcmp(strings[i], ">>") == 0)
            {
                if(i + 1 < *count)
                {
                    for(int k = i + 1; k < *count; k ++)
                    {
                        if(strcmp(strings[k], ">") == 0 || strcmp(strings[k], ">>") == 0) 
                        {
                            //error
                            fprintf(stderr, RED"ERROR: Cannot handle multiple output redirections\n"reset);
                            return;
                        }
                    }
                }
                else
                {
                    fprintf(stderr, RED"ERROR: No output file specified\n"reset);
                    return;   
                }
                
                for(int k = i; k < *count; k ++)
                {
                    if(strcmp(strings[k], "<") == 0)
                    {
                        //handle both in and output
                        if(i > 0)
                        {
                            // printf("inside k loop\n");
                            if(i + 2 < *count)
                            {
                                output_file = strings[i + 1];
                                // printf("outfile: %s\n", output_file);
                                // printf("next: %c\n", strings[i + 2][0]);

                                if(strings[i + 2][0] != '<')
                                {
                                    // printf("hoe\n");
                                    fprintf(stderr, RED"ERROR: Multiple output files not allowed\n"reset);
                                    return;  
                                }

                                int g = i;
                                g--;
                                while(g >= 0 && strcmp(strings[g], ";") != 0 && strcmp(strings[g], "&") != 0)
                                {
                                    // printf("string[%d]: %s\n", g, strings[g]);
                                    g--;
                                }
                                g++;
                                cmd->command = strings[g];
                                // printf("command: %s\n", cmd->command);
                                cmd->argcount = i - g - 1;
                                int numargs = cmd->argcount;
                                cmd->args = malloc(sizeof(char*)*(cmd->argcount + 1));
                                for(int h = g + 1; h < i; h++)
                                {
                                    cmd->args[h - g - 1] = strings[h];
                                }
                                cmd->args[cmd->argcount] = malloc(2);
                                cmd->args[cmd->argcount][0] = '\0';

                                if(strcmp(strings[i], ">>") == 0)
                                {
                                    redirect_flag = 4;
                                }
                                else
                                {
                                    redirect_flag = 5;
                                }
                                
                                if(i + 3 < *count)
                                {
                                    input_file = strings[i + 3];
                                    // printf("infile: %s\n", input_file);
                                }
                                else
                                {
                                    fprintf(stderr, RED"ERROR: No input file specified\n"reset);
                                    return;  
                                }
                                strcpy(process, cmd->command);
                                EvaluateCmd(strings, count, parsefilepath, cmd, flag, invokeddir, pwd, numargs, timetaken, process, redirect_flag, input_file, output_file); 
                                
                                i = i + 4;
                                lastadd = i + 4;
                                evaled_both = 1;
                                // printf("came back to parser, i = %d\n", i);
                                return;
                            }
                            else
                            {
                                fprintf(stderr, RED"ERROR: No output file specified\n"reset);
                                return; 
                            }


                            break;
                        }
                    }
                    redirectpos = i;
                    
                }
                if(strcmp(strings[i], ">") == 0)
                {
                    redirect_flag = 1;
                }
                else
                {
                    redirect_flag = 2;
                }
            }
            else if(strcmp(strings[i], ">>") == 0)
            {
                redirect_flag = 2;
                redirectpos = i;
            }
            else if(strcmp(strings[i], "<") == 0)
            {
                redirectpos = i;
                redirect_flag = 3;

                if(i + 1 < *count)
                {
                    for(int k = i + 1; k < *count; k ++)
                    {
                        if(strcmp(strings[k], "<") == 0) 
                        {
                            //error
                            fprintf(stderr, RED"ERROR: Cannot handle multiple input files\n"reset);
                            return; 
                        }
                    }
                }
                else
                {
                    fprintf(stderr, RED"ERROR: No input file specified\n"reset);
                    return;   
                }

                for(int k = i; k < *count; k ++)
                {
                    if(strcmp(strings[k], ">") == 0 || strcmp(strings[k], ">>") == 0)
                    {
                        //handle both in and output
                        if(i > 0)
                        {
                            // printf("inside k loop\n");
                            if(i + 2 < *count)
                            {
                                input_file = strings[i + 1];
                                // printf("infile: %s\n", input_file);
                                // printf("next: %c\n", strings[i + 2][0]);

                                if(strings[i + 2][0] != '>')
                                {
                                    // printf("hoe\n");
                                    fprintf(stderr, RED"ERROR: Multiple input files not allowed\n"reset);
                                    return;   
                                }

                                int g = i;
                                g--;
                                while(g >= 0 && strcmp(strings[g], ";") != 0 && strcmp(strings[g], "&") != 0)
                                {
                                    // printf("string[%d]: %s\n", g, strings[g]);
                                    g--;
                                }
                                g++;
                                cmd->command = strings[g];
                                // printf("command: %s\n", cmd->command);
                                cmd->argcount = i - g - 1;
                                int numargs = cmd->argcount;
                                cmd->args = malloc(sizeof(char*)*(cmd->argcount + 1));
                                for(int h = g + 1; h < i; h++)
                                {
                                    cmd->args[h - g - 1] = strings[h];
                                }
                                cmd->args[cmd->argcount] = malloc(2);
                                cmd->args[cmd->argcount][0] = '\0';

                                //set red flag
                                if(strcmp(strings[k], ">") == 0)
                                {
                                    redirect_flag = 5;
                                }
                                else
                                {
                                    redirect_flag = 4;
                                }
                                
                                if(i + 3 < *count)
                                {
                                    output_file = strings[i + 3];
                                    // printf("outfile: %s\n", output_file);
                                }
                                else
                                {
                                    fprintf(stderr, RED"ERROR: No output file specified\n"reset);
                                    return; 
                                }

                                // printf("redflag = %d\n", redirect_flag);
                                strcpy(process, cmd->command);
                                EvaluateCmd(strings, count, parsefilepath, cmd, flag, invokeddir, pwd, numargs, timetaken, process, redirect_flag, input_file, output_file); 
                                
                                i = i + 4;
                                lastadd = i + 4;
                                evaled_both = 1;
                                // printf("came back to parser, i = %d\n", i);
                                return;
                            }
                            else
                            {
                                fprintf(stderr, RED"ERROR: No input file specified\n"reset);
                                return; 
                            }
                            break;
                        }
                    }
                }
            }
            if(redirect_flag != 0)
            {
                if(redirect_flag == 1 || redirect_flag == 2)
                {
                    strcpy(output_file, strings[redirectpos + 1]);
                    // printf("outfile: %s\n", output_file);
                }
                else
                {
                    strcpy(input_file, strings[redirectpos + 1]);
                    // printf("infile: %s\n", input_file);
                }
                
                int g = redirectpos;
                // printf("redirectpos = %d\n", redirectpos);
                while(--g)
                {
                    // printf("string[%d]: %s\n", g, strings[g]);
                }

                cmd->command = strings[g];
                // printf("command: %s\n", cmd->command);
                cmd->argcount = redirectpos - 1;
                int numargs = cmd->argcount;
                cmd->args = malloc(sizeof(char*)*(cmd->argcount + 1));
                for(int h = g + 1; h < i; h++)
                {
                    cmd->args[h - g - 1] = strings[h];
                }
                cmd->args[cmd->argcount] = malloc(2);
                cmd->args[cmd->argcount][0] = '\0';

                strcpy(process, cmd->command);
                EvaluateCmd(strings, count, parsefilepath, cmd, flag, invokeddir, pwd, numargs, timetaken, process, redirect_flag, input_file, output_file); 

                return;

            }
        }
    }
}

