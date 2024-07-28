#include "headers.h"

extern Node bgCheckList;
extern volatile sig_atomic_t cntc;
extern volatile sig_atomic_t cntz;

time_t start_time, end_time;
clock_t cpu_start, cpu_end;

void EvaluateCmd(char** strings, int* count, char* parsefilepath, Command cmd,  int flag, char* invokeddir, char** pwd, int numargs, int* timetaken, char* process, int redirect_flag, char* input_file, char* output_file) //flag = 1 means & run in bg, flag = -1 means ; just run
{
    //printf("EVALUATINGGGGG as %d\n", flag);

    if(strcmp(cmd->command, "warp") == 0)
    {
        if(redirect_flag == 1 || redirect_flag == 2)
        {
            void* err;
            if(redirect_flag == 1)
            {
                err = freopen(output_file, "w", stdout); 
            }
            else
            {
                err = freopen(output_file, "a+", stdout); 
            }

            if(err == NULL)
            {
                perror(RED"freopen"reset);
                return;
            }
            chmod(err, 0644);
            for(int i = 0 ; i < numargs; i++)
            {
                Warp(cmd->args[i], invokeddir, pwd);
            }
            freopen("/dev/tty", "w", stdout); 
        }

        for(int i = 0 ; i < numargs; i++)
        {
            Warp(cmd->args[i], invokeddir, pwd);

        }
        
        if(redirect_flag == 1 || redirect_flag == 2 || redirect_flag == 4 || redirect_flag == 5)
        {
            freopen("/dev/tty", "w", stdout); 
            free(output_file);
        }
        if(redirect_flag == 3 || redirect_flag == 4 || redirect_flag == 5)
        {
            freopen("/dev/tty", "r", stdin);
            free(input_file);
        }   
    }
    else if(strcmp(cmd->command, "peek") == 0)
    {
        if(redirect_flag == 1 || redirect_flag == 2 || redirect_flag == 3 || redirect_flag == 4 || redirect_flag == 5)
        {
            //printf("cmd: %s, cmd->args[0]: %s\n", cmd->command, cmd->args[0]);
            void* err;
            void* errout;
            void* errin;

            if(redirect_flag == 1)
            {
                err = freopen(output_file, "w", stdout); 
                if(err == NULL)
                {
                    perror(RED"freopen"reset);
                    return;
                }
            }
            else if(redirect_flag == 2)
            {
                err = freopen(output_file, "a+", stdout); 
                if(err == NULL)
                {
                    perror(RED"freopen"reset);
                    return;
                }
            }
            else if(redirect_flag == 3)
            {
                // printf("%s\n", input_file);
                err = freopen(input_file, "r", stdin);
                if(err == NULL)
                {
                    perror(RED"freopen"reset);
                    return;
                }
            }
            else if (redirect_flag == 4)
            {
                // printf("out: %s\n", output_file);
                // printf("in: %s\n", input_file);
                errout = freopen(output_file, "a+", stdout); 
                errin = freopen(input_file, "r", stdin);
                if(errout == NULL || errin == NULL)
                {
                    perror(RED"freopen"reset);
                    return;
                }
            }
            else if(redirect_flag == 5)
            {
                // printf("out: %s\n", output_file);
                // printf("in: %s\n", input_file);
                errout = freopen(output_file, "w", stdout); 
                errin = freopen(input_file, "r", stdin);
                if(errout == NULL || errin == NULL)
                {
                    perror(RED"freopen"reset);
                    return;
                }
            }

            chmod(err, 0644);
        }
        //no flags, -a, -l, -a -l, -l -a, -al, -la
        int l_flag = 0;
        int a_flag = 0;
        char absolute_path[MAX_PATH_LEN];

        char* path = malloc(sizeof(char)*(MAX_PATH_LEN));
        path[0] = '\0';

        for(int i = 0; i < numargs; i++)
        {
            if(cmd->args[i][0] == '-')
            {
                // printf("minus\n");
                if(cmd->args[i][1] == 'a')
                {
                    // printf("a\n");
                    a_flag = 1;
                }
                else if(cmd->args[i][1] == 'l')
                {
                    // printf("l\n");
                    l_flag = 1;
                }
                else
                {
                    printf(RED "ERROR: INVALID FLAG '%s'\n" reset, cmd->args[i]);
                    return;
                }

                if(cmd->args[i][2] != '\0')
                {
                    // printf("two flags\n");
                    if(cmd->args[i][2] == 'a')
                    {
                        a_flag = 1;
                    }
                    else if(cmd->args[i][2] == 'l')
                    {
                        l_flag = 1;
                    }
                    else 
                    {
                        printf(RED "ERROR: INVALID FLAG'%s'\n" reset, cmd->args[i]);
                        return;
                    }
                }
            }
            else
            {

                path = cmd->args[i];

                // Use realpath to get the absolute path
                if (realpath(path, absolute_path) == NULL) {
                    perror(RED "realpath" reset);
                    return;
                }
                
            }
        }

        if(strcmp(path, "") == 0)
        {
            strcat(path, ".");
            if (realpath(path, absolute_path) == NULL) 
            {
                perror(RED "realpath" reset);
                return;
            }
        }

        // printf("path: %s\n", path);

        Peek(absolute_path, a_flag, l_flag);
        //printf("a = %d, l = %d\n", a_flag, l_flag);

        if(redirect_flag == 1 || redirect_flag == 2 || redirect_flag == 4 || redirect_flag == 5)
        {
            freopen("/dev/tty", "w", stdout); 
            free(output_file);
        }
        if(redirect_flag == 3 || redirect_flag == 4 || redirect_flag == 5)
        {
            freopen("/dev/tty", "r", stdin);
            free(input_file);
        }   
    }
    else if(strcmp(cmd->command, "pastevents") == 0)
    {
        if(redirect_flag == 1 || redirect_flag == 2 || redirect_flag == 3 || redirect_flag == 4 || redirect_flag == 5)
        {
            //printf("cmd: %s, cmd->args[0]: %s\n", cmd->command, cmd->args[0]);
            void* err;
            void* errout;
            void* errin;

            if(redirect_flag == 1)
            {
                err = freopen(output_file, "w", stdout); 
                if(err == NULL)
                {
                    perror(RED"freopen"reset);
                    return;
                }
            }
            else if(redirect_flag == 2)
            {
                err = freopen(output_file, "a+", stdout); 
                if(err == NULL)
                {
                    perror(RED"freopen"reset);
                    return;
                }
            }
            else if(redirect_flag == 3)
            {
                // printf("%s\n", input_file);
                err = freopen(input_file, "r", stdin);
                if(err == NULL)
                {
                    perror(RED"freopen"reset);
                    return;
                }
            }
            else if (redirect_flag == 4)
            {
                // printf("out: %s\n", output_file);
                // printf("in: %s\n", input_file);
                errout = freopen(output_file, "a+", stdout); 
                errin = freopen(input_file, "r", stdin);
                if(errout == NULL || errin == NULL)
                {
                    perror(RED"freopen"reset);
                    return;
                }
            }
            else if(redirect_flag == 5)
            {
                // printf("out: %s\n", output_file);
                // printf("in: %s\n", input_file);
                errout = freopen(output_file, "w", stdout); 
                errin = freopen(input_file, "r", stdin);
                if(errout == NULL || errin == NULL)
                {
                    perror(RED"freopen"reset);
                    return;
                }
            }

            chmod(err, 0644);
        }

        if(cmd->args[0][0] == '\0') //printf pastevents file
        {
            PastPrint(parsefilepath);
        }
        else if(strcmp(cmd->args[0], "purge") == 0) //open file for read and close
        {
            PastPurge(parsefilepath);
        }
        else if(strcmp(cmd->args[0], "execute") == 0)
        {
            if(strlen(cmd->args[1]) == 1 || strlen(cmd->args[1]) == 2)
            {
                //printf("a\n");
                int address;
                if(cmd->args[1][0] > 48 && cmd->args[1][0] < 58)
                {
                   // printf("b\n");
                    address = (int)(cmd->args[1][0] - 48);

                    if(cmd->args[1][1] != '\0')
                    {
                        if(cmd->args[1][1] >= 48 && cmd->args[1][1] < 58)
                        {
                            address = address*10 + (int)(cmd->args[1][1] - 48);
                            if(address > 15 || address < 1)
                            {
                                printf(RED "ERROR: pastevent %d not available(max: 15)'\n"reset, address);
                                return;
                            }
                        }
                    }
                    //printf("add: %d\n", address);
                    FindPastEvent(strings, count, cmd, address, parsefilepath, invokeddir, pwd, timetaken, process);
                }
                else
                {
                    fprintf(stderr, RED"ERROR: Invalid flag for pastevents execute '%s'"reset, cmd->args[1]);
                    return;
                }
            }
            else
            {
                //printf("d\n");
                printf(RED "ERROR: Invalid flag for pastevents '%s'\n" reset, cmd->args[1]);
                return;
            }
        }
        else
        {
            printf(RED "ERROR: Invalid pastevents command\n(Valid commands: 'pastevents', 'pastevents purge', 'pastevents execute *number*'\n"reset);
            return;
        }
        if(redirect_flag == 1 || redirect_flag == 2 || redirect_flag == 4 || redirect_flag == 5)
        {
            freopen("/dev/tty", "w", stdout); 
            free(output_file);
        }
        if(redirect_flag == 3 || redirect_flag == 4 || redirect_flag == 5)
        {
            freopen("/dev/tty", "r", stdin);
            free(input_file);
        }   
    }
    else if(strcmp(cmd->command, "proclore") == 0)
    {
        if(redirect_flag == 1 || redirect_flag == 2 || redirect_flag == 3 || redirect_flag == 4 || redirect_flag == 5)
        {
            //printf("cmd: %s, cmd->args[0]: %s\n", cmd->command, cmd->args[0]);
            void* err;
            void* errout;
            void* errin;

            if(redirect_flag == 1)
            {
                err = freopen(output_file, "w", stdout); 
                if(err == NULL)
                {
                    perror(RED"freopen"reset);
                    return;
                }
            }
            else if(redirect_flag == 2)
            {
                err = freopen(output_file, "a+", stdout); 
                if(err == NULL)
                {
                    perror(RED"freopen"reset);
                    return;
                }
            }
            else if(redirect_flag == 3)
            {
                // printf("%s\n", input_file);
                err = freopen(input_file, "r", stdin);
                if(err == NULL)
                {
                    perror(RED"freopen"reset);
                    return;
                }
            }
            else if (redirect_flag == 4)
            {
                // printf("out: %s\n", output_file);
                // printf("in: %s\n", input_file);
                errout = freopen(output_file, "a+", stdout); 
                errin = freopen(input_file, "r", stdin);
                if(errout == NULL || errin == NULL)
                {
                    perror(RED"freopen"reset);
                    return;
                }
            }
            else if(redirect_flag == 5)
            {
                // printf("out: %s\n", output_file);
                // printf("in: %s\n", input_file);
                errout = freopen(output_file, "w", stdout); 
                errin = freopen(input_file, "r", stdin);
                if(errout == NULL || errin == NULL)
                {
                    perror(RED"freopen"reset);
                    return;
                }
            }

            chmod(err, 0644);
        }
            //check for pid process
            //error if incorrect pid
        char* filepath = malloc(sizeof(char)*MAX_PATH_LEN);

        filepath[0] = '\0';

        if(cmd->args[0][0] == '\0')
        {
            //printf("hello\n");
            strcat(filepath, "/proc/");
            strcat(filepath, "self");
            strcat(filepath, "/stat");
        }
        else
        {
            strcat(filepath, "/proc/");
            strcat(filepath, cmd->args[0]);
            strcat(filepath, "/stat");
        }
        //printf("path: %s\n", filepath);

        FILE *fd;
        fd = fopen(filepath, "r");
        if(fd == NULL)
        {
            perror(RED"open"reset);
            return;
        }


        char buf[1000];
        int pid;
        char status;
        int progrp;
        int tpgid;
        long virmem;
        
        fscanf(fd, "%d %*s %c %*d %d %*d %*d %d %*u %*lu %*lu %*lu %*lu %*lu %*lu %*ld %*ld %*ld %*ld %*ld %*ld %*llu %lu", &pid, &status, &tpgid, &progrp, &virmem);
        
        if(progrp == tpgid)
        {
            printf("PID : %d\nProcess Status: %c+\nProcess Group : %d\nVirtual Memory : %lu\n", pid, status, progrp,virmem);
        }
        else
        {
            printf("PID : %d\nProcess Status: %c\nProcess Group : %d\nVirtual Memory : %lu\n", pid, status, progrp, virmem);
        }


        free(filepath);

        char exefile[MAX_PATH_LEN];
        snprintf(exefile, sizeof(exefile), "/proc/%d/exe", pid);

        char respath[MAX_PATH_LEN];

        int readcount;
        readcount = readlink(exefile, respath, sizeof(respath) - 1);
        
        if(readcount == -1)
        {
            perror(RED"Error reading symbolic link"reset);
            return;
        }

        respath[readcount] = '\0';

        if(strncmp(respath, invokeddir, strlen(invokeddir)) == 0)
        {
            int len = strlen(invokeddir);
            char* relpath = malloc(sizeof(char)*MAX_PATH_LEN);
            relpath[0] = '~';
            for(int i = len; i < MAX_PATH_LEN; i ++) 
            //um??????youre putting the rest of the entire instruction as the path there could be redirects or pipes. apparently not.
            {
                relpath[i - len + 1] = respath[i];
                if(respath[i] == '\0')
                {
                    break;
                }
            }

            printf("Executable Path: %s\n", relpath);
            free(relpath);
        }
        else
        {
            printf("Executable Path: %s\n", respath);
        }

        if(redirect_flag == 1 || redirect_flag == 2 || redirect_flag == 4 || redirect_flag == 5)
        {
            freopen("/dev/tty", "w", stdout); 
            free(output_file);
        }
        if(redirect_flag == 3 || redirect_flag == 4 || redirect_flag == 5)
        {
            freopen("/dev/tty", "r", stdin);
            free(input_file);
        }   
        
    }
    else if(strcmp(cmd->command, "seek") == 0)
    {
        if(redirect_flag == 1 || redirect_flag == 2 || redirect_flag == 3 || redirect_flag == 4 || redirect_flag == 5)
        {
            //printf("cmd: %s, cmd->args[0]: %s\n", cmd->command, cmd->args[0]);
            void* err;
            void* errout;
            void* errin;

            if(redirect_flag == 1)
            {
                err = freopen(output_file, "w", stdout); 
                if(err == NULL)
                {
                    perror(RED"freopen"reset);
                    return;
                }
            }
            else if(redirect_flag == 2)
            {
                err = freopen(output_file, "a+", stdout); 
                if(err == NULL)
                {
                    perror(RED"freopen"reset);
                    return;
                }
            }
            else if(redirect_flag == 3)
            {
                // printf("%s\n", input_file);
                err = freopen(input_file, "r", stdin);
                if(err == NULL)
                {
                    perror(RED"freopen"reset);
                    return;
                }
            }
            else if (redirect_flag == 4)
            {
                // printf("out: %s\n", output_file);
                // printf("in: %s\n", input_file);
                errout = freopen(output_file, "a+", stdout); 
                errin = freopen(input_file, "r", stdin);
                if(errout == NULL || errin == NULL)
                {
                    perror(RED"freopen"reset);
                    return;
                }
            }
            else if(redirect_flag == 5)
            {
                // printf("out: %s\n", output_file);
                // printf("in: %s\n", input_file);
                errout = freopen(output_file, "w", stdout); 
                errin = freopen(input_file, "r", stdin);
                if(errout == NULL || errin == NULL)
                {
                    perror(RED"freopen"reset);
                    return;
                }
            }

            chmod(err, 0644);
        }
        int d_flag = 0;
        int f_flag = 0;
        int e_flag = 0;
        int target = 0;
        char* target_name;
        char* searchdirectory;

        //printf("seeking\n");
        target_name;
        searchdirectory = malloc(MAX_PATH_LEN);
        searchdirectory[0] = '\0';
        //printf("hi\n");

        char* path = malloc(sizeof(char)*(MAX_PATH_LEN));
        path[0] = '\0';

        for(int i = 0 ; i < numargs; i++)
        {
            if(cmd->args[i][0] == '-')
            {
                if(cmd->args[i][1] == 'f')
                {
                    f_flag = 1;
                }
                else if(cmd->args[i][1] == 'd')
                {
                    d_flag = 1;
                }
                else if(cmd->args[i][1] == 'e')
                {
                    e_flag = 1;
                }

                if(cmd->args[i][2] != '\0')
                {
                    if(cmd->args[i][2] == 'f')
                    {
                        if(cmd->args[i][1] == 'd')
                        {
                            fprintf(stderr, RED"ERROR: INVALID FLAG '%s'\n" reset, cmd->args[i]);
                            return;
                        }
                        else if(cmd->args[i][1] != 'e')
                        {
                            fprintf(stderr, RED"ERROR: INVALID FLAG '%s'\n" reset, cmd->args[i]);
                            return;
                        }
                    }
                    else if(cmd->args[i][2] == 'd')
                    {
                        if(cmd->args[i][1] == 'f')
                        {
                            fprintf(stderr, RED"ERROR: INVALID FLAG '%s'\n" reset, cmd->args[i]);
                            return;
                        }
                        else if(cmd->args[i][1] != 'e')
                        {
                            fprintf(stderr, RED"ERROR: INVALID FLAG '%s'\n" reset, cmd->args[i]);
                            return;
                        }
                    }
                    else if(cmd->args[i][2] == 'e')
                    {
                        e_flag = 1;
                        if(cmd->args[i][3] != '\0')
                        {
                            fprintf(stderr, RED"ERROR: INVALID FLAG '%s'\n" reset, cmd->args[i]);
                            return;
                        }
                    }
                    else 
                    {
                        fprintf(stderr, RED"ERROR: INVALID FLAG '%s'\n" reset, cmd->args[i]);
                        return;
                    }
                }
            }
            else if(target == 0)
            {
                target_name = cmd->args[i];
                target = 1;
            }
            else
            {
                path = cmd->args[i];
                //printf("%s\n", path);

                // Use realpath to get the absolute path
                if (realpath(path, searchdirectory) == NULL) 
                {
                    perror(RED"realpath"reset);
                    return;
                }
            }
    
        }

        if(target == 0)
        {
            fprintf(stderr, RED"ERROR: No search term inputted\n" reset);
            return;
        }
       // printf("f: %d\nd: %d\n", f_flag, d_flag);

        if(path[0] == '\0')
        {
            path = invokeddir;
            //printf("%s\n", path);

            // Use realpath to get the absolute path
            if (realpath(path, searchdirectory) == NULL) {
                perror(RED"realpath"reset);
                return;
            }
        }

        Seek(cmd, target_name, searchdirectory, invokeddir, pwd, f_flag, d_flag, e_flag);

        free(searchdirectory);

        if(redirect_flag == 1 || redirect_flag == 2 || redirect_flag == 4 || redirect_flag == 5)
        {
            freopen("/dev/tty", "w", stdout); 
            free(output_file);
        }
        if(redirect_flag == 3 || redirect_flag == 4 || redirect_flag == 5)
        {
            freopen("/dev/tty", "r", stdin);
            free(input_file);
        }   
    }
    else if(strcmp(cmd->command, "iMan") == 0)
    {
        if(redirect_flag == 1 || redirect_flag == 2 || redirect_flag == 3 || redirect_flag == 4 || redirect_flag == 5)
        {
            //printf("cmd: %s, cmd->args[0]: %s\n", cmd->command, cmd->args[0]);
            void* err;
            void* errout;
            void* errin;

            if(redirect_flag == 1)
            {
                err = freopen(output_file, "w", stdout); 
                if(err == NULL)
                {
                    perror(RED"freopen"reset);
                    return;
                }
            }
            else if(redirect_flag == 2)
            {
                err = freopen(output_file, "a+", stdout); 
                if(err == NULL)
                {
                    perror(RED"freopen"reset);
                    return;
                }
            }
            else if(redirect_flag == 3)
            {
                // printf("%s\n", input_file);
                err = freopen(input_file, "r", stdin);
                if(err == NULL)
                {
                    perror(RED"freopen"reset);
                    return;
                }
            }
            else if (redirect_flag == 4)
            {
                // printf("out: %s\n", output_file);
                // printf("in: %s\n", input_file);
                errout = freopen(output_file, "a+", stdout); 
                errin = freopen(input_file, "r", stdin);
                if(errout == NULL || errin == NULL)
                {
                    perror(RED"freopen"reset);
                    return;
                }
            }
            else if(redirect_flag == 5)
            {
                // printf("out: %s\n", output_file);
                // printf("in: %s\n", input_file);
                errout = freopen(output_file, "w", stdout); 
                errin = freopen(input_file, "r", stdin);
                if(errout == NULL || errin == NULL)
                {
                    perror(RED"freopen"reset);
                    return;
                }
            }

            chmod(err, 0644);
        }

        GetTheMan(cmd->args[0]);

        if(redirect_flag == 1 || redirect_flag == 2 || redirect_flag == 4 || redirect_flag == 5)
        {
            freopen("/dev/tty", "w", stdout); 
            free(output_file);
        }
        if(redirect_flag == 3 || redirect_flag == 4 || redirect_flag == 5)
        {
            freopen("/dev/tty", "r", stdin);
            free(input_file);
        }   
    }
    else if(strcmp(cmd->command, "neonate") == 0)
    {
        if(redirect_flag == 1 || redirect_flag == 2 || redirect_flag == 3 || redirect_flag == 4 || redirect_flag == 5)
        {
            //printf("cmd: %s, cmd->args[0]: %s\n", cmd->command, cmd->args[0]);
            void* err;
            void* errout;
            void* errin;

            if(redirect_flag == 1)
            {
                err = freopen(output_file, "w", stdout); 
                if(err == NULL)
                {
                    perror(RED"freopen"reset);
                    return;
                }
            }
            else if(redirect_flag == 2)
            {
                err = freopen(output_file, "a+", stdout); 
                if(err == NULL)
                {
                    perror(RED"freopen"reset);
                    return;
                }
            }
            else if(redirect_flag == 3)
            {
                // printf("%s\n", input_file);
                err = freopen(input_file, "r", stdin);
                if(err == NULL)
                {
                    perror(RED"freopen"reset);
                    return;
                }
            }
            else if (redirect_flag == 4)
            {
                // printf("out: %s\n", output_file);
                // printf("in: %s\n", input_file);
                errout = freopen(output_file, "a+", stdout); 
                errin = freopen(input_file, "r", stdin);
                if(errout == NULL || errin == NULL)
                {
                    perror(RED"freopen"reset);
                    return;
                }
            }
            else if(redirect_flag == 5)
            {
                // printf("out: %s\n", output_file);
                // printf("in: %s\n", input_file);
                errout = freopen(output_file, "w", stdout); 
                errin = freopen(input_file, "r", stdin);
                if(errout == NULL || errin == NULL)
                {
                    perror(RED"freopen"reset);
                    return;
                }
            }

            chmod(err, 0644);
        }

        int time = 0;
        if(cmd->args[0][0] == '\0')
        {
            fprintf(stderr, RED"ERROR: Flag missing\n"reset);
            return;
        }
        else if(strcmp(cmd->args[0], "-n") == 0)
        {
            if(cmd->args[1][0] == '\0')
            {
                fprintf(stderr, RED"Time interval not specified\nCommand format: 'neonate -n *time in seconds*'\n"reset);
                return;
            }
            else 
            {
                int a = 0;
                while(cmd->args[1][a] != '\0')
                {
                    // printf("checking: %c\n", cmd->args[1][a]);
                    if(!isdigit(cmd->args[1][a++]))
                    {
                        fprintf(stderr, RED"Non integer not allowed as time interval\nCommand format: 'neonate -n *time in seconds*'\n"reset);
                        return;
                    }
                }

                time = (atoi)(cmd->args[1]);
                // printf("time = %d\n", time);
                neonate(time);
            }
        }
        else
        {
            fprintf(stderr, RED"ERROR: Incorrect flag: please specify '-n'\n"reset);
            return;
        }
        if(redirect_flag == 1 || redirect_flag == 2 || redirect_flag == 4 || redirect_flag == 5)
        {
            freopen("/dev/tty", "w", stdout); 
            free(output_file);
        }
        if(redirect_flag == 3 || redirect_flag == 4 || redirect_flag == 5)
        {
            freopen("/dev/tty", "r", stdin);
            free(input_file);
        }   
    }
    else if(strcmp(cmd->command, "activities") == 0)
    {
        if(redirect_flag == 1 || redirect_flag == 2 || redirect_flag == 3 || redirect_flag == 4 || redirect_flag == 5)
        {
            //printf("cmd: %s, cmd->args[0]: %s\n", cmd->command, cmd->args[0]);
            void* err;
            void* errout;
            void* errin;

            if(redirect_flag == 1)
            {
                err = freopen(output_file, "w", stdout); 
                if(err == NULL)
                {
                    perror(RED"freopen"reset);
                    return;
                }
            }
            else if(redirect_flag == 2)
            {
                err = freopen(output_file, "a+", stdout); 
                if(err == NULL)
                {
                    perror(RED"freopen"reset);
                    return;
                }
            }
            else if(redirect_flag == 3)
            {
                // printf("%s\n", input_file);
                err = freopen(input_file, "r", stdin);
                if(err == NULL)
                {
                    perror(RED"freopen"reset);
                    return;
                }
            }
            else if (redirect_flag == 4)
            {
                // printf("out: %s\n", output_file);
                // printf("in: %s\n", input_file);
                errout = freopen(output_file, "a+", stdout); 
                errin = freopen(input_file, "r", stdin);
                if(errout == NULL || errin == NULL)
                {
                    perror(RED"freopen"reset);
                    return;
                }
            }
            else if(redirect_flag == 5)
            {
                // printf("out: %s\n", output_file);
                // printf("in: %s\n", input_file);
                errout = freopen(output_file, "w", stdout); 
                errin = freopen(input_file, "r", stdin);
                if(errout == NULL || errin == NULL)
                {
                    perror(RED"freopen"reset);
                    return;
                }
            }

            chmod(err, 0644);
        }

        if(cmd->argcount > 0)
        {
            fprintf(stderr, RED"'activties' does not support arguments\n"reset);
            return;
        }
        else
        {
            activities();
        }

        if(redirect_flag == 1 || redirect_flag == 2 || redirect_flag == 4 || redirect_flag == 5)
        {
            freopen("/dev/tty", "w", stdout); 
            free(output_file);
        }
        if(redirect_flag == 3 || redirect_flag == 4 || redirect_flag == 5)
        {
            freopen("/dev/tty", "r", stdin);
            free(input_file);
        }   
    }
    else if(strcmp(cmd->command, "ping") == 0)
    {
        if(redirect_flag == 1 || redirect_flag == 2 || redirect_flag == 3 || redirect_flag == 4 || redirect_flag == 5)
        {
            //printf("cmd: %s, cmd->args[0]: %s\n", cmd->command, cmd->args[0]);
            void* err;
            void* errout;
            void* errin;

            if(redirect_flag == 1)
            {
                err = freopen(output_file, "w", stdout); 
                if(err == NULL)
                {
                    perror(RED"freopen"reset);
                    return;
                }
            }
            else if(redirect_flag == 2)
            {
                err = freopen(output_file, "a+", stdout); 
                if(err == NULL)
                {
                    perror(RED"freopen"reset);
                    return;
                }
            }
            else if(redirect_flag == 3)
            {
                // printf("%s\n", input_file);
                err = freopen(input_file, "r", stdin);
                if(err == NULL)
                {
                    perror(RED"freopen"reset);
                    return;
                }
            }
            else if (redirect_flag == 4)
            {
                // printf("out: %s\n", output_file);
                // printf("in: %s\n", input_file);
                errout = freopen(output_file, "a+", stdout); 
                errin = freopen(input_file, "r", stdin);
                if(errout == NULL || errin == NULL)
                {
                    perror(RED"freopen"reset);
                    return;
                }
            }
            else if(redirect_flag == 5)
            {
                // printf("out: %s\n", output_file);
                // printf("in: %s\n", input_file);
                errout = freopen(output_file, "w", stdout); 
                errin = freopen(input_file, "r", stdin);
                if(errout == NULL || errin == NULL)
                {
                    perror(RED"freopen"reset);
                    return;
                }
            }

            chmod(err, 0644);
        }

        int pid;
        int sig;

        if(cmd->args[0][0] != '\0')
        {
            pid = atoi(cmd->args[0]);
            if(cmd->args[1][0] != '\0')
            {
                sig = atoi(cmd->args[1]);

                sig = sig % 32;

                if(sig == 0)
                {
                    fprintf(stderr, RED"ERROR: Invalid signal '0'\n"reset);
                    return;
                }

                //open stat file for process
                char* filepath = malloc(sizeof(char)*MAX_PATH_LEN);

                filepath[0] = '\0';

                strcat(filepath, "/proc/");

                char buf[10];
                memset(buf, 0, 10);
                sprintf(buf, "%d", pid);

                strcat(filepath, buf);
                strcat(filepath, "/stat");

                FILE *fd;
                fd = fopen(filepath, "r");
                if(fd == NULL)
                {
                    fprintf(stderr, RED"ERROR: Process has exited or does not exist\n"reset);
                    return;
                }
                fclose(fd);
                free(filepath);

                kill(pid, sig); 
            }
            else
            {
                fprintf(stderr, RED"ERROR: Must specify signal\n"reset);
                return;
            }
        }
        else
        {
            fprintf(stderr, RED"ERROR: Must specify process and signal\n"reset);
            return;
        }

        if(redirect_flag == 1 || redirect_flag == 2 || redirect_flag == 4 || redirect_flag == 5)
        {
            freopen("/dev/tty", "w", stdout); 
            free(output_file);
        }
        if(redirect_flag == 3 || redirect_flag == 4 || redirect_flag == 5)
        {
            freopen("/dev/tty", "r", stdin);
            free(input_file);
        }   
    }
    else if(strcmp(cmd->command, "fg") == 0)
    {
        if(redirect_flag == 1 || redirect_flag == 2 || redirect_flag == 3 || redirect_flag == 4 || redirect_flag == 5)
        {
            //printf("cmd: %s, cmd->args[0]: %s\n", cmd->command, cmd->args[0]);
            void* err;
            void* errout;
            void* errin;

            if(redirect_flag == 1)
            {
                err = freopen(output_file, "w", stdout); 
                if(err == NULL)
                {
                    perror(RED"freopen"reset);
                    return;
                }
            }
            else if(redirect_flag == 2)
            {
                err = freopen(output_file, "a+", stdout); 
                if(err == NULL)
                {
                    perror(RED"freopen"reset);
                    return;
                }
            }
            else if(redirect_flag == 3)
            {
                // printf("%s\n", input_file);
                err = freopen(input_file, "r", stdin);
                if(err == NULL)
                {
                    perror(RED"freopen"reset);
                    return;
                }
            }
            else if (redirect_flag == 4)
            {
                // printf("out: %s\n", output_file);
                // printf("in: %s\n", input_file);
                errout = freopen(output_file, "a+", stdout); 
                errin = freopen(input_file, "r", stdin);
                if(errout == NULL || errin == NULL)
                {
                    perror(RED"freopen"reset);
                    return;
                }
            }
            else if(redirect_flag == 5)
            {
                // printf("out: %s\n", output_file);
                // printf("in: %s\n", input_file);
                errout = freopen(output_file, "w", stdout); 
                errin = freopen(input_file, "r", stdin);
                if(errout == NULL || errin == NULL)
                {
                    perror(RED"freopen"reset);
                    return;
                }
            }

            chmod(err, 0644);
        }

        int arg_pid;
        int status;
        if(cmd->args[0][0] != '\0')
        {
            arg_pid = atoi(cmd->args[0]);
        }
        else
        {
            fprintf(stderr, RED"ERROR: Process ID not specified\n"reset);
            return;
        }

        kill(arg_pid, SIGCONT);

        RemoveNode(bgCheckList, arg_pid);

        
        char path[MAX_PATH_LEN];
        char* exePath = calloc(300, 1);
        char* command;
        ssize_t len;

        snprintf(path, MAX_PATH_LEN, "/proc/%d/exe", arg_pid);

        len = readlink(path, exePath, MAX_PATH_LEN - 1);

        if (len == -1) 
        {
            fprintf(stderr, RED"ERROR: Process has exited or does not exist\n"reset);
            return;
        }

        exePath[len] = '\0';

        command = strrchr(exePath, '/');

        strcpy(process, command + 1);

        while(waitpid(arg_pid, &status, WNOHANG) == 0)
        {    
            // printf("int: %d\n", interrupted);
            if(cntc == 1)
            {
                printf("\n");
                cntc = 0;
                break;
            }
            if(cntz == 1)
            {
                cntz = 0;
                char path[MAX_PATH_LEN];
                char* exePath = calloc(300, 1);
                char* command;
                ssize_t len;

                snprintf(path, MAX_PATH_LEN, "/proc/%d/exe", arg_pid);

                len = readlink(path, exePath, MAX_PATH_LEN - 1);
                if (len == -1) 
                {
                    fprintf(stderr, RED"ERROR: Process has exited or does not exist\n"reset);
                    return;
                }

                exePath[len] = '\0';

                command = strrchr(exePath, '/');
                // printf("command: %s\n", command + 1);
                AddNode(bgCheckList, command + 1, arg_pid);

                kill(arg_pid, SIGTSTP);
                printf("\n");
                break;
            }
        }

        end_time = time(NULL);
        cpu_end = clock();

        time_t t = (int)(difftime(end_time, start_time));
        *timetaken = t;

        if(redirect_flag == 1 || redirect_flag == 2 || redirect_flag == 4 || redirect_flag == 5)
        {
            freopen("/dev/tty", "w", stdout); 
            free(output_file);
        }
        if(redirect_flag == 3 || redirect_flag == 4 || redirect_flag == 5)
        {
            freopen("/dev/tty", "r", stdin);
            free(input_file);
        }   
    }
    else if(strcmp(cmd->command, "bg") == 0)
    {
        if(redirect_flag == 1 || redirect_flag == 2 || redirect_flag == 3 || redirect_flag == 4 || redirect_flag == 5)
        {
            //printf("cmd: %s, cmd->args[0]: %s\n", cmd->command, cmd->args[0]);
            void* err;
            void* errout;
            void* errin;

            if(redirect_flag == 1)
            {
                err = freopen(output_file, "w", stdout); 
                if(err == NULL)
                {
                    perror(RED"freopen"reset);
                    return;
                }
            }
            else if(redirect_flag == 2)
            {
                err = freopen(output_file, "a+", stdout); 
                if(err == NULL)
                {
                    perror(RED"freopen"reset);
                    return;
                }
            }
            else if(redirect_flag == 3)
            {
                // printf("%s\n", input_file);
                err = freopen(input_file, "r", stdin);
                if(err == NULL)
                {
                    perror(RED"freopen"reset);
                    return;
                }
            }
            else if (redirect_flag == 4)
            {
                // printf("out: %s\n", output_file);
                // printf("in: %s\n", input_file);
                errout = freopen(output_file, "a+", stdout); 
                errin = freopen(input_file, "r", stdin);
                if(errout == NULL || errin == NULL)
                {
                    perror(RED"freopen"reset);
                    return;
                }
            }
            else if(redirect_flag == 5)
            {
                // printf("out: %s\n", output_file);
                // printf("in: %s\n", input_file);
                errout = freopen(output_file, "w", stdout); 
                errin = freopen(input_file, "r", stdin);
                if(errout == NULL || errin == NULL)
                {
                    perror(RED"freopen"reset);
                    return;
                }
            }

            chmod(err, 0644);
        }

        int arg_pid;
        int status;
        if(cmd->args[0][0] != '\0')
        {
            arg_pid = atoi(cmd->args[0]);
        }
        else
        {
            fprintf(stderr, RED"ERROR: Process ID not specified\n"reset);
            return;
        }

        char path[MAX_PATH_LEN];
        char* exePath = calloc(300, 1);
        char* command;

        snprintf(path, MAX_PATH_LEN, "/proc/%d/exe", arg_pid);

        if (readlink(path, exePath, MAX_PATH_LEN - 1) == -1) 
        {
            fprintf(stderr, RED"ERROR: Process has exited or does not exist\n"reset);
            return;
        }

        kill(arg_pid, SIGCONT);

        if(redirect_flag == 1 || redirect_flag == 2 || redirect_flag == 4 || redirect_flag == 5)
        {
            freopen("/dev/tty", "w", stdout); 
            free(output_file);
        }
        if(redirect_flag == 3 || redirect_flag == 4 || redirect_flag == 5)
        {
            freopen("/dev/tty", "r", stdin);
            free(input_file);
        }   
    }
    else if(strcmp(cmd->command, "exit") == 0)
    {
        exit(EXIT_SUCCESS);
    }
    else
    {
        // printf("else condition, process: %s\n", process);
        // printf("red: %d\n", redirect_flag);
        if(redirect_flag == 1 || redirect_flag == 2 || redirect_flag == 3 || redirect_flag == 4 || redirect_flag == 5)
        {
            //printf("cmd: %s, cmd->args[0]: %s\n", cmd->command, cmd->args[0]);
            void* err;
            void* errout;
            void* errin;

            if(redirect_flag == 1)
            {
                err = freopen(output_file, "w", stdout); 
                if(err == NULL)
                {
                    perror(RED"freopen"reset);
                    return;
                }
            }
            else if(redirect_flag == 2)
            {
                err = freopen(output_file, "a+", stdout); 
                if(err == NULL)
                {
                    perror(RED"freopen"reset);
                    return;
                }
            }
            else if(redirect_flag == 3)
            {
                // printf("%s\n", input_file);
                err = freopen(input_file, "r", stdin);
                if(err == NULL)
                {
                    perror(RED"freopen"reset);
                    return;
                }
            }
            else if (redirect_flag == 4)
            {
                // printf("out: %s\n", output_file);
                // printf("in: %s\n", input_file);
                errout = freopen(output_file, "a+", stdout); 
                errin = freopen(input_file, "r", stdin);
                if(errout == NULL || errin == NULL)
                {
                    perror(RED"freopen"reset);
                    return;
                }
            }
            else if(redirect_flag == 5)
            {
                // printf("out: %s\n", output_file);
                // printf("in: %s\n", input_file);
                errout = freopen(output_file, "w", stdout); 
                errin = freopen(input_file, "r", stdin);
                if(errout == NULL || errin == NULL)
                {
                    perror(RED"freopen"reset);
                    return;
                }
            }

            chmod(err, 0644);
        }

        start_time = time(NULL);
        cpu_start = clock();

        //execvp
        pid_t pid = fork();

        if(pid == -1)
        {
            perror(RED"fork"reset);
            return;
        }
        else if(pid == 0)
        {
            int ret;
            //make an array with arguments, the last one being null
            char** argarray = malloc(sizeof(char*)*(numargs+2));

            argarray[0] = strdup(cmd->command);


            //printf("0: %s\n", argarray[0]);
            for(int i = 1; i < numargs + 1; i++)
            {
                //printf("arg%d: %s\n", i - 1, cmd->args[i - 1]);
                argarray[i] = strdup(cmd->args[i - 1]);
                //printf("arg[%d]: %s\n", i, argarray[i]);
            }

            argarray[numargs + 1] = NULL;
    
            ret = execvp (cmd->command, argarray);
            fprintf(stderr, RED"ERROR: '%s' is not a valid command\n"reset, cmd->command);
            
            exit(EXIT_SUCCESS);
        }
        else if(flag == 1 && pid > 0) //run in fg
        {
            int prid;
            // printf("foreground\n");
            
            while(waitpid(pid, &prid, WNOHANG) == 0)
            {    
                // printf("c: %d, z: %d\n", cntc, cntz);
                if(cntc == 1)
                {
                    printf("\n");
                    cntc = 0;
                    break;
                }
                if(cntz == 1)
                {
                    // printf("CONTROL Z\n");
                    cntz = 0;
                    //get command with pid instead of cmd->command
                    char path[MAX_PATH_LEN];
                    char* exePath = calloc(300, 1);
                    char* command;
                    ssize_t len;

                    snprintf(path, MAX_PATH_LEN, "/proc/%d/exe", pid);

                    len = readlink(path, exePath, MAX_PATH_LEN - 1);
                    if (len == -1) 
                    {
                        fprintf(stderr, RED"ERROR: Process has exited or does not exist\n"reset);
                        return;
                    }

                    exePath[len] = '\0';

                    command = strrchr(exePath, '/');
                    // printf("command: %s\n", command + 1);
                    AddNode(bgCheckList, command + 1, pid);

                    kill(pid, SIGTSTP);
                    printf("\n");
                    break;
                }
            }
            
            end_time = time(NULL);
            cpu_end = clock();

            time_t t = (int)(difftime(end_time, start_time));
            
            *timetaken = t;
        }
    //fg normal fork, manhandle child, print time if > 2, for bg: process name and id into ll, 
    //in main after prompt do it pid WNOHANG for every process in ll and if complete
    //remove from ll and print pid and executed
        else if(flag == 0 && pid > 0) //run in bg
        {
            printf("%d\n", pid);

            AddNode(bgCheckList, cmd->command, pid);
        }


        if(redirect_flag == 1 || redirect_flag == 2 || redirect_flag == 4 || redirect_flag == 5)
        {
            freopen("/dev/tty", "w", stdout); 
            free(output_file);
        }
        if(redirect_flag == 3 || redirect_flag == 4 || redirect_flag == 5)
        {
            freopen("/dev/tty", "r", stdin);
            free(input_file);
        }   

    }
    // printf("process: %s\n", process);
}


