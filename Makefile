CC=gcc
CFLAGS= -lSDL2 -lSDL2main -lSDL2_image -lm
SOURCE=*.c
EXECUTABLE=game

default:
	$(CC) $(SOURCE) -o $(EXECUTABLE) $(CFLAGS) -Wall -Wextra -std=c99

raw:
	$(CC) $(SOURCE) -o $(EXECUTABLE)

run:
	$(CC) $(SOURCE) -o $(EXECUTABLE) $(CFLAGS) -Wall -Wextra -std=c99
	./game

rr:
	$(CC) $(SOURCE) -o $(EXECUTABLE)
	./game