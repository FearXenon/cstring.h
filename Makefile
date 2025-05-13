
SRCS = $(shell find . -name '*.c')
OBJS = $(SRCS:.c=.o)

TARGET = test 

.PHONY: all run clean

all: $(TARGET)

run:
	./$(TARGET)

check-leaks:
	valgrind --track-origins=yes --leak-check=full -s ./$(TARGET)

clean:
	rm -f $(OBJS)

$(TARGET): $(OBJS)
	gcc -o $@ $^
%.o: %.c
	gcc -g -ggdb -c -o $@ $< -I./src

	 