void Peek(char* path, int a_flag, int l_flag)
{

    struct dirent **stdir;
    //printf("abs path: %s\n", path);

    int n = scandir(path, &stdir, NULL, alphasort);
    if(n == -1)
    {
        perror(RED"scandir"reset);
        return;
    }

    if(l_flag != 1)
    {
        for(int i = 0; i < n; i++)
        {
            if(a_flag != 1)
            {
                if(stdir[i]->d_name[0] != '.')
                {
                    char filepath[MAX_PATH_LEN];
                    filepath[0] = '\0';
                    strcat(filepath, path);
                    strcat(filepath, "/");
                    strcat(filepath, stdir[i]->d_name);
                    //printf("file path: %s\n", filepath);

                    struct stat* filestat = malloc(sizeof(struct stat));
                    stat(filepath, filestat);

                    if(access(filepath, X_OK) == 0)
                    {
                        printf(GREEN"%s\n"reset, stdir[i]->d_name); 
                    }
                    else if(S_ISREG(filestat->st_mode))
                    {
                        printf(WHITE"%s\n"reset, stdir[i]->d_name);
                    }
                    else if(S_ISDIR(filestat->st_mode))
                    {
                        printf(BLUE"%s\n"reset, stdir[i]->d_name);
                    }
                    free(filestat);
                }
                free(stdir[i]);
            }
            else
            {
                char filepath[MAX_PATH_LEN];
                filepath[0] = '\0';
                strcat(filepath, path);
                strcat(filepath, "/");
                strcat(filepath, stdir[i]->d_name);
                //printf("file path: %s\n", filepath);

                struct stat* filestat = malloc(sizeof(struct stat));
                stat(filepath, filestat);
                //printf("size: %ld\n", filestat->st_size);
                if(access(filepath, X_OK) == 0)
                {
                    printf(GREEN"%s\n"reset, stdir[i]->d_name);
                }
                else if(S_ISREG(filestat->st_mode))
                {
                    printf(WHITE"%s\n"reset, stdir[i]->d_name);
                }
                else if(S_ISDIR(filestat->st_mode))
                {
                    printf(BLUE"%s\n"reset, stdir[i]->d_name);
                }
                free(filestat);
                free(stdir[i]);
            }
        }
        free(stdir);

        //!free path
    }
    else
    {   
        //printf("path:%s\n", path);

        int fd;
        fd = open(path, O_RDONLY);

        if(a_flag == 1)
        {
            for(int i = 0; i < n; i++)
            {
                // char* newpath[1] = '\0';
                // strcat(path, "/");
                // strcat(path, stdir[i]->d_name);
                //printf("path sent to fxn: %s\n", stdir[i]->d_name);
                PrintFileData(fd, stdir[i]->d_name);
            }
        }
        else
        {   
            for(int i = 0; i < n; i++)
            {
                if(stdir[i]->d_name[0] != '.')
                {
                    PrintFileData(fd, stdir[i]->d_name);
                }
            }
        }

        //!free path
    }
}

