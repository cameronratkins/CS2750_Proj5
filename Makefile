CC = gcc
CFLAGS = -Wall

DEPS = mathwait.c
OBJ = mathwait.o

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

mathwait: $(OBJ)
	gcc $(CFLAGS) -o $@ $^

