CC=gcc
CFLAGS=-c -Wall 
LDFLAGS= -std=c11
SOURCES=main.c btree.c
OBJECTS=$(SOURCES:.cpp=.o)
	EXECUTABLE=btree

all: $(SOURCES) $(EXECUTABLE)
		
$(EXECUTABLE): $(OBJECTS) 
		$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
		$(CC) $(CFLAGS) $< -o $@
