#include "flood-fill.h"

void flood_fill(int *matriz, int *visitado, int linhas, int colunas, int linha, int coluna, int *pilha) {
    int dl[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int dc[8] = {-1, 0, 1, -1, 1, -1, 0, 1};

    int topo;
    int indice;
    int novo_indice;
    int linha_atual;
    int coluna_atual;
    int nova_linha;
    int nova_coluna;
    int k;

    indice = linha * colunas + coluna;

    visitado[indice] = 1;
    pilha[0] = indice;
    topo = 1;

    while (topo > 0) {
        topo--;

        indice = pilha[topo];

        linha_atual = indice / colunas;
        coluna_atual = indice % colunas;

        for (k = 0; k < 8; k++) {
            nova_linha = linha_atual + dl[k];
            nova_coluna = coluna_atual + dc[k];

            if (nova_linha >= 0 && nova_linha < linhas && nova_coluna >= 0 && nova_coluna < colunas) {
                novo_indice = nova_linha * colunas + nova_coluna;

                if (matriz[novo_indice] == 1 && visitado[novo_indice] == 0) {
                    visitado[novo_indice] = 1;
                    pilha[topo] = novo_indice;
                    topo++;
                }
            }
        }
    }
}

void flood_fill_regiao(int *matriz, int *rotulos, int colunas, int linha_inicio, int linha_fim, int linha, int coluna, int rotulo, int *pilha) {
    int dl[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int dc[8] = {-1, 0, 1, -1, 1, -1, 0, 1};

    int topo;
    int indice;
    int novo_indice;
    int linha_atual;
    int coluna_atual;
    int nova_linha;
    int nova_coluna;
    int k;

    indice = linha * colunas + coluna;

    rotulos[indice] = rotulo;
    pilha[0] = indice;
    topo = 1;

    while (topo > 0) {
        topo--;

        indice = pilha[topo];

        linha_atual = indice / colunas;
        coluna_atual = indice % colunas;

        for (k = 0; k < 8; k++) {
            nova_linha = linha_atual + dl[k];
            nova_coluna = coluna_atual + dc[k];

            if (nova_linha >= linha_inicio && nova_linha <= linha_fim && nova_coluna >= 0 && nova_coluna < colunas) {
                novo_indice = nova_linha * colunas + nova_coluna;

                if (matriz[novo_indice] == 1 && rotulos[novo_indice] == 0) {
                    rotulos[novo_indice] = rotulo;
                    pilha[topo] = novo_indice;
                    topo++;
                }
            }
        }
    }
}