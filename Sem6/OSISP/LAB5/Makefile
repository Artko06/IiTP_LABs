CC = gcc
TARGET = mt_sort
SRC = mt_sort.c

all: $(TARGET)

$(TARGET): mt_sort.c
	$(CC) $(SRC) -o $(TARGET)

run1: $(TARGET)
	./$(TARGET) 10000000 1

run2: $(TARGET)
	./$(TARGET) 10000000 2

run4: $(TARGET)
	./$(TARGET) 10000000 4

run8: $(TARGET)
	./$(TARGET) 10000000 8

run16: $(TARGET)
	./$(TARGET) 10000000 16

clean:
	rm -f $(TARGET)
