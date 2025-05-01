
SRCS = $(shell find . -name '*.c')
OBJS = $(SRCS:.c=.o)

TARGET = main

.PHONY: all run clean

all: $(TARGET)

run:
	./$(TARGET)

clean:
	rm -f $(OBJS)

$(TARGET): $(OBJS)
	gcc -o $@ $^
%.o: %.c
	gcc -g -ggdb -c -o $@ $< -I./src

	 
