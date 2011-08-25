FLAGS		= -g -Wno-unused-value
CC      = clang
EXEC		= diceRoll
LIBS 		= -lcurses

all: clean main.o dice.o tags
	$(CC) $(FLAGS) main.o dice.o -o $(EXEC) $(LIBS)

main.o: main.c main.h
	$(CC) $(FLAGS) -c main.c 

dice.o: dice.c dice.h
	$(CC) $(FLAGS) -c dice.c

clean: 
	rm -f *.o
	rm -f $(EXEC)

splint:
	splint *.c *.h

tags:
	ctags -R --c++-kinds=+p --fields=+iaS --extra=+q .
