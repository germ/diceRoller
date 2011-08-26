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
	rm -rf build doxy tags $(EXEC)

tags:
	ctags $(SRC)/main.c $(SRC)/dice.c

docs:
	doxygen

run: all
	./$(BUILD)/$(EXEC) 
