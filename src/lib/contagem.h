#ifndef CONTAGEM_H
#define CONTAGEM_H

int contar_objetos_sequencial(int *matriz, int linhas, int colunas);
int contar_objetos_paralelo(int *matriz, int linhas, int colunas, int num_threads);

#endif