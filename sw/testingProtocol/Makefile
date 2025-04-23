CC = gcc
CFLAGS = \
-Wall \
-g

SRC_FILES = \
main.c \
ip.c \
stubs.c \
Networking_Globs.c \
mac.c \

OBJ_FILES = $(patsubst %.c,build/%.o,$(filter %.c,$(SRC_FILES)))

TARGET = build/exe

testing: $(TARGET).elf

$(TARGET).elf: $(OBJ_FILES)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -o $@ $^

build/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf build
