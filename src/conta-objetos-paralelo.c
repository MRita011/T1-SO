#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "flood-fill.h"
#include "union-find.h"

#define NUM_THREADS 2

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


/* Prototipos */

void *processar_regiao(void *arg);

int consolidar_fronteira(
    int *matriz,
    int *rotulos,
    int colunas,
    int linha_superior,
    int linha_inferior,
    UnionFind *uf
);


/*
 * Verifica a fronteira entre duas regioes.
 *
 * Para cada celula da linha superior, verifica:
 *
 * diagonal esquerda
 * vertical
 * diagonal direita
 *
 * Se duas celulas de valor 1 pertencem a componentes
 * diferentes, os seus rotulos sao unidos pelo Union-Find.
 */
int consolidar_fronteira(
    int *matriz,
    int *rotulos,
    int colunas,
    int linha_superior,
    int linha_inferior,
    UnionFind *uf
)
{
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

                if (coluna_inferior >= 0 &&
                    coluna_inferior < colunas) {

                    indice_inferior =
                        linha_inferior * colunas + coluna_inferior;

                    if (matriz[indice_inferior] == 1) {

                        rotulo_superior =
                            rotulos[indice_superior];

                        rotulo_inferior =
                            rotulos[indice_inferior];

                        if (rotulo_superior > 0 &&
                            rotulo_inferior > 0) {

                            unioes += uf_union(
                                uf,
                                rotulo_superior,
                                rotulo_inferior
                            );
                        }
                    }
                }
            }
        }
    }

    return unioes;
}


/*
 * Funcao executada por cada thread.
 *
 * Cada thread percorre somente as linhas
 * pertencentes a sua propria regiao.
 */
void *processar_regiao(void *arg)
{
    ThreadArgs *dados;
    int i;
    int j;
    int indice;
    int rotulo;

    dados = (ThreadArgs *)arg;

    dados->objetos_locais = 0;

    for (i = dados->linha_inicio;
         i <= dados->linha_fim;
         i++) {

        for (j = 0; j < dados->colunas; j++) {

            indice = i * dados->colunas + j;

            if (dados->matriz[indice] == 1 &&
                dados->rotulos[indice] == 0) {

                dados->objetos_locais++;

                /*
                 * Cada thread recebe uma faixa propria
                 * de rotulos para evitar repeticoes.
                 */
                rotulo =
                    dados->id_thread *
                    (dados->linhas * dados->colunas) +
                    dados->objetos_locais;

                flood_fill_regiao(
                    dados->matriz,
                    dados->rotulos,
                    dados->colunas,
                    dados->linha_inicio,
                    dados->linha_fim,
                    i,
                    j,
                    rotulo
                );
            }
        }
    }

    printf(
        "Thread %d: linhas %d a %d - %d componentes locais\n",
        dados->id_thread,
        dados->linha_inicio,
        dados->linha_fim,
        dados->objetos_locais
    );

    return NULL;
}


int main(void)
{
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

    pthread_t threads[NUM_THREADS];
    ThreadArgs args[NUM_THREADS];

    UnionFind uf;

    int *rotulos;

    int linhas;
    int colunas;

    int i;
    int retorno;

    int total_local;
    int total_global;
    int unioes;
    int max_rotulos;

    linhas = 8;
    colunas = 8;

    rotulos = (int *)calloc(
        linhas * colunas,
        sizeof(int)
    );

    if (rotulos == NULL) {
        printf("Erro ao alocar memoria para os rotulos.\n");
        return 1;
    }


    /*
     * Divide as linhas da matriz entre as threads.
     */
    for (i = 0; i < NUM_THREADS; i++) {

        args[i].matriz = matriz3;
        args[i].rotulos = rotulos;

        args[i].linhas = linhas;
        args[i].colunas = colunas;

        args[i].id_thread = i;

        args[i].linha_inicio =
            i * linhas / NUM_THREADS;

        args[i].linha_fim =
            ((i + 1) * linhas / NUM_THREADS) - 1;

        args[i].objetos_locais = 0;
    }


    /*
     * Cria as threads.
     */
    for (i = 0; i < NUM_THREADS; i++) {

        retorno = pthread_create(
            &threads[i],
            NULL,
            processar_regiao,
            &args[i]
        );

        if (retorno != 0) {
            printf("Erro ao criar thread %d\n", i);
            free(rotulos);
            return 1;
        }
    }


    /*
     * Aguarda todas as threads terminarem.
     */
    for (i = 0; i < NUM_THREADS; i++) {

        retorno = pthread_join(
            threads[i],
            NULL
        );

        if (retorno != 0) {
            printf("Erro ao aguardar thread %d\n", i);
            free(rotulos);
            return 1;
        }
    }


    /*
     * Soma os componentes encontrados
     * individualmente pelas threads.
     */
    total_local = 0;

    for (i = 0; i < NUM_THREADS; i++) {
        total_local += args[i].objetos_locais;
    }

    printf(
        "Total local antes da consolidacao: %d\n",
        total_local
    );


    /*
     * Cria o Union-Find.
     *
     * Cada thread pode utilizar uma faixa de
     * linhas * colunas rotulos.
     */
    max_rotulos =
        NUM_THREADS * linhas * colunas + 1;

    if (!uf_criar(&uf, max_rotulos)) {
        printf("Erro ao criar Union-Find.\n");
        free(rotulos);
        return 1;
    }


    /*
     * Consolida todas as fronteiras entre
     * regioes vizinhas.
     */
    unioes = 0;

    for (i = 0; i < NUM_THREADS - 1; i++) {

        unioes += consolidar_fronteira(
            matriz3,
            rotulos,
            colunas,
            args[i].linha_fim,
            args[i + 1].linha_inicio,
            &uf
        );
    }


    /*
     * Cada uniao valida representa dois
     * componentes locais que pertencem ao
     * mesmo objeto global.
     */
    total_global = total_local - unioes;

    printf(
        "Unioes entre regioes: %d\n",
        unioes
    );

    printf(
        "Total global de objetos: %d\n",
        total_global
    );


    uf_destruir(&uf);
    free(rotulos);

    return 0;
}