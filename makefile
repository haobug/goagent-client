CC = gcc
LIBS = 
TARGET = goagent-client
CFLAGS = -Wall -O3 
SOURCE_C = $(wildcard *.c)
OBJECT_O = $(SOURCE_C:.c=.o)

%.o: %.c
	$(CC) -c $(CFLAGS)  $< -o $@   

$(TARGET): $(OBJECT_O)
	 $(CC) ${CFLAGS} $(OBJECT_O) $(LIBS) -o $@ 

clean:
	rm -rf *.o goagent-client
