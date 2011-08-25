CFLAGS		= -g -Wno-unused-value
CC          = clang
EXEC		= diceRoll
LIBS 		= -lcurses
SRC			= src
BUILD       = build

.PHONY: all clean docs

all: $(EXEC) tags

$(EXEC): $(BUILD)/main.o $(BUILD)/dice.o
	$(CC) $(CFLAGS) $(LIBS) -o $@ $^

# Implicit rules for building object files:
$(BUILD)/%.o: $(SRC)/%.c $(SRC)/%.h
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -c -o $@ $<

clean: 
	rm -rf build
	rm -rf doxy
	rm -f tags

tags:
	# This may be a little much for a C program; consider
	# POSIX ctags whose rule would be:
	# ctags $(SRC)/main.c $(SRC)/dice.c
	ctags -R --c++-kinds=+p --fields=+iaS --extra=+q .

docs:
	doxygen

run: all
	./$(BUILD)/$(EXEC) 
