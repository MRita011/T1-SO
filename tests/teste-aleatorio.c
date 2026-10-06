#include <stdio.h>
#include <stdlib.h>

#include "../src/lib/contagem.h"

/* Compara a versao paralela com a sequencial em matrizes pseudoaleatorias */

#define CASOS 20000
#define LADO_MAXIMO 30
#define THREADS_MAXIMO 8
#define SEMENTE 12345

int main(void) {
    int caso;
    int linhas;
    int colunas;
    int densidade;
    int total_celulas;
    int i;
    int threads;
    int referencia;
    int obtido;
    int comparacoes;
    int divergencias;
    int *matriz;

    comparacoes = 0;
    divergencias = 0;

    srand(SEMENTE);

    for (caso = 0; caso < CASOS; caso++) {
        linhas = 1 + rand() % LADO_MAXIMO;
        colunas = 1 + rand() % LADO_MAXIMO;
        densidade = 5 + rand() % 70;
        total_celulas = linhas * colunas;

        matriz = (int *)malloc(total_celulas * sizeof(int));

        if (matriz == NULL) {
            printf("Erro ao alocar a matriz do caso %d.\n", caso);
            return 1;
        }

        for (i = 0; i < total_celulas; i++) {
            matriz[i] = (rand() % 100) < densidade;
        }

        referencia = contar_objetos_sequencial(matriz, linhas, colunas);

        for (threads = 1; threads <= THREADS_MAXIMO && threads <= linhas; threads++) {
            obtido = contar_objetos_paralelo(matriz, linhas, colunas, threads);
            comparacoes++;

            if (referencia < 0 || obtido != referencia) {
                divergencias++;

                if (divergencias <= 5) {
                    printf("DIVERGENCIA %dx%d com %d threads: sequencial %d, paralela %d\n", linhas, colunas, threads, referencia, obtido);
                }
            }
        }

        free(matriz);
    }

    printf("\nTeste aleatorio (semente %d)\n", SEMENTE);
    printf("Matrizes: %d (ate %dx%d), de 1 a %d threads\n", CASOS, LADO_MAXIMO, LADO_MAXIMO, THREADS_MAXIMO);
    printf("Comparacoes: %d\n", comparacoes);
    printf("Divergencias: %d\n\n", divergencias);

    return divergencias == 0 ? 0 : 1;
}
