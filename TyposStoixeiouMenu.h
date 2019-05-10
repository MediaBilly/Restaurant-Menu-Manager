/*************************************************
File	: TyposStoixeiouMenu.h
Author	: V. Kiriakopoulos - 27/4/2018
**************************************************/
#ifndef __TSMENU__
#define __TSMENU__
#include <stdio.h>

typedef struct Komvos {
	char * name; 	// Name of Category or Plate
	int Category;	// 0 if Plate, 1 if Category
	float Price;	// Valid only if Plate(-1 if it is not yet initialized and has no content)
} TStoixeiouMenu;

void TSInitialize(TStoixeiouMenu *V);//Initialize for first read
void TSInitializeHead(TStoixeiouMenu *V);//Initialize as head node data
int TSIsHead(TStoixeiouMenu V);//Returns 1 if this data node belongs to head node and 0 if not
void TSDestroy(TStoixeiouMenu V);//Frees memory allocated for name(it is used in change content and destroy functions)
void TSSetValue(TStoixeiouMenu *Lhs, TStoixeiouMenu Rhs); // Lhs = Rhs
int TSPrintValue(FILE *out, TStoixeiouMenu V,int htmlPrint,unsigned int level);//Prints contents of data node
int TSReadValue(FILE *in, TStoixeiouMenu * V,int htmlRead);//Returns level of category if category and reading from html.Otherwise returns 0;In end of html menu it returns -1.If file format error occurs it returns -2
#endif
