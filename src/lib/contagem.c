#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "contagem.h"
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
    int erro;
} ThreadArgs;

static void *processar_regiao(void *arg);
static int consolidar_fronteira(int *matriz, int *rotulos, int colunas, int linha_superior, int linha_inferior, UnionFind *uf);


/* Contagem sequencial */

int contar_objetos_sequencial(int *matriz, int linhas, int colunas) {
    int *visitado, *pilha;
    int objetos;
    int i;
    int j;
    int indice; 
    int total_celulas;

    total_celulas = linhas * colunas;

    visitado = (int *)calloc(total_celulas, sizeof(int));
    pilha = (int *)malloc(total_celulas * sizeof(int));

    if (visitado == NULL || pilha == NULL) {
        printf("Erro ao alocar memoria para a contagem sequencial.\n");
        free(visitado);
        free(pilha);
        return -1;
    }

    objetos = 0;
    
    for (i = 0; i < linhas; i++) {
        for (j = 0; j < colunas; j++) {
            indice = i * colunas + j;

            if (matriz[indice] == 1 && visitado[indice] == 0) {
                objetos++;
                flood_fill(matriz, visitado, linhas, colunas, i, j, pilha);
            }
        }
    }
    free(visitado);
    free(pilha);
    return objetos;
}


/* Verifica a fronteira entre duas regioes */

static int consolidar_fronteira(int *matriz, int *rotulos, int colunas, int linha_superior, int linha_inferior, UnionFind *uf) {
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


/* Funcao executada por cada thread */

static void *processar_regiao(void *arg) {
    ThreadArgs *dados;
    int *pilha;
    int total_celulas_regiao;
    int i;
    int j;
    int indice;
    int rotulo;

    dados = (ThreadArgs *)arg;

    dados->objetos_locais = 0;
    dados->erro = 0;

    total_celulas_regiao = (dados->linha_fim - dados->linha_inicio + 1) * dados->colunas;

    pilha = (int *)malloc(total_celulas_regiao * sizeof(int));

    if (pilha == NULL) {
        dados->erro = 1;
        return NULL;
    }

    for (i = dados->linha_inicio; i <= dados->linha_fim; i++) {
        for (j = 0; j < dados->colunas; j++) {
            indice = i * dados->colunas + j;

            if (dados->matriz[indice] == 1 && dados->rotulos[indice] == 0) {
                dados->objetos_locais++;
                rotulo = indice + 1;
                flood_fill_regiao(dados->matriz, dados->rotulos, dados->colunas, dados->linha_inicio, dados->linha_fim, i, j, rotulo, pilha);
            }
        }
    }

    free(pilha);

    return NULL;
}


/* Contagem paralela */

int contar_objetos_paralelo(int *matriz, int linhas, int colunas, int num_threads) {
    pthread_t *threads;
    ThreadArgs *args;
    UnionFind uf;

    int *rotulos;
    int total_celulas;
    int max_rotulos;

    int i;
    int j;
    int retorno;
    int threads_criadas;
    int erro_join;

    int total_local;
    int total_global;
    int unioes;

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
        args[i].erro = 0;
    }

    threads_criadas = 0;

    for (i = 0; i < num_threads; i++) {
        retorno = pthread_create(&threads[i], NULL, processar_regiao, &args[i]);

        if (retorno != 0) {
            printf("Erro ao criar thread %d\n", i);

            erro_join = 0;

            for (j = 0; j < threads_criadas; j++) {
                retorno = pthread_join(threads[j], NULL);

                if (retorno != 0) {
                    printf("Erro ao aguardar thread %d durante limpeza\n", j);
                    erro_join = 1;
                }
            }

            if (erro_join) {
                printf("Nao foi possivel finalizar todas as threads com seguranca.\n");
                exit(EXIT_FAILURE);
            }

            free(threads);
            free(args);
            free(rotulos);

            return -1;
        }

        threads_criadas++;
    }

    erro_join = 0;

    for (i = 0; i < num_threads; i++) {
        retorno = pthread_join(threads[i], NULL);

        if (retorno != 0) {
            printf("Erro ao aguardar thread %d\n", i);
            erro_join = 1;
        }
    }

    if (erro_join) {
        printf("Nao foi possivel finalizar todas as threads com seguranca.\n");
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < num_threads; i++) {
        if (args[i].erro) {
            printf("Erro ao alocar memoria em uma das threads.\n");
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

    max_rotulos = total_celulas + 1;

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