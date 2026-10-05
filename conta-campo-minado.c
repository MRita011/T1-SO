#include <stdio.h>
#include <stdlib.h>

/*
 * Busca de figuras em matriz binaria utilizando
 * o algoritmo de Flood Fill (estilo Campo Minado).
 *
 * Conectividade 8:
 *
 * (-1,-1) (-1,0) (-1,+1)
 * ( 0,-1)   X    ( 0,+1)
 * (+1,-1) (+1,0) (+1,+1)
 */


int dentro_da_matriz(int linha, int coluna,
                     int linhas, int colunas)
{
    return linha >= 0 &&
           linha < linhas &&
           coluna >= 0 &&
           coluna < colunas;
}

void flood_fill(int *matriz,
                int *visitado,
                int linhas,
                int colunas,
                int linha,
                int coluna)
{
    int delta_linha[8] = {
        -1, -1, -1,
         0,  0,
         1,  1,  1
    };

    int delta_coluna[8] = {
        -1,  0,  1,
        -1,  1,
        -1,  0,  1
    };

    int i;
    int nova_linha;
    int nova_coluna;
    int atual;
    int vizinho;

    atual = linha * colunas + coluna;

    if (!dentro_da_matriz(linha, coluna,
                          linhas, colunas)) {
        return;
    }

    if (matriz[atual] == 0 || visitado[atual] == 1) {
        return;
    }

    visitado[atual] = 1;

    for (i = 0; i < 8; i++) {

        nova_linha = linha + delta_linha[i];
        nova_coluna = coluna + delta_coluna[i];

        if (dentro_da_matriz(nova_linha,
                             nova_coluna,
                             linhas,
                             colunas)) {

            vizinho = nova_linha * colunas + nova_coluna;

            if (matriz[vizinho] == 1 &&
                visitado[vizinho] == 0) {

                flood_fill(matriz,
                           visitado,
                           linhas,
                           colunas,
                           nova_linha,
                           nova_coluna);
            }
        }
    }
}

int contar_objetos_campo_minado(int *matriz,
                                int linhas,
                                int colunas)
{
    int *visitado;

    int i;
    int j;
    int indice;
    int objetos;

    visitado = (int *)calloc(
        linhas * colunas,
        sizeof(int)
    );

    if (visitado == NULL) {
        return -1;
    }

    objetos = 0;

    /*
     * Percorre toda a matriz.
     */
    for (i = 0; i < linhas; i++) {

        for (j = 0; j < colunas; j++) {

            indice = i * colunas + j;

            
            if (matriz[indice] == 1 &&
                visitado[indice] == 0) {

               
                objetos++;

                flood_fill(matriz,
                           visitado,
                           linhas,
                           colunas,
                           i,
                           j);
            }
        }
    }

    free(visitado);

    return objetos;
}


/*
 * Exemplo de utilizacao.
 */
int main(void)
{
    int matriz[5][5] = {
        {1, 1, 0, 0, 0},
        {1, 0, 0, 0, 0},
        {0, 0, 0, 1, 0},
        {0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0}
    };

    int resultado;

    resultado = contar_objetos_campo_minado(
        &matriz[0][0],
        5,
        5
    );

    printf("Objetos encontrados: %d\n", resultado);

    return 0;
}