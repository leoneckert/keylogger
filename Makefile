CC=gcc
CFLAGS=-framework ApplicationServices -framework Carbon
SOURCES=keylogger4research.c
EXECUTABLE=keylogger4research

all: $(SOURCES)
	$(CC) $(SOURCES) $(CFLAGS) -o $(EXECUTABLE)


