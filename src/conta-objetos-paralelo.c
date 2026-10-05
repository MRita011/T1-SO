#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/* modularalizacao */
#include "flood-fill.h"
#include "union-find.h"

typedef struct {
    int *matriz;
    int *rotulos;
    int linhas;
    int colunas;
    int linha_inicio;
    int linha_fim;
    int id_thread;
    int objetos_locais;
} ThreadArgs;

void *processar_regiao(void *arg);

int consolidar_fronteira(
    int *matriz,
    int *rotulos,
    int colunas,
    int linha_superior,
    int linha_inferior,
    UnionFind *uf
);

int contar_objetos_paralelo(
    int *matriz,
    int linhas,
    int colunas,
    int num_threads
);

/*
 * verifica a fronteira entre duas regioes

 * para cada celula da linha superior, verifica:
 
 * diagonal esquerda
 * vertical
 * diagonal direita
 
 * se duas celulas de valor 1 pertencem a componentes
 * diferentes, os seus rotulos sao unidos pelo Union-Find
 */

int consolidar_fronteira(int *matriz, int *rotulos, int colunas, int linha_superior, int linha_inferior, UnionFind *uf) {
    int coluna;
    int deslocamento;
    int coluna_inferior;
    int indice_superior;
    int indice_inferior;
    int rotulo_superior;
    int rotulo_inferior;
    int unioes;

    unioes = 0;
    
    for (coluna = 0; coluna < colunas; coluna++) {
        indice_superior = linha_superior * colunas + coluna;
        
        if (matriz[indice_superior] == 1) {
            for (deslocamento = -1; deslocamento <= 1; deslocamento++) {
                coluna_inferior = coluna + deslocamento;
                
                if (coluna_inferior >= 0 && coluna_inferior < colunas) {
                    indice_inferior = linha_inferior * colunas + coluna_inferior;
                    
                    if (matriz[indice_inferior] == 1) {
                        rotulo_superior = rotulos[indice_superior];
                        rotulo_inferior = rotulos[indice_inferior];

                        if (rotulo_superior > 0 && rotulo_inferior > 0) {
                            
                            unioes += uf_union(uf, rotulo_superior, rotulo_inferior);
                        }
                    }
                }
            }
        }
    }
    return unioes;
}

/*
 * funcao executada por cada thread
 *
 * cada thread percorre somente as linhas
 * pertencentes a sua propria regiao
 */

void *processar_regiao(void *arg) {
    ThreadArgs *dados;
    int i;
    int j;
    int indice;
    int rotulo;

    dados = (ThreadArgs *)arg;

    dados->objetos_locais = 0;

    for (i = dados->linha_inicio; i <= dados->linha_fim; i++) {
        for (j = 0; j < dados->colunas; j++) {
            indice = i * dados->colunas + j;
            
            if (dados->matriz[indice] == 1 && dados->rotulos[indice] == 0) {
                dados->objetos_locais++;

                /* cada thread recebe uma faixa propria de rotulos para evitar repeticoes */
                rotulo = dados->id_thread * (dados->linhas * dados->colunas) + dados->objetos_locais;


                flood_fill_regiao(dados->matriz, dados->rotulos, dados->colunas, dados->linha_inicio, dados->linha_fim, i, j, rotulo);
            }
        }
    }
    return NULL;
}

