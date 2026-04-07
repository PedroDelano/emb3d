CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Iinclude
SRC = src/main.c src/matrix.c src/linalg.c src/utils.c src/tokenizer.c
OUT = build/emb3d

all: $(OUT)

$(OUT): $(SRC) include/matrix.h include/linalg.h include/utils.h include/tokenizer.h
	@mkdir -p build
	$(CC) $(CFLAGS) -o $(OUT) $(SRC)

run: $(OUT)
	./$(OUT)

format:
	clang-format -i $(SRC) include/*.h

clean:
	rm -rf build

.PHONY: all run format clean
