EXECUTABLE=lightsoffsolver
SOURCES=src/boolarray.c src/boolmatrix.c src/progress.c src/boolgauss.c src/lightsoffsolver.c src/main.c
CFLAGS=-g -O0 -c -Wall -pedantic `pkg-config --cflags gdk-pixbuf-2.0`
LDFLAGS=`pkg-config --libs gdk-pixbuf-2.0`
CC=gcc
OBJECTS=$(SOURCES:.c=.o)
DOC_MODULE=$(EXECUTABLE)

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf src/*.o $(EXECUTABLE)
