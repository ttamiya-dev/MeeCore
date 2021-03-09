CC = gcc
CFLAGS = -c -Wall -g -Os -Isrc -DLINUX
LD = $(CC)

TARGET = MeeCore

OBJECTS = $(patsubst %.c, %.o, $(shell find . -name "*.c"))

all: $(TARGET)

$(TARGET): $(OBJECTS)
		$(LD) -o $@ $^

%.o: %.c
		$(CC) $(CFLAGS) $^ -o $@

clean:
		rm $(TARGET) $(OBJECTS)
