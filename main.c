/*************************************************
File	: main.c
Author	: V. Kiriakopoulos - 27/4/2018
**************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Menu.h"
#include "TyposStoixeiouMenu.h"
#include "HtmlParser.h"

int main(int argc, char *argv[]) {
	int func,running = 1,res,food,empty,level = 1,tmpLevel,error;
	InfoMenuPtr 	FoodMenu=NULL,
					CafeMenu=NULL;
	char *foodName,*cafeName,buf[100];
	TStoixeiouMenu tmpData;
	FILE *htmlInput,*htmlOutput;
	while (running)
	{
		error = 0;
		printf("---Restaurant Menu Manager---\n");
		printf("Select Restaurant:\n");
		if(FoodMenu != NULL)
			printf("1)FoodRestaurant(%s)\n",foodName);
		else
			printf("1)FoodRestaurant\n");
		if(CafeMenu != NULL)
			printf("2)Cafeteria(%s)\n",cafeName);
		else
			printf("2)Cafeteria\n");
		//Choose menu
		scanf("%d",&res);
		switch(res)
		{
			case 1:
				food = 1;
				break;
			case 2:
				food = 0;
				break;
			default:
				printf("Wrong Option\n");
				food = -1;
				break;
		}
		if(food == -1)
			continue;
		//Choose and execute menu function
		printf("Functions:\n");
		printf("0)Exit\n");
		printf("1)MenuCreate\n");
		printf("2)MenuEmptyCategory\n");
		printf("3)MenuInsertNode\n");
		printf("4)MenuPrintContent\n");
		printf("5)MenuChangeContent\n");
		printf("6)MenuNext\n");
		printf("7)MenuPrev\n");
		printf("8)MenuMoveNext\n");
		printf("9)MenuMovePrev\n");
		printf("10)MenuInsertCategory\n");
		printf("11)MenuMoveUp\n");
		printf("12)MenuMoveDown\n");
		printf("13)MenuPrintCategory\n");
		printf("14)MenuDelete\n");
		printf("15)MenuDeleteDown\n");
		printf("16)MenuCut\n");
		printf("17)MenuPaste\n");
		printf("18)MenuDestroy\n");
		printf("19)ReadFromHTML\n");
		printf("20)OutputToHTML\n");
		printf("Give the number of the function you want to call\n");
		scanf("%d",&func);
		switch (func)
		{
			case 0:
				running = 0;
				break;
			case 1:
				if(food)
				{
					if(FoodMenu == NULL)
					{
						printf("Give the name of the FoodRestaurant:\n");
						scanf("%s",buf);
						foodName = malloc(strlen(buf) + 1);
						strcpy(foodName,buf);
						FoodMenu = MenuCreate(foodName);
					}
					else
						printf("Already initialized\n");
				}
				else
				{
					if(CafeMenu == NULL)
					{
						printf("Give the name of the Cafeteria:\n");
						scanf("%s",buf);
						cafeName = malloc(strlen(buf) + 1);
						strcpy(cafeName,buf);
						CafeMenu = MenuCreate(cafeName);
					}
					else
						printf("Already initialized\n");
				}
				break;
			case 2:
				empty = MenuEmptyCategory(food ? FoodMenu : CafeMenu);
				printf(empty == -1 ? "Not initialized\n" : (empty ? "Empty" : "Not Empty"));
				break;
			case 3:
				TSInitialize(&tmpData);
				TSReadValue(stdin,&tmpData,0);
				MenuInsertNode((food ? FoodMenu : CafeMenu),tmpData);
				break;
			case 4:
				MenuPrintContent(food ? FoodMenu : CafeMenu);
				break;
			case 5:
				MenuChangeContent(food ? FoodMenu : CafeMenu);
				break;
			case 6:
				MenuNext(food ? FoodMenu : CafeMenu);
				break;
			case 7:
				MenuPrev(food ? FoodMenu : CafeMenu);
				break;
			case 8:
				MenuMoveNext(food ? FoodMenu : CafeMenu);
				break;
			case 9:
				MenuMovePrev(food ? FoodMenu : CafeMenu);
				break;
			case 10:
				MenuInsertCategory(food ? FoodMenu : CafeMenu);
				break;
			case 11:
				MenuMoveUp(food ? FoodMenu : CafeMenu);
				break;
			case 12:
				MenuMoveDown(food ? FoodMenu : CafeMenu);
				break;
			case 13:
				MenuPrintCategory((food ? FoodMenu : CafeMenu),0,stdout);
				break;
			case 14:
				MenuDelete(food ? FoodMenu : CafeMenu);
				break;
			case 15:
				MenuDeleteDown(food ? FoodMenu : CafeMenu);
				break;
			case 16:
				MenuCut(food ? FoodMenu : CafeMenu);
				break;
			case 17:
				MenuPaste(food ? FoodMenu : CafeMenu);
				break;
			case 18:
				if(food)
				{
					MenuDestroy(&FoodMenu);
					free(foodName);
					foodName = NULL;
				}
				else
				{
					MenuDestroy(&CafeMenu);
					free(cafeName);
					cafeName = NULL;
				}
				break;
			case 19:
				if(food)
				{
					if(FoodMenu == NULL)
					{
						printf("Specify input file:\n");
						scanf("%s",buf);
						if((htmlInput = fopen(buf,"r")) != NULL)
						{
							if((foodName = HtmlGetTitle(htmlInput)) != NULL)
							{
								FoodMenu = MenuCreate(foodName);
								while((tmpLevel = TSReadValue(htmlInput,&tmpData,1)) != -1)
								{
									if(tmpLevel == -2)//Error
									{
										printf("Format Error\n");
										MenuDestroy(&FoodMenu);
										free(foodName);
										foodName = NULL;
										error = 1;
										break;
									}
									if(tmpLevel != 0)//Insert Category
									{
										while(level > tmpLevel)
										{
											MenuMoveUp(FoodMenu);
											level--;
										}
										MenuInsertNode(FoodMenu,tmpData);
										MenuInsertCategory(FoodMenu);
										MenuMoveDown(FoodMenu);
										level++;
									}
									else//Insert Plate
										MenuInsertNode(FoodMenu,tmpData);
								}
								level = 1;
								if(!error)
								{
									if(!EndHtmlFile(htmlInput))
									{
										printf("Format Error\n");
										MenuDestroy(&FoodMenu);
										free(foodName);
										foodName = NULL;
									}
									else
										MenuMoveStart(FoodMenu);
								}
								fclose(htmlInput);
							}
							else
								printf("Format Error\n");
						}
						else
							printf("Could not open input file\n");
					}
					else
						printf("Already initialized\n");
				}
				else
				{
					if(CafeMenu == NULL)
					{
						printf("Specify input file:\n");
						scanf("%s",buf);
						if((htmlInput = fopen(buf,"r")) != NULL)
						{
							if((cafeName = HtmlGetTitle(htmlInput)) != NULL)
							{
								CafeMenu = MenuCreate(cafeName);
								while((tmpLevel = TSReadValue(htmlInput,&tmpData,1)) != -1)//Read and insert all categories and plates from html file until it reaches the </body> tag
								{
									if(tmpLevel == -2)//Error
									{
										printf("Format Error\n");
										MenuDestroy(&CafeMenu);
										free(cafeName);
										cafeName = NULL;
										error = 1;
										break;
									}
									if(tmpLevel != 0)//Insert Category
									{
										while(level > tmpLevel)
										{
											MenuMoveUp(CafeMenu);
											level--;
										}
										MenuInsertNode(CafeMenu,tmpData);
										MenuInsertCategory(CafeMenu);
										MenuMoveDown(CafeMenu);
										level++;
									}
									else//Insert Plate
										MenuInsertNode(CafeMenu,tmpData);
								}
								level = 1;
								if(!error)
								{
									if(!EndHtmlFile(htmlInput))//Error in end file format
									{
										printf("Format Error\n");
										MenuDestroy(&CafeMenu);
										free(cafeName);
										cafeName = NULL;
									}
									else
										MenuMoveStart(CafeMenu);
								}
								fclose(htmlInput);
							}
							else
								printf("Format Error\n");
						}
						else
							printf("Could not open file\n");
					}
					else
						printf("Already initialized\n");
				}
				break;
			case 20:
				if((food ? FoodMenu : CafeMenu) != NULL)//Print menu to HTML file 
				{
					htmlOutput = fopen("output.html","w");
					MenuMoveStart(food ? FoodMenu : CafeMenu);
					HtmlPrintTitle(htmlOutput,(food ? foodName : cafeName));
					MenuPrintCategory((food ? FoodMenu : CafeMenu),1,htmlOutput);
					EndHtmlOutput(htmlOutput);
					fclose(htmlOutput);
				}
				else
					printf("Error:Not Initialized\n");
				break;
			default:
				printf("Wrong function.\n");
				break;
		}
		printf("\n");
	}
	return 0;
}
