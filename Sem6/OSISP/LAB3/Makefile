CC = gcc
TARGET = invert_chars
OBJS = main.o invert.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS)

main.o: main.c invert.h
	$(CC) -c main.c

invert.o: invert.c invert.h
	$(CC) -c invert.c

clean:
	rm -f $(OBJS) $(TARGET)

run: $(TARGET)
	./$(TARGET) strings.txt
