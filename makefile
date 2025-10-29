CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Iincs
SRC = src/main.c src/bio.c src/trie.c src/list.c src/commands.c src/file_utils.c
OBJ = $(SRC:.c=.o)
EXEC = bio

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(EXEC)

clean:
	rm -f $(OBJ) $(EXEC)

run: all
	./$(EXEC)
