FLAGS		= -g -Wno-unused-value
CC          = clang
EXEC		= diceRoll
LIBS 		= -lcurses
SRC			= src
BUILD       = build

all: main.o dice.o tags 
	$(CC) $(FLAGS) $(BUILD)/main.o $(BUILD)/dice.o -o $(BUILD)/$(EXEC) $(LIBS)

main.o: $(SRC)/main.c $(SRC)/main.h
	mkdir -p $(BUILD)
	$(CC) $(FLAGS) -c $(SRC)/main.c -o $(BUILD)/main.o

dice.o: $(SRC)/dice.c $(SRC)/dice.h
	mkdir -p $(BUILD)
	$(CC) $(FLAGS) -c $(SRC)/dice.c -o $(BUILD)/dice.o

clean: 
	rm -rf build
	rm -rf doxy
	rm -f tags

tags:
	ctags -R --c++-kinds=+p --fields=+iaS --extra=+q .

docs:
	doxygen

run: all
	./$(BUILD)/$(EXEC) 
