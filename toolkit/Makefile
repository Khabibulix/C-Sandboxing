CC = gcc

CFLAGS = -Wall -Wextra -std=c11 -Iinclude -O0 -pedantic -fno-builtin -fsanitize=address -Wextra
LDFLAGS += -fsanitize=address

TARGET = toolkit

SRC = src/main.c src/snapshot.c src/memtrace.c src/scenarios.c src/stacktrace.c src/write_hex.c
OBJ = $(SRC:.c=.o)

all: $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJ)
	$(CC) $(OBJ) $(LDFLAGS) -o $(TARGET)

clean:
	rm -f $(OBJ) $(TARGET)

