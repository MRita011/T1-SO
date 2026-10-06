#include <stdlib.h>
#include "union-find.h"

int uf_criar(UnionFind *uf, int quantidade) {
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

void uf_destruir(UnionFind *uf) {
    free(uf->pai);
    free(uf->tamanho);

    uf->pai = NULL;
    uf->tamanho = NULL;
    uf->quantidade = 0;
}

int uf_find(UnionFind *uf, int elemento) {
    if (uf->pai[elemento] != elemento) {
        uf->pai[elemento] = uf_find(uf, uf->pai[elemento]);
    }

    return uf->pai[elemento];
}
int uf_union(UnionFind *uf, int a, int b) {
    int raiz_a;
    int raiz_b;

    raiz_a = uf_find(uf, a);
    raiz_b = uf_find(uf, b);

    if (raiz_a == raiz_b) {
        return 0;
    }

    if (uf->tamanho[raiz_a] < uf->tamanho[raiz_b]) {
        uf->pai[raiz_a] = raiz_b;
        uf->tamanho[raiz_b] += uf->tamanho[raiz_a];
    }
    else {
        uf->pai[raiz_b] = raiz_a;
        uf->tamanho[raiz_a] += uf->tamanho[raiz_b];
    }

    return 1;
}