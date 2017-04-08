EXEC=myshell.exe
SOURCES= myshell.c -lm
OBJECTS=$(SOURCES:.c=.o)
CC=gcc
CFLAGS=-W -Wall 
 
.PHONY: default clean
 
default: $(EXEC)
 
myshell.o: myshell.c

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)
 
$(EXEC): $(OBJECTS)
	$(CC) -o $@ $^
 
clean:
	rm -rf $(EXEC) $(OBJECTS) $(SOURCES:.c=.c~) $(SOURCES:.c=.h~) Makefile~