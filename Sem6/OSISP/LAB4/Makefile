CC=gcc
TARGET = self_healing
SRC = self_healing.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET) self_healing.log

run: $(TARGET)
	./$(TARGET)