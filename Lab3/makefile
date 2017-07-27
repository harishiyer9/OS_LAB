#makefile

CC := gcc
CFLAGS := -Wall
SOURCES := squareroot.h sqrt.c SQRT.c
OUT_FILE := a.out

build: $(SOURCES)
	$(CC) $(CFLAGS) -o $(OUT_FILE) $(SOURCES) -lm

clean:
	rm -f $(OUT_FILE)

rebuild: clean build

all: build

