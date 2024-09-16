CC := clang

OPT := -O3

BINARY := blackjack

# OS detection and setting of DEL command
NAME := $(BINARY)
NAME_DEBUG := $(BINARY)_debug
DEL := rm -rf bin/release/*.o; rm -rf bin/debug/*.o $(NAME) $(NAME_DEBUG)

# Check if bin directory and its release and debug subdirectories are present
$(shell mkdir -p bin/release)
$(shell mkdir -p bin/debug)

DEB = -g3 -O0

CFLAGS = -Wall -Wextra -Werror -std=gnu2x -finline-functions -march=native -Wno-unused-parameter
CFLAGS += $(OPT)

HDR = hdr

SRC = $(wildcard src/*.c)
SRC += $(wildcard src/Objects/*.c)

OBJ = $(patsubst src/%.c,bin/release/%.o,$(SRC))

DEBUG_OBJ = $(patsubst %.c,%_debug.o,$(subst src/,bin/debug/,$(SRC)))


all: debug

release: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

debug: $(DEBUG_OBJ)
	$(CC) $(DEB) $(DEBUG_OBJ) -o $(NAME_DEBUG)


bin/debug/%_debug.o: src/%.c
	$(CC) $(DEB) -I $(HDR) -c $< -o $@

bin/debug/Objects/%_debug.o: src/Objects/%.c
	$(CC) $(DEB) -I $(HDR) -c $< -o $@


bin/release/%.o: src/%.c
	$(CC) $(CFLAGS) -I $(HDR) -c $< -o $@

bin/release/Objects/%.o: src/Objects/%.c
	$(CC) $(CFLAGS) -I $(HDR) -c $< -o $@

assembly: $(DEBUG_OBJ)
	$(shell mkdir -p bin/assembly)
	$(foreach file,$(DEBUG_OBJ), \
		objdump -d -S $(file) > $(patsubst bin/debug/%.o,bin/assembly/%.s, $(file)) \
	)


clean:
	$(DEL)
	rm -rf bin/assembly/*.s

rebuild:
	$(DEL)
	make all

# uses bear to build a compile_commands.json
# for clangd
paths:
	bear -- make release
