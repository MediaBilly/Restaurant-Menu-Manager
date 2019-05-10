/*************************************************
File    : TyposStoixeiouMenu.c
Author  : V. Kiriakopoulos - 27/4/2018
**************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TyposStoixeiouMenu.h"
#include "HtmlParser.h"

void TSInitialize(TStoixeiouMenu *V)
{
  V->Price = -1;
}

void TSInitializeHead(TStoixeiouMenu *V)
{
  V->name = malloc(7);
  strcpy(V->name,"kefali");
  V->Price = -1;
  V->Category = 1;
}

void TSSetValue(TStoixeiouMenu *Lhs, TStoixeiouMenu Rhs)
{
  *Lhs = Rhs;
}

int TSIsHead(TStoixeiouMenu V)
{
  return !strcmp(V.name,"kefali");
}

int TSPrintValue(FILE *out,TStoixeiouMenu V,int htmlPrint,unsigned int level)
{
  unsigned int i;
  if(out == NULL)
    return 0;
  if(strcmp(V.name,"kefali"))
  {
    for(i = 0;i < level;i++)
        fprintf(out,"    ");
    if(htmlPrint)
    {
      if(V.Category)
        fprintf(out, "<h%d>%s</h%d>\n",level+1,V.name,level+1);
      else
        fprintf(out, "<p>%s %.2f</p>\n",V.name,V.Price);
    }
    else
    {
      fprintf(out,V.Category ? "" : "%c ",42);
      fprintf(out,"%s ",V.name);
      if(!V.Category)
        fprintf(out,"%.2f\n",V.Price);
      else
        fprintf(out, "\n");
    }
  }
  return 1;
}

int TSReadValue(FILE *in,TStoixeiouMenu *V,int htmlRead)
{
  char buf[30];
  field htmlField;
  if(in == NULL)
    return -1;
  if(htmlRead)
  {
    htmlField = HtmlGetNextField(in);
    if(HtmlEndOfMenu(htmlField)) return -1;//End of HTML menu
    if(HtmlFormatError(htmlField)) return -2;//Format error in HTML file
    V->name = htmlFieldGetContent(htmlField);
    V->Category = htmlFieldIsCategory(htmlField);
    V->Price = htmlFieldGetPrice(htmlField);
    return htmlFieldGethLevel(htmlField);
  }
  else
  {
    if(V->Price == -1)
    {
      printf("Is this Category or Plate?(type 0 for Plate and 1 for Category)\n");
      scanf("%d",&V->Category);
    }
    if(V->Category)
    {
      printf("Category:");
      fscanf(in,"%s",buf);
      V->name = malloc(strlen(buf) + 1);
      strcpy(V->name,buf);
      V->Price = 0;
    }
    else
    {
      printf("Name:");
      fscanf(in,"%s",buf);
      V->name = malloc(strlen(buf) + 1);
      strcpy(V->name,buf);
      printf("Price:");
      fscanf(in,"%f",&(V->Price));
    }
    return 0;//No HTMl read so return 0
  }
}

void TSDestroy(TStoixeiouMenu V)
{
  free(V.name);
}
