/*************************************************
File	: HtmlParser.c
Author	: V. Kiriakopoulos - 27/4/2018
**************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "HtmlParser.h"

typedef struct tagStackNode *tagStackNodePtr;

tagStackNodePtr tagStack = NULL;

struct tagStackNode
{
	char *content;
	tagStackNodePtr next;
};

void tagStackPush(tagStackNodePtr *stack,char *content)
{
	tagStackNodePtr tmp = malloc(sizeof(struct tagStackNode));
	tmp->content = malloc(strlen(content) + 1);
	strcpy(tmp->content,content);
	tmp->next = *stack;
	*stack = tmp;
}

void tagStackPop(tagStackNodePtr *stack)
{
	tagStackNodePtr toDel = *stack;
	*stack = (*stack)->next;
	free(toDel->content);
	free(toDel);
}

void tagStackDestroy(tagStackNodePtr *stack)
{
	tagStackNodePtr toDel;
	while(*stack != NULL)
	{
		toDel = *stack;
		*stack = (*stack)->next;
		free(toDel->content);
		free(toDel);
	}
	*stack = NULL;
}

char *HtmlGetTitle(FILE *input)
{
	int i = 0,htmlTag = 0,headTag = 0,found = 0;
	char ch,buf[100],*title;
	while((ch = fgetc(input)) < 33 || ch == 127) if(ch==EOF) return NULL;//Ignore whitespace characters
	if(ch != '<') return NULL;
	while(1)
	{
		if((ch = fgetc(input)) == '!')//Comment field
		{
			while((ch = fgetc(input)) != '>') if(ch==EOF) return NULL;
			while((ch = fgetc(input)) != '<') if(ch==EOF) return NULL;
		}
		else
		{
			if(ch == '/')//Closing tags
			{
				while((ch = fgetc(input)) >= 'a' && ch <= 'z')
					buf[i++] = ch;
				buf[i++] = '\0';
				while(ch != '>') 
				{
					if(ch==EOF) 
						return NULL;
					ch = fgetc(input);
				}
				if(tagStack == NULL || strcmp(tagStack->content,buf))
					return NULL;
				if(!strcmp(tagStack->content,"head"))//Close head tag
				{
					tagStackPop(&tagStack);
					if(headTag)
					{
						if(found)
							return title;//if head tag was opened before return the title
						else
						{
							printf("Title not found\n");
							free(title);
							return NULL;//Otherwise return NULL for format error
						}
					}
					else
					{
						free(title);
						return NULL;
					}
				}
				else//Close other tag and go to the next one
				{
					while((ch = fgetc(input)) != '<');
					tagStackPop(&tagStack);
				}
			}
			else//Opning tags
			{
				while(ch >= 'a' && ch <= 'z')
				{
					buf[i++] = ch;
					ch = fgetc(input);
				}
				buf[i++] = '\0';
				while(ch != '>') 
				{
					if(ch==EOF) 
						return NULL;
					ch = fgetc(input);
				}
				if(!strcmp(buf,"html"))//Open html tag
				{
					if(!htmlTag)
					{
						htmlTag = 1;
						while((ch = fgetc(input)) != '<') if(ch==EOF) return NULL;
					}
					else
						return NULL;
				}
				else if(!strcmp(buf,"head"))//Open head tag
				{
					if(htmlTag)
					{
						headTag = 1;
						tagStackPush(&tagStack,buf);
						while((ch = fgetc(input)) != '<') if(ch==EOF) return NULL;
					}
					else
						return NULL;
				}
				else if(!strcmp(buf,"title"))//Open title tag read title and close title tag
				{
					i = 0;
					while((ch = fgetc(input)) != '<')
					{
						if(ch == EOF)
							return NULL;
						if(ch != ' ')
							buf[i++] = ch;
					}
					buf[i++] = '\0';
					if(fgetc(input) == '/' && fgetc(input) == 't' && fgetc(input) == 'i' && fgetc(input) == 't' && fgetc(input) == 'l' && fgetc(input) == 'e')
					{
						while((ch = fgetc(input)) != '>')
								if(ch == EOF)
									return NULL;
						title = malloc(i);
						strcpy(title,buf);
						found = 1;
						while((ch = fgetc(input)) != '<') 
						{
							if(ch==EOF) 
							{
								free(title);
								return NULL;
							}
						}
					}
					else
						return NULL;
				}
				else
				{
					while((ch = fgetc(input)) != '<') if(ch==EOF) return NULL;
					tagStackPush(&tagStack,buf);
				}
			}
		}
		i = 0;
	}
}

field HtmlGetNextField(FILE *input)
{
	int i = 0;
	static int bodyTag = 0;
	char ch,buf[100];
	field ret = {0,0,NULL,0.0};
	while((ch = fgetc(input)) != '<') if(ch==EOF) return ret;
	while(1)
	{
		if((ch = fgetc(input)) == '!')//Comment field
		{
			while((ch = fgetc(input)) != '>') if(ch==EOF) return ret;
			while((ch = fgetc(input)) != '<') if(ch==EOF) return ret;
		}
		else
		{
			if(ch != '/')//Read opening tags
			{
				if(ch == 'h')//Read potential h tag(category)
				{
					if(!bodyTag) return ret;
					ch = fgetc(input);
					if(ch >= '0' && ch <= '6')
					{
						ret.hLevel = ch - '0';
						while((ch = fgetc(input)) != '>');
						while((ch = fgetc(input)) != '<')
						{
							if(ch == EOF)
								return ret;
							if(ch != ' ')
								buf[i++] = ch;
						}
						buf[i++] = '\0';
						if(fgetc(input) == '/' && fgetc(input) == 'h' && fgetc(input) == ret.hLevel + '0')
						{
							while((ch = fgetc(input)) != '>')
								if(ch == EOF)
									return ret;
							ret.content = malloc(i);
							strcpy(ret.content,buf);
							ret.category = 1;
						}
						return ret;//If not h tag we have error
					}
					else
						return ret;
				}
				else if(ch == 'p')//Read potential p tag(plate)
				{
					if(!bodyTag) return ret;
					while((ch = fgetc(input)) != '>');
					fscanf(input,"%s %f",buf,&ret.price);
					if(fgetc(input) == '<' && fgetc(input) == '/' && fgetc(input) == 'p')
					{
						while((ch = fgetc(input)) != '>')
								if(ch == EOF)
									return ret;
						ret.content = malloc(strlen(buf)+1);
						strcpy(ret.content,buf);
						ret.category = 0;
					}
					return ret;
				}
				else//If other tag was read push it in the stack to close it later
				{
					while(ch >= 'a' && ch <= 'z')
					{
						buf[i++] = ch;
						ch = fgetc(input);
					}
					buf[i++] = '\0';
					while(ch != '>') 
					{
						if(ch==EOF) 
							return ret;
						ch = fgetc(input);
					}
					while((ch = fgetc(input)) != '<') if(ch==EOF) return ret;
					if(!strcmp(buf,"body")) bodyTag = 1;//Body tag was read
					tagStackPush(&tagStack,buf);
				}
			}
			else//Read closing tags
			{
				while((ch = fgetc(input)) >= 'a' && ch <= 'z')
					buf[i++] = ch;
				buf[i++] = '\0';
				while(ch != '>') 
				{
					if(ch==EOF) 
						return ret;
					ch = fgetc(input);
				}
				//If stack has 0 tags pending to close or the last opened tag(which is at the top of the stack) 
				//is different fronm the tag we are trying to close,there is error
				if(tagStack == NULL || strcmp(tagStack->content,buf))
					return ret;
				if(!strcmp(tagStack->content,"body"))//End of menu
				{
					tagStackPop(&tagStack);
					ret.category = -1;
					return ret;
				}
				else
				{
					while((ch = fgetc(input)) != '<');
					tagStackPop(&tagStack);
				}//Pop (and close) latest tag and go to the next one
			}
			i = 0;
		}
	}
}

int EndHtmlFile(FILE *input)
{
	char ch;
	int success = 1;
	while(success && (ch = fgetc(input)) != '<') if(ch==EOF) success = 0;
	if(success && ((ch = fgetc(input)) != '/' || (ch = fgetc(input)) != 'h' || (ch = fgetc(input)) != 't' || (ch = fgetc(input)) != 'm' || (ch = fgetc(input)) != 'l'))
		while(success && (ch = fgetc(input)) != '>')
			if(ch == EOF)
				success = 0;
	tagStackDestroy(&tagStack);
	return success;
}

void HtmlPrintTitle(FILE *output,char *title)
{
	fprintf(output,"<html>\n<head>\n   <title>%s</title>\n</head>\n<body>\n",title);
}

void EndHtmlOutput(FILE *output)
{
	fprintf(output,"</body>\n</html>");
}

int HtmlEndOfMenu(field f)
{
	return f.category == -1;
}

int HtmlFormatError(field f)
{
	return f.content == NULL;
}

int htmlFieldIsCategory(field f)
{
	return f.category;
}

int htmlFieldGethLevel(field f)
{
	return f.hLevel;
}

char *htmlFieldGetContent(field f)
{
	return f.content;
}

float htmlFieldGetPrice(field f)
{
	return f.price;
}