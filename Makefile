CC = gcc
CFLAGS = -Wall -Wextra -std=c11
SRC = src/main.c
OUT = build/emb3d

all: $(OUT)

$(OUT): $(SRC)
	@mkdir -p build
	$(CC) $(CFLAGS) -o $(OUT) $(SRC)

run: $(OUT)
	./$(OUT)

clean:
	rm -rf build

.PHONY: all run clean
