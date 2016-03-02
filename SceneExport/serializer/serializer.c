#include <stdlib.h>
#include <stdio.h>
#include "serializer.h"

Ser_Node * InternalAddSerNode(Ser_Node ** serNode, void * data, size_t size);
static int cc=0;
Ser_Node * AddSerNode(Ser_Base * serBase, void * data, size_t size)
{
	Ser_Node ** node= &(serBase->start);
	while (*node)
	{
		if ((*node)->data == data)
			return *node;
		node = &((*node)->next);
	}
	serBase->end = InternalAddSerNode(node, data, size);
	serBase->size += size;
	cc++;
	return serBase->end;
}

Ser_Node * InternalAddSerNode(Ser_Node ** serNode, void * data, size_t size)
{
	Ser_Node **p;
	p = (*serNode == NULL) ? serNode:&((*serNode)->next);
	*p = malloc(sizeof(Ser_Node));
	(*p)->data=data;
	(*p)->size=size;
	(*p)->next=NULL;
	(*p)->ptrs=NULL;
	return *p;
}

void CollectPtr(Ser_Node * serNode, void ** ptr)
{
	Ser_PtrNode ** ptrNode = &(serNode->ptrs);
	while (*ptrNode)
	{
		if ((*ptrNode)->ptr == ptr)	// Maybe unneeded  
			return;
		ptrNode = &((*ptrNode)->next);
	}
	*ptrNode = malloc(sizeof(Ser_PtrNode));
	(*ptrNode)->ptr=ptr;
	(*ptrNode)->real_ptr=*ptr; //During pre-collecting of pointers it is unusable
	(*ptrNode)->reloc_ptr=0;
	(*ptrNode)->next = NULL;
} 

size_t RelocatePtrs(Ser_Base * serBase)
{
	Ser_Node * node=serBase->start, * node2;
	Ser_PtrNode * ptrNode;
	size_t offset;
	serBase->offset = 0;
	while (node)
	{
		ptrNode = node->ptrs;
		while (ptrNode)
		{
			// ???
			if (!ptrNode->ptr || !(*ptrNode->ptr))	// NULL pointer handling
			{
				ptrNode->reloc_ptr = serBase->size; // if reloc_ptr == size --> NULL pointer
				ptrNode = ptrNode->next;
				continue;
			}
			node2 = serBase->start;
			offset = 0;
			while (node2)
			{				
				if (node2->data == *(ptrNode->ptr))
					break;
				offset += node2->size;
				node2 = node2->next;
			}
			ptrNode->real_ptr = *(ptrNode->ptr);
			ptrNode->reloc_ptr = offset;	// if offset == size --> NULL pointer
			ptrNode = ptrNode->next;
		}
		serBase->offset += node->size;
		node = node->next;	
	}
	return serBase->size;
}

void Serialize(Ser_Base *serBase,  HANDLE hFile)
{
	Ser_Node * node=serBase->start;
	Ser_PtrNode * ptrNode;
	DWORD written;
	while (node)
	{
		ptrNode = node->ptrs;
		while (ptrNode)
		{
			ptrNode->real_ptr = *(ptrNode->ptr);	// Used when pre-collecting the pointers
			*(ptrNode->ptr) = (void*)ptrNode->reloc_ptr;			
			ptrNode = ptrNode->next;
		}
		WriteFile(hFile, node->data,node->size,&written, NULL);
		node=node->next;
	}
	node=serBase->start;
	while (node)
	{
		ptrNode = node->ptrs;
		while (ptrNode)
		{
			*(ptrNode->ptr) = ptrNode->real_ptr;
			ptrNode = ptrNode->next;
		}
		node=node->next;
	}
}

void ReleaseNodePtrs(Ser_PtrNode * ptrNode)
{
	while(ptrNode)
	{
		ReleaseNodePtrs(ptrNode->next);
		free(ptrNode);
		ptrNode = NULL;
	}
}
void ReleaseNodes(Ser_Node * serNode)
{
	while(serNode)
	{
		ReleaseNodes(serNode->next);
		ReleaseNodePtrs(serNode->ptrs);
		free(serNode);
		serNode = NULL;
	}
}
void Release(Ser_Base * serBase)
{
	ReleaseNodes(serBase->start);
}

void InitSerBase(struct st_Ser_Base * serBase)
{
	serBase->start = serBase->end = NULL;
	serBase->offset = serBase->size = 0;
	serBase->AddSerNode = AddSerNode;
	serBase->CollectPtr = CollectPtr;
	serBase->RelocatePtrs = RelocatePtrs;
	serBase->Serialize = Serialize;
	serBase->Release = Release;
}
