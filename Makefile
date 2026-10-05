CC = gcc
CFLAGS = -std=c89 -Wall -Wextra -pedantic -pthread

LIB = src/lib/contagem.c src/lib/flood-fill.c src/lib/union-find.c

SEQUENCIAL = src/conta-objetos-sequencial.c
PARALELO = src/conta-objetos-paralelo.c
DESEMPENHO = tests/teste-desempenho.c

.PHONY: all clean

all: sequencial paralelo desempenho

sequencial:
	$(CC) $(CFLAGS) $(SEQUENCIAL) $(LIB) -o sequencial

paralelo:
	$(CC) $(CFLAGS) $(PARALELO) $(LIB) -o paralelo

desempenho:
	$(CC) $(CFLAGS) $(DESEMPENHO) $(LIB) -o desempenho

clean:
	rm -f sequencial paralelo desempenho