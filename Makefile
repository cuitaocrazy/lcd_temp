
CC = gcc

SOURCES = $(wildcard src/*.c)
OBJECT = $(patsubst src/%.c, dist/%.o, $(SOURCES))
LOADLIBES = -lwiringPi -lwiringPiDev


all: dist lcd_temp

dist:
	mkdir dist

lcd_temp: $(OBJECT)
	$(CC) $(LOADLIBES) $^ -o dist/$@

dist/%.o: src/%.c
	$(CC) $(CCFLAGS) -c $^ -o $@

clean:
	rm -rf dist
