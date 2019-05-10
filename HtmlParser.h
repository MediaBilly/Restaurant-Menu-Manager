/*************************************************
File	: HtmlParser.h
Author	: V. Kiriakopoulos - 27/4/2018
**************************************************/
#ifndef __HTMLPARSER__
#define __HTMLPARSER__
#include <stdio.h>

typedef struct field
{
	int category;//0 for plate,1 for category,-1 for end of file
	int hLevel;//The level in html h Tag
	char *content;//NULL if format error
	float price;//The price if plate
} field;

char *HtmlGetTitle(FILE *input);//Reads title from html file
field HtmlGetNextField(FILE *input);//Reads and returns the content from the next p or h tag 
int EndHtmlFile(FILE *input);//Checks if the closing tags are correct
void HtmlPrintTitle(FILE *output,char *title);//Prints the first opening tags with the title to the output html file
void EndHtmlOutput(FILE *output);//Prints the last closing tags with the title to the output html file
int HtmlEndOfMenu(field f);//Checks if another field(p or h tag) exists in input html file
int HtmlFormatError(field f);//Checks if there is error in html file format
//Getters
int htmlFieldIsCategory(field f);
int htmlFieldGethLevel(field f);
char *htmlFieldGetContent(field f);
float htmlFieldGetPrice(field f);

#endif