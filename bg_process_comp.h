#ifndef __BG_PROCESS_COMP_H
#define __BG_PROCESS_COMP_H

typedef struct LinkedList* Node;
typedef struct LinkedList* List;

struct LinkedList
{
    char* p_name;
    int pid;
    Node next;
};

void RemoveNode(List L, int pid);
void AddNode(List L, char* p_name, int pid);
List InitList();

#endif