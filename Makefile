CFLAGS=-Wall -Wextra -std=c11 -pedantic
LIBS=

virtual_chunchine:
	$(CC) $(CFLAGS) -o main main.c $(LIBS)
