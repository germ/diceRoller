FLAGS		= -g 
CC			=	gcc
EXEC		= diceRoll
LIBS 		= -lcurses
FLAGS 	= -Wall

all: main.o dice.o tags
	gcc -g main.o dice.o -o $(EXEC) $(LIBS)

main.o: main.c main.h
	gcc $(FLAGS) -c main.c 

dice.o: dice.c dice.h
	gcc $(FLAGS) -c dice.c

clean: 
	rm -f *.o
	rm $(EXEC)

splint:
	splint *.c *.h

tags:
	ctags -R --c++-kinds=+p --fields=+iaS --extra=+q .
