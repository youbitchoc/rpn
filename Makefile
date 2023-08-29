BIN = rpn
SRC = $(shell find . -name \*.c)
OBJ = ${SRC:.c=.o}

all: $(BIN)

.c.o:
	$(CC) -c $(CFLAGS) $< 

clean:
	rm -f $(BIN) $(OBJ)

$(BIN): $(OBJ)
	$(CC) -o $@ $(OBJ) $(LDFLAGS)

run:
	./$(BIN)

.PHONY: all clean run
