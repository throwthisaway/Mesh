#ifndef SERIALIZER_H_
#define SERIALIZER_H_
#include <windows.h>

typedef struct st_Ser_PtrNode
{
	void ** ptr;
	void * real_ptr;
	int reloc_ptr;
	struct st_Ser_PtrNode * next;
}Ser_PtrNode;

typedef struct st_Ser_Node
{
	void * data;
	size_t size;
	Ser_PtrNode *ptrs; 	
	struct st_Ser_Node * next;
}Ser_Node;

typedef struct st_Ser_Base
{
	Ser_Node * start, * end;
	size_t offset, size;
	Ser_Node * (*AddSerNode)(struct st_Ser_Base * serBase, void * data, size_t size);
	void (*CollectPtr)(Ser_Node * serNode, void ** ptr);
	long (*RelocatePtrs)(struct st_Ser_Base * serBase);
	void (*Serialize)(struct st_Ser_Base *serBase, HANDLE hFile);
	void (*Release)(struct st_Ser_Base * serBase);
}Ser_Base;
#ifdef __cplusplus
extern "C" void InitSerBase(Ser_Base * serBase);
#else
void InitSerBase(Ser_Base * serBase);
#endif
/*Ser_Node * AddSerNode(Ser_Base * serBase, void * data, size_t size);
void CollectPtr(Ser_Node * serNode, void ** ptr);
void RelocatePtrs(Ser_Base * serBase);*/
//void Serialize(Ser_Base *serBase, FILE /*should be open and properly positioned*/*f);

#endif /*SERIALIZER_H_*/
