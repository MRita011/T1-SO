#include "flood-fill.h"

void flood_fill(int *matriz, int *visitado, int linhas, int colunas, int linha, int coluna) {
    int dl[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int dc[8] = {-1, 0, 1, -1, 1, -1, 0, 1};

    int nova_linha, nova_coluna;
    int k;
    int indice, novo_indice;

    indice = linha * colunas + coluna;

    visitado[indice] = 1;

    for (k = 0; k < 8; k++) {
        nova_linha = linha + dl[k];
        nova_coluna = coluna + dc[k];

        if (nova_linha >= 0 && nova_linha < linhas && nova_coluna >= 0 && nova_coluna < colunas) {
            novo_indice = nova_linha * colunas + nova_coluna;

            if (matriz[novo_indice] == 1 && visitado[novo_indice] == 0) {
                flood_fill(matriz, visitado, linhas, colunas, nova_linha, nova_coluna);
            }
        }
    }
}

void flood_fill_regiao(int *matriz, int *rotulos, int colunas, int linha_inicio, int linha_fim, int linha, int coluna, int rotulo) {
    int dl[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int dc[8] = {-1, 0, 1, -1, 1, -1, 0, 1};

    int nova_linha, nova_coluna;
    int k;
    int indice, novo_indice;

    indice = linha * colunas + coluna;

    rotulos[indice] = rotulo;

    for (k = 0; k < 8; k++) {
        nova_linha = linha + dl[k];
        nova_coluna = coluna + dc[k];

        if (nova_linha >= linha_inicio && nova_linha <= linha_fim && nova_coluna >= 0 && nova_coluna < colunas) {
            novo_indice = nova_linha * colunas + nova_coluna;
            
            if (matriz[novo_indice] == 1 && rotulos[novo_indice] == 0) {
                flood_fill_regiao(matriz, rotulos, colunas, linha_inicio, linha_fim, nova_linha, nova_coluna, rotulo);
            }
        }
    }
}