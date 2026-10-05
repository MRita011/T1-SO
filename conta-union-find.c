#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *pai;
    int *tamanho;
    int quantidade;
} UnionFind;

int uf_criar(UnionFind *uf, int quantidade)
{
    int i;

    uf->pai = (int *)malloc(quantidade * sizeof(int));
    uf->tamanho = (int *)malloc(quantidade * sizeof(int));

    if (uf->pai == NULL || uf->tamanho == NULL) {
        free(uf->pai);
        free(uf->tamanho);
        return 0;
    }

    uf->quantidade = quantidade;

    for (i = 0; i < quantidade; i++) {
        uf->pai[i] = i;
        uf->tamanho[i] = 1;
    }

    return 1;
}

void uf_destruir(UnionFind *uf)
{
    free(uf->pai);
    free(uf->tamanho);

    uf->pai = NULL;
    uf->tamanho = NULL;
    uf->quantidade = 0;
}

int uf_find(UnionFind *uf, int elemento)
{
    if (uf->pai[elemento] != elemento) {
        uf->pai[elemento] = uf_find(uf, uf->pai[elemento]);
    }

    return uf->pai[elemento];
}

void uf_union(UnionFind *uf, int a, int b)
{
    int raiz_a;
    int raiz_b;

    raiz_a = uf_find(uf, a);
    raiz_b = uf_find(uf, b);

    if (raiz_a == raiz_b) {
        return;
    }

    if (uf->tamanho[raiz_a] < uf->tamanho[raiz_b]) {
        uf->pai[raiz_a] = raiz_b;
        uf->tamanho[raiz_b] += uf->tamanho[raiz_a];
    }
    else {
        uf->pai[raiz_b] = raiz_a;
        uf->tamanho[raiz_a] += uf->tamanho[raiz_b];
    }
}

int indice(int linha, int coluna, int colunas)
{
    return linha * colunas + coluna;
}


/*
Conta os objetos da matriz usando Union-Find.
Conectividade 8:
(-1,-1)  (-1,0)  (-1,+1)
( 0,-1)    X     ( 0,+1)
(+1,-1)  (+1,0)  (+1,+1)
Cada grupo de 1s conectados representa uma figura.
 */

int contar_objetos_union_find(int *matriz, int linhas, int colunas)
{
    UnionFind uf;

    int total;
    int i;
    int j;
    int k;

    int atual;
    int vizinho;

    int nova_linha;
    int nova_coluna;

    int objetos;

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

    total = linhas * colunas;

    if (!uf_criar(&uf, total)) {
        return -1;
    }

    for (i = 0; i < linhas; i++) {

        for (j = 0; j < colunas; j++) {

            atual = indice(i, j, colunas);

            if (matriz[atual] == 0) {
                continue;
            }

            for (k = 0; k < 8; k++) {

                nova_linha = i + delta_linha[k];
                nova_coluna = j + delta_coluna[k];

                 /* Verifica se o vizinho esta dentro da matriz */
                 
                if (nova_linha >= 0 &&
                    nova_linha < linhas &&
                    nova_coluna >= 0 &&
                    nova_coluna < colunas) {

                    vizinho = indice(
                        nova_linha,
                        nova_coluna,
                        colunas
                    );

                    if (matriz[vizinho] == 1) {
                        uf_union(&uf, atual, vizinho);
                    }
                }
            }
        }
    }

    objetos = 0;

    for (i = 0; i < total; i++) {

        if (matriz[i] == 0) {
            continue;
        }

        if (uf_find(&uf, i) == i) {
            objetos++;
        }
    }

    uf_destruir(&uf);

    return objetos;
}

/* Exemplo de utilizacao */

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

    resultado = contar_objetos_union_find(
        &matriz[0][0],
        5,
        5
    );

    printf("Objetos encontrados: %d\n", resultado);

    return 0;
}