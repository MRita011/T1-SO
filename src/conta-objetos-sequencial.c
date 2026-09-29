#include <stdio.h>
#include <stdlib.h>

int contar_objetos(int *matriz, int linhas, int colunas);

void flood_fill (int *matriz, int *visitado, int linhas, int colunas, int linha, int coluna);

int main (void) {
    int matriz1[] = {
        1, 1, 0, 0, 0,
        1, 1, 0, 0, 0,
        0, 0, 0, 1, 0,
        0, 0, 0, 1, 0,
        1, 0, 0, 0, 0
    };

    int matriz2[] = {
        0, 0, 0, 0, 0, 0, 1, 1,
        0, 1, 1, 1, 1, 0, 1, 0,
        0, 0, 1, 1, 0, 0, 0, 0,
        0, 0, 0, 1, 1, 0, 0, 0,
        0, 0, 0, 0, 1, 0, 0, 1,
        1, 1, 0, 0, 0, 0, 1, 1
    };

    int matriz3[] = {
        1, 1, 0, 0, 0, 0, 0, 0,
        1, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 1, 0,
        0, 0, 0, 1, 1, 0, 1, 0,
        0, 0, 0, 1, 1, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 1, 0, 0, 0, 0, 1,
        0, 0, 1, 0, 0, 0, 1, 1
    };

    int matriz4[] = {
        0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0,
        0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0,
        0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0,
        0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0,
        0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0,
        0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0,
        0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0,
        0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1
    };

    int matriz5[] = {
        1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 1,
        0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0,
        0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
        1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
        1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0,
        0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0,
        0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0,
        0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,
        0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1
    };

    printf("Teste 1 - esperado 3: %d\n", contar_objetos(matriz1, 5, 5));

    printf("Teste 2 - esperado 4: %d\n", contar_objetos(matriz2, 6, 8));

    printf("Teste 3 - esperado 5: %d\n", contar_objetos(matriz3, 8, 8));

    printf("Teste 4 - esperado 6: %d\n", contar_objetos(matriz4, 9, 12));

    printf("Teste 5 - esperado 7: %d\n", contar_objetos(matriz5, 12, 12));

    return 0;
}

void flood_fill(int *matriz, int *visitado, int linhas, int colunas, int linha, int coluna) {
    int dl[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int dc[8] = {-1,  0,  1, -1, 1, -1, 0, 1};

    int nova_linha, nova_coluna;
    int k,indice, novo_indice;

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

int contar_objetos(int *matriz, int linhas, int colunas) {
    int *visitado;
    int i, j, objetos, indice;

    visitado = (int *) calloc(linhas * colunas, sizeof(int));

    if (visitado == NULL) {
        printf("Erro ao aclocar memória.\n");
        return -1;
    }

    objetos = 0;

    for (i = 0; i < linhas; i++) {

        for (j = 0; j < colunas; j++) {
            indice = i * colunas + j;

            if (matriz[indice] == 1 && visitado[indice] == 0) {
                objetos++;

                flood_fill(matriz, visitado, linhas, colunas, i,j);
            }
        }
    }
    free(visitado);

    return objetos;
}