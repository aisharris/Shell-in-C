#include "headers.h"

extern Node bgCheckList;

void activities ()
{
    // printf("in activities\n");
    List temp = bgCheckList;

    while(temp->next != NULL)
    {
        // printf("pid: %d\n", temp->next->pid);
        char* filepath = malloc(sizeof(char)*MAX_PATH_LEN);

        filepath[0] = '\0';

        strcat(filepath, "/proc/");

        char buf[10];
        memset(buf, 0, 10);

        sprintf(buf, "%d", temp->next->pid);

        strcat(filepath, buf);

        strcat(filepath, "/stat");

        // printf("filepath: %s\n", filepath);

        FILE *fd;
        fd = fopen(filepath, "r");
        if(fd == NULL)
        {
            perror(RED"open"reset);
            return;
        }

        char status;

        if(fscanf(fd, "%*d %*s %c", &status) == EOF)
        {
            perror(RED"fscanf"reset);
        }
        
        fclose(fd);
        free(filepath);
        
        char* state = calloc(100, 1);

        // printf("status: %c\n", status);
        if(status == 'T')
        {
            state = "Stopped";
        }
        else if(status == 'Z')
        {
            state = "Zombie";
            temp = temp->next;
            continue;
        }
        else
        {
            state = "Running";
        }

        printf("%d : %s - %s\n", temp->next->pid, temp->next->p_name, state);

        temp = temp-> next;
    }
}