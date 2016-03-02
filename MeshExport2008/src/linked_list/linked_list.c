#include "linked_list.h"
#include "stdlib.h"

void PushBack(struct st_LinkedList * list, Node * node)
{
	if (!list->start) list->start = node;
	node->next = NULL;
	list->end->next = node;
	list->end = node;
}
void Add(struct st_LinkedList * list, Node * /* insert after */ node1,Node * /* node to insert */ node2)
{
    Node * temp = list->start;
    while ((temp != NULL) && (temp != node1)) temp = temp->next;
    if (temp)
    {
		node2->next = temp->next;
		if (!temp->next) list->end = node2;
		temp->next = node2->next;
    }
}

void Clear(struct st_LinkedList * list)
{
    Node * temp=list->start, * next;
    while (temp != NULL) 
    {
        next = temp->next;
        free(temp);
        temp = next;
    }
}

void InitLinkedList(LinkedList * list)
{
	list->Add = Add;
	list->PushBack = PushBack;
	list->Clear = Clear;
}