int contar_objetos_paralelo(int *matriz, int linhas, int colunas, int num_threads) {
    pthread_t *threads;
    ThreadArgs *args;
    UnionFind uf;

    int *rotulos;
    int total_celulas;
    int max_rotulos;

    int i, j, retorno;
    int threads_criadas;

    int total_local, total_global, unioes;

    if (num_threads < 1 || num_threads > linhas) {
        printf("Quantidade de threads invalida.\n");
        return -1;
    }

    total_celulas = linhas * colunas;

    rotulos = (int *)calloc(total_celulas, sizeof(int));

    if (rotulos == NULL) {
        printf("Erro ao alocar memoria para os rotulos.\n");
        return -1;
    }

    threads = (pthread_t *)malloc(num_threads * sizeof(pthread_t));
    args = (ThreadArgs *)malloc(num_threads * sizeof(ThreadArgs));

    if (threads == NULL || args == NULL) {
        printf("Erro ao alocar memoria para as threads.\n");
        free(threads);
        free(args);
        free(rotulos);
        return -1;
    }

    for (i = 0; i < num_threads; i++) {
        args[i].matriz = matriz;
        args[i].rotulos = rotulos;
        args[i].linhas = linhas;
        args[i].colunas = colunas;
        args[i].id_thread = i;

        args[i].linha_inicio = i * linhas / num_threads;
        args[i].linha_fim = ((i + 1) * linhas / num_threads) - 1;

        args[i].objetos_locais = 0;
    }

    threads_criadas = 0;

    for (i = 0; i < num_threads; i++) {
        retorno = pthread_create(&threads[i], NULL, processar_regiao, &args[i]);

        if (retorno != 0) {
            printf("Erro ao criar thread %d\n", i);

            for (j = 0; j < threads_criadas; j++) {
                pthread_join(threads[j], NULL);
            }

            free(threads);
            free(args);
            free(rotulos);

            return -1;
        }

        threads_criadas++;
    }

    for (i = 0; i < num_threads; i++) {
        retorno = pthread_join(threads[i], NULL);

        if (retorno != 0) {
            printf("Erro ao aguardar thread %d\n", i);
            free(threads);
            free(args);
            free(rotulos);
            return -1;
        }
    }

    total_local = 0;

    for (i = 0; i < num_threads; i++) {
        total_local += args[i].objetos_locais;
    }

    max_rotulos = num_threads * total_celulas + 1;

    if (!uf_criar(&uf, max_rotulos)) {
        printf("Erro ao criar Union-Find.\n");
        free(threads);
        free(args);
        free(rotulos);
        return -1;
    }

    unioes = 0;

    for (i = 0; i < num_threads - 1; i++) {
        unioes += consolidar_fronteira(matriz, rotulos, colunas, args[i].linha_fim, args[i + 1].linha_inicio, &uf);
    }

    total_global = total_local - unioes;

    uf_destruir(&uf);
    free(threads);
    free(args);
    free(rotulos);

    return total_global;
}

int main(void) {
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

    int resultado;

    int esperado;

    printf("\n");
    printf("+-------+----------+---------+----------+--------+----------+\n");
    printf("| Teste | Dimensao | Threads | Esperado | Obtido | Status   |\n");
    printf("+-------+----------+---------+----------+--------+----------+\n");

    esperado = 3;
    resultado = contar_objetos_paralelo(matriz1, 5, 5, 2);
    printf("| %-5d | %-8s | %-7d | %-8d | %-6d | %-8s |\n", 1, "5x5", 2, esperado, resultado, resultado == esperado ? "OK" : "ERRO");

    esperado = 4;
    resultado = contar_objetos_paralelo(matriz2, 6, 8, 2);
    printf("| %-5d | %-8s | %-7d | %-8d | %-6d | %-8s |\n", 2, "6x8", 2, esperado, resultado, resultado == esperado ? "OK" : "ERRO");

    esperado = 5;
    resultado = contar_objetos_paralelo(matriz3, 8, 8, 2);
    printf("| %-5d | %-8s | %-7d | %-8d | %-6d | %-8s |\n", 3, "8x8", 2, esperado, resultado, resultado == esperado ? "OK" : "ERRO");

    esperado = 6;
    resultado = contar_objetos_paralelo(matriz4, 9, 12, 2);
    printf("| %-5d | %-8s | %-7d | %-8d | %-6d | %-8s |\n", 4, "9x12", 2, esperado, resultado, resultado == esperado ? "OK" : "ERRO");

    esperado = 7;
    resultado = contar_objetos_paralelo(matriz5, 12, 12, 2);
    printf("| %-5d | %-8s | %-7d | %-8d | %-6d | %-8s |\n", 5, "12x12", 2, esperado, resultado, resultado == esperado ? "OK" : "ERRO");

    printf("+-------+----------+---------+----------+--------+----------+\n");
    printf("\n");

    return 0;
}