void PrintFileData(int fd, char* path)
{
    struct stat file_stat;
    if(fstatat(fd, path, &file_stat, AT_SYMLINK_NOFOLLOW) == -1)
    {
        perror(RED"fstatat"reset);
        close(fd);
        return;
    }


    printf("%s", S_ISDIR(file_stat.st_mode) ? "d" : "-");
    printf("%c%c%c%c%c%c%c%c%c",
    file_stat.st_mode & S_IRUSR ? 'r' : '-',
    file_stat.st_mode & S_IWUSR ? 'w' : '-',
    file_stat.st_mode & S_IXUSR ? 'x' : '-',
    file_stat.st_mode & S_IRGRP ? 'r' : '-',
    file_stat.st_mode & S_IWGRP ? 'w' : '-',
    file_stat.st_mode & S_IXGRP ? 'x' : '-',
    file_stat.st_mode & S_IROTH ? 'r' : '-',
    file_stat.st_mode & S_IWOTH ? 'w' : '-',
    file_stat.st_mode & S_IXOTH ? 'x' : '-');

    printf(" ");

    printf(" %2ld", file_stat.st_nlink);

    struct passwd *pw = getpwuid(file_stat.st_uid);
    if(pw == 0)
    {
        perror(RED"getpwuid"reset);
        return;
    }
    else
    {
        printf(" %s", pw->pw_name);
    }
    struct group  *gr = getgrgid(file_stat.st_gid);
    if(gr == 0)
    {
        perror(RED"getgrgid"reset);
        return;
    }
    else
    {
        printf(" %s", gr->gr_name);
    }   

    printf(" %8ld ", (long)file_stat.st_size);

    char date[30];

    time_t curr_time = time(NULL);

    struct tm* current = localtime(&curr_time);
    struct tm* lm_file = localtime(&file_stat.st_mtime);

    printf(" %d", ((lm_file->tm_year - current->tm_year)*12 + current->tm_mon - lm_file->tm_mon));

    if(((lm_file->tm_year - current->tm_year)*12 + current->tm_mon - lm_file->tm_mon) > 6)
    {
        strftime(date, 30, "%h %d %Y", localtime(&(file_stat.st_mtime)));
    }
    else
    {
        strftime(date, 30, "%h %d %H:%M", localtime(&(file_stat.st_mtime)));
    }
    printf(" %s ", date);

    //printf("size: %ld\n", filestat->st_size);
    if(access(path, X_OK) == 0)
    {
        printf(GREEN"%s\n"reset, path);
    }
    else if(S_ISREG(file_stat.st_mode))
    {
        printf(WHITE"%s\n"reset, path);
    }
    else if(S_ISDIR(file_stat.st_mode))
    {
        printf(BLUE"%s\n"reset, path);
    }
    //printf(" %s\n", path);

}

