/*************************************************
File	: Menu.c
Author	: V. Kiriakopoulos - 27/4/2018
**************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Menu.h"

typedef struct MenuNode *MenuNodePtr;

struct InfoMenu {
	char* 	  Name;
	MenuNodePtr Start;
	MenuNodePtr Current;
	MenuNodePtr CutToPaste;
};

struct MenuNode {
	MenuNodePtr Prev, Next, Up, Down;
	TStoixeiouMenu Data;
};

InfoMenuPtr MenuCreate(char *name)
{
	InfoMenuPtr tmpMenu = malloc(sizeof(struct InfoMenu));
	tmpMenu->Name = malloc(strlen(name) + 1);
	strcpy(tmpMenu->Name,name);
	MenuNodePtr tmpNode = malloc(sizeof(struct MenuNode));
	tmpNode->Prev = tmpNode->Next = tmpNode->Up = tmpNode->Down = NULL;
	TSInitializeHead(&(tmpNode->Data));
	tmpMenu->Start = tmpMenu->Current = tmpNode;
	tmpMenu->CutToPaste = NULL;
	return tmpMenu;
}

int MenuEmptyCategory(InfoMenuPtr menu)
{
	if(menu == NULL) return -1;
	return TSIsHead(menu->Current->Data) && menu->Current->Next == NULL;
}

void MenuInsertNode(InfoMenuPtr menu,TStoixeiouMenu data)
{
	if(menu == NULL)
	{
		printf("Error:Not Initialized\n");
		return;
	}
	MenuNodePtr tmpNode = malloc(sizeof(struct MenuNode));
	tmpNode->Prev = menu->Current;
	tmpNode->Next = menu->Current->Next;
	tmpNode->Down = NULL;
	tmpNode->Up = menu->Current->Up;
	TSSetValue(&tmpNode->Data,data);
	if(menu->Current->Next != NULL)
		menu->Current->Next->Prev = tmpNode;
	menu->Current->Next = tmpNode;
	menu->Current = menu->Current->Next;
}

void MenuPrintContent(InfoMenuPtr menu)
{
	if(menu == NULL)
	{
		printf("Error:Not Initialized\n");
		return;
	}
	if(TSIsHead(menu->Current->Data))
		printf("Error:Head node\n");
	else
		TSPrintValue(stdout,menu->Current->Data,0,0);
}

void MenuChangeContent(InfoMenuPtr menu)
{
	if(menu == NULL)
	{
		printf("Error:Not Initialized\n");
		return;
	}
	if(TSIsHead(menu->Current->Data))
	{
		printf("Error:Head node\n");
		return;
	}
	TSDestroy(menu->Current->Data);
	TSReadValue(stdin,&(menu->Current->Data),0);
}

int MenuNext(InfoMenuPtr menu)
{
	if(menu == NULL)
	{
		printf("Error:Not Initialized\n");
		return 0;
	}
	if(menu->Current->Next == NULL)
		return 0;
	menu->Current = menu->Current->Next;
	return 1;
}

int MenuPrev(InfoMenuPtr menu)
{
	if(menu == NULL)
	{
		printf("Error:Not Initialized\n");
		return 0;
	}
	if(menu->Current->Prev == NULL)
		return 0;
	menu->Current = menu->Current->Prev;
	return 1;
}

int MenuMoveNext(InfoMenuPtr menu)
{
	if(menu == NULL)
	{
		printf("Error:Not Initialized\n");
		return 0;
	}
	if(TSIsHead(menu->Current->Data))
	{
		printf("Error:Head node\n");
		return 0;
	}
	if(menu->Current->Next == NULL)
	{
		printf("Error:No next node\n");
		return 0;
	}
	MenuNodePtr tmp,tmp2;
	menu->Current->Prev->Next = menu->Current->Next;
	menu->Current->Next->Prev = menu->Current->Prev;
	tmp = menu->Current->Next->Next;
	menu->Current->Next->Next = menu->Current;
	tmp2 = menu->Current->Next;
	menu->Current->Next = tmp;
	if(menu->Current->Next != NULL)
		menu->Current->Next->Prev = menu->Current;
	menu->Current->Prev = tmp2;
	return 1;
}

int MenuMovePrev(InfoMenuPtr menu)
{
	if(menu == NULL)
	{
		printf("Error:Not Initialized\n");
		return 0;
	}
	if(TSIsHead(menu->Current->Data))
	{
		printf("Error:Head node\n");
		return 0;
	}
	if(TSIsHead(menu->Current->Prev->Data))
	{
		printf("Error:Previous node is Head node\n");
		return 0;
	}
	menu->Current->Prev->Next = menu->Current->Next;
	if(menu->Current->Next != NULL)
		menu->Current->Next->Prev = menu->Current->Prev;
	menu->Current->Next = menu->Current->Prev;
	menu->Current->Prev = menu->Current->Prev->Prev;
	menu->Current->Prev->Next = menu->Current;
	menu->Current->Next->Prev = menu->Current;
	return 1;
}

int MenuInsertCategory(InfoMenuPtr menu)
{
	if(menu == NULL)
	{
		printf("Error:Not Initialized\n");
		return 0;
	}
	if(TSIsHead(menu->Current->Data))
	{
		printf("Error:Head node\n");
		return 0;
	}
	if(menu->Current->Data.Category == 0)
	{
		printf("Error:Plates Cannot have sub-category\n");
		return 0;
	}
	if(menu->Current->Down != NULL)
	{
		printf("Error:This node has already a sub-category\n");
		return 0;
	}
	MenuNodePtr tmpNode = malloc(sizeof(struct MenuNode));
	TSInitializeHead(&(tmpNode->Data));
	tmpNode->Up = menu->Current;
	tmpNode->Down = tmpNode->Prev = tmpNode->Next = NULL;
	menu->Current->Down = tmpNode;
	return 1;
}

int MenuMoveUp(InfoMenuPtr menu)
{
	if(menu == NULL)
	{
		printf("Error:Not Initialized\n");
		return 0;
	}
	if(menu->Current->Up == NULL)
	{
		printf("Error:No upper category\n");
		return 0;
	}
	menu->Current = menu->Current->Up;
	return 1;
}

int MenuMoveDown(InfoMenuPtr menu)
{
	if(menu == NULL)
	{
		printf("Error:Not Initialized\n");
		return 0;
	}
	if(menu->Current->Down == NULL)
	{
		printf("Error:No sub-category\n");
		return 0;
	}
	menu->Current = menu->Current->Down;
	return 1;
}

void print(MenuNodePtr node,unsigned int level,int htmlOutput,FILE *outStream)
{
	while(node != NULL)
	{
		TSPrintValue(outStream,node->Data,htmlOutput,level);
		print(node->Down,level + 1,htmlOutput,outStream);
		node = node->Next;
	}
}

void MenuPrintCategory(InfoMenuPtr menu,int htmlOutput,FILE *outStream)
{
	if(menu == NULL)
	{
		printf("Error:Not Initialized\n");
		return;
	}
	if(!TSIsHead(menu->Current->Data))
	{
		printf("Error:Current pointing to non head node\n");
		return;
	}
	print(menu->Current,0,htmlOutput,outStream);
}

int MenuDelete(InfoMenuPtr menu)
{
	if(menu == NULL)
	{
		printf("Error:Not Initialized\n");
		return 0;
	}
	if(menu->Current->Next == NULL)
	{
		printf("Error:The node after Current is NULL\n");
		return 0;
	}
	if(menu->Current->Next->Down != NULL)
	{
		printf("Error:The node that you are going to delete has sub-category\n");
		return 0;
	}
	MenuNodePtr toDel = menu->Current->Next;
	menu->Current->Next = menu->Current->Next->Next;
	if(menu->Current->Next != NULL)
		menu->Current->Next->Prev = menu->Current;
	free(toDel);
	return 1;
}

void Delete(MenuNodePtr node)
{
	MenuNodePtr toDel;
	while(node != NULL)
	{
		toDel = node;
		Delete(node->Down);
		node = node->Next;
		TSDestroy(toDel->Data);
		free(toDel);
	}
}

void MenuDeleteDown(InfoMenuPtr menu)
{
	if(menu == NULL)
	{
		printf("Error:Not Initialized\n");
		return;
	}
	Delete(menu->Current->Down);
	menu->Current->Down = NULL;
}

void MenuCut(InfoMenuPtr menu)
{
	if(menu == NULL)
	{
		printf("Error:Not Initialized\n");
		return;
	}
	if(TSIsHead(menu->Current->Data))
	{
		printf("Error:Cannot cut-paste head node\n");
		return;
	}
	if(menu->CutToPaste != NULL)
	{
		printf("Error:There is another node pending to paste\n");
		return;
	}
	menu->CutToPaste = menu->Current;
	menu->Current->Prev->Next = menu->Current->Next;
	if(menu->Current->Next != NULL)
		menu->Current->Next->Prev = menu->Current->Prev;
	menu->Current = menu->Current->Prev;
}

void MenuPaste(InfoMenuPtr menu)
{
	if(menu == NULL)
	{
		printf("Error:Not Initialized\n");
		return;
	}
	if(menu->CutToPaste == NULL)
	{
		printf("Error:Nothing to paste\n");
		return;
	}
	menu->CutToPaste->Next = menu->Current->Next;
	menu->CutToPaste->Prev = menu->Current;
	menu->Current->Next = menu->CutToPaste;
	if(menu->CutToPaste->Next != NULL)
		menu->CutToPaste->Next->Prev = menu->CutToPaste;
	menu->Current = menu->CutToPaste;
	menu->Current->Up = menu->Current->Prev->Up;
	menu->CutToPaste = NULL;
}

void MenuDestroy(InfoMenuPtr *menu)
{
	if(*menu == NULL)
	{
		printf("Error:Not Initialized\n");
		return;
	}
	Delete((*menu)->Start);
	free((*menu)->Name);
	free(*menu);
	*menu = NULL;
}

void MenuMoveStart(InfoMenuPtr menu)
{
	menu->Current = menu->Start;
}
