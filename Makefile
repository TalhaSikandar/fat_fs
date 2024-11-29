CC = gcc
CFLAGS = -Wall -g
SRC = src/min_heap.c src/fat_fs.c src/disk.c src/fat_table.c src/directory.c src/utils.c src/main.c
OBJ = $(SRC:.c=.o)
EXEC = main

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(EXEC)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXEC)
