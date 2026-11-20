CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -pedantic -O2
TARGET = ambulance_routing

all: $(TARGET)

$(TARGET): main.c engine.c engine.h
	$(CC) $(CFLAGS) main.c engine.c -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all run clean
