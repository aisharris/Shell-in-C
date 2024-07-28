#include "headers.h"

List InitList()
{
    List LL = (struct LinkedList*)malloc(sizeof(struct LinkedList));
    LL->pid = 0;
    LL->p_name = malloc(sizeof(char)*6);
    LL->p_name = "DUMMY";
    LL->next = NULL;
    return LL;
}

void AddNode(List L, char* p_name, int pid)
{
    Node temp = L;

    // printf("Adding node(trying to)\n");

    while(temp->next != NULL)
    {
        if(temp->next->pid > pid)
        {
            // printf("Adding to bglist: %d\n", pid);
            List successor = temp->next;
            temp->next = (struct LinkedList*)malloc(sizeof(struct LinkedList));
            temp->next->next = NULL;
            temp->next->p_name = p_name;
            temp->next->pid = pid;
            temp->next->next = successor;
            return;
        }
        else
        {
            temp = temp->next;
        }
    }

    temp->next = (struct LinkedList*)malloc(sizeof(struct LinkedList));
    temp->next->next = NULL;
    temp->next->p_name = p_name;
    temp->next->pid = pid;    
}

void RemoveNode(List L, int pid)
{
    //printf("in removenode\n");
    Node temp = L;
    int flag = 0;
    while(temp->next != NULL && temp->next->pid != pid)
    {
        //printf("%d\n", temp->next->pid);
        temp = temp->next;
    }
    if(temp->next != NULL)
    {
        flag = 1;
    }
    if(flag == 1)
    {
        Node tofree = temp->next;
        //printf("removed: %d\n", temp->next->pid);
        temp->next = temp->next->next;
        free(tofree);
    }
}