void Warp(char* dest, char* invokeddir, char** pwd)
{
    if(strcmp(dest, "-") == 0)
    {
        
        int pid = fork();

        if(pid == 0)
        {
            printf("%s\n", *pwd);
            if(pwd == NULL)
            {
                fprintf(stderr, RED"ERROR: no pwd set\n"reset);
                return;
            }
            
            char* prevdir = *pwd;
            printf("%s\n", *pwd);

            *pwd = getcwd(NULL, MAX_PATH_LEN);

            if(chdir(prevdir) != 0)
            {
                perror(RED"chdir"reset);
                return;
            }

            char* cwd;
            cwd = getcwd(NULL, MAX_PATH_LEN);
            printf("%s\n", cwd);
            free(cwd);

            exit(EXIT_SUCCESS);
        }
        else
        {
            wait(NULL);
        }
    }
    else if(dest[0] == '~')
    {
        *pwd = getcwd(NULL, MAX_PATH_LEN);
        //tilde replace with home path
        char* path = malloc(sizeof(char)*MAX_PATH_LEN);
        path[0] = '\0';
        //strcat(path, "~");
        strcat(path, invokeddir);
        
        int len = strlen(dest);
        strcat(path, dest+1);

        if(chdir(path) != 0)
        {
            perror(RED"chdir"reset);
            return;
        }
        
        char* cwd;
        cwd = getcwd(NULL, MAX_PATH_LEN);
        printf("%s\n", cwd);
        free(cwd);
        free(path);
        
    }
    else 
    {
        int pid = fork();

        if(pid == 0)
        {
            *pwd = getcwd(NULL, MAX_PATH_LEN);

            if(*pwd == 0)
            {
                perror(RED"getcwd"reset);
            }


            if(chdir(dest) != 0)
            {
                perror(RED"chdir"reset);
                return;
            }
            //printf("%s\n", dest);

            char* cwd;
            cwd = getcwd(NULL, MAX_PATH_LEN);
            printf("%s\n", cwd);
            free(cwd);
        }
        else
        {
            wait(NULL);
        }
        
    }
    //printf("pwd at end of Warp: '%s'\n", *pwd);
}

