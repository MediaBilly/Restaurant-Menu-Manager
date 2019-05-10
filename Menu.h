/*************************************************
File	: Menu.h
Author	: V. Kiriakopoulos - 27/4/2018
**************************************************/
#ifndef __MENU__
#define __MENU__
#include <stdio.h>
#include "TyposStoixeiouMenu.h"

typedef struct InfoMenu *InfoMenuPtr;

//All menu type functions in the same order with the task pdf
InfoMenuPtr MenuCreate(char *name);
int MenuEmptyCategory(InfoMenuPtr menu);
void MenuInsertNode(InfoMenuPtr menu,TStoixeiouMenu data);
void MenuPrintContent(InfoMenuPtr menu);
void MenuChangeContent(InfoMenuPtr menu);
int MenuNext(InfoMenuPtr menu);
int MenuPrev(InfoMenuPtr menu);
int MenuMoveNext(InfoMenuPtr menu);
int MenuMovePrev(InfoMenuPtr menu);
int MenuInsertCategory(InfoMenuPtr menu);
int MenuMoveUp(InfoMenuPtr menu);
int MenuMoveDown(InfoMenuPtr menu);
void MenuPrintCategory(InfoMenuPtr menu,int htmlOutput,FILE *outStream);
int MenuDelete(InfoMenuPtr menu);
void MenuDeleteDown(InfoMenuPtr menu);
void MenuCut(InfoMenuPtr menu);
void MenuPaste(InfoMenuPtr menu);
void MenuDestroy(InfoMenuPtr *menu);
void MenuMoveStart(InfoMenuPtr menu);//A function i added to set the Current to Start to simplify the print of the whole menu

#endif
