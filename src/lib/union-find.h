#ifndef UNION_FIND_H
#define UNION_FIND_H

typedef struct {
    int *pai;
    int *tamanho;
    int quantidade;
} UnionFind;

int uf_criar(UnionFind *uf, int quantidade);
void uf_destruir(UnionFind *uf);
int uf_find(UnionFind *uf, int elemento);
int uf_union(UnionFind *uf, int a, int b);

#endif