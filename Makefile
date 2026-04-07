CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Iinclude
SRC = src/main.c src/matrix.c
OUT = build/emb3d

all: $(OUT)

$(OUT): $(SRC) include/matrix.h
	@mkdir -p build
	$(CC) $(CFLAGS) -o $(OUT) $(SRC)

run: $(OUT)
	./$(OUT)

clean:
	rm -rf build

.PHONY: all run clean
