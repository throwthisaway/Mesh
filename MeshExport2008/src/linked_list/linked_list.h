#pragma once

typedef struct st_Node
{
	void * data;
	struct st_Node * next;
}Node;
typedef struct st_LinkedList
{
	Node * start;
	Node * end;
	void (*PushBack)(struct st_LinkedList *, Node *);
	void (*Add)(struct st_LinkedList *, Node * /* insert after */,Node * /* node to insert */);
	void (*Clear)(struct st_LinkedList *);
	//Node *PopBack(void);
}LinkedList;

void InitLinkedList(LinkedList *);

