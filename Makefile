CC = gcc
FLAGS = -Wall

restaurantManager: main.o Menu.o TyposStoixeiouMenu.o HtmlParser.o
	$(CC) $(FLAGS) -o restaurantManager main.o Menu.o TyposStoixeiouMenu.o HtmlParser.o

main.o: main.c
	$(CC) $(FLAGS) -c main.c

Menu.o: Menu.c
	$(CC) $(FLAGS) -c Menu.c

TyposStoixeiouMenu.o: TyposStoixeiouMenu.c
	$(CC) $(FLAGS) -c TyposStoixeiouMenu.c

HtmlParser.o: HtmlParser.c
	$(CC) $(FLAGS) -c HtmlParser.c

.PHONY: clean

clean:
	rm -f restaurantManager main.o Menu.o TyposStoixeiouMenu.o HtmlParser.o
