#ifndef FLOOD_FILL_H
#define FLOOD_FILL_H

void flood_fill(
    int *matriz, 
    int *visitado, 
    int linhas, 
    int colunas, 
    int linha, 
    int coluna, 
    int *pilha
);

void flood_fill_regiao(
    int *matriz,
    int *rotulos, 
    int colunas, 
    int linha_inicio, 
    int linha_fim, 
    int linha, 
    int coluna, 
    int rotulo, 
    int *pilha
);

#endif