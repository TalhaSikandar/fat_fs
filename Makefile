
CC = gcc
CFLAGS = -Wall -g
SRC = src/fat_fs.c src/disk.c src/fat_table.c src/directory.c src/utils.c
OBJ = $(SRC:.c=.o)
EXEC = fat_fs

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(EXEC)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXEC)
