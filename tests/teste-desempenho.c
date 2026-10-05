#define _POSIX_C_SOURCE 199309L

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../src/lib/contagem.h"

#define REPETICOES 5
#define LINHAS 4000
#define COLUNAS 4000

double tempo_atual(void) {
    struct timespec tempo;

    clock_gettime(CLOCK_MONOTONIC, &tempo);

    return tempo.tv_sec + tempo.tv_nsec / 1000000000.0;
}

void preencher_matriz(int *matriz, int linhas, int colunas) {
    int i;
    int j;
    int indice;

    for (i = 0; i < linhas; i++) {
        for (j = 0; j < colunas; j++) {
            indice = i * colunas + j;

            if ((i * 31 + j * 17 + (i * j) % 23) % 10 < 3) {
                matriz[indice] = 1;
            }
            else {
                matriz[indice] = 0;
            }
        }
    }
}

int main(void) {
    int *matriz;
    int total_celulas;
    int threads_testadas[3] = {2, 3, 4};
    int resultados_paralelos[3];

    int resultado_sequencial;
    int resultado;
    int repeticao;
    int i;

    double inicio;
    double fim;
    double tempo_sequencial;
    double tempos_paralelos[3];
    double speedup;

    total_celulas = LINHAS * COLUNAS;

    matriz = (int *)malloc(total_celulas * sizeof(int));

    if (matriz == NULL) {
        printf("Erro ao alocar matriz de desempenho.\n");
        return 1;
    }

    preencher_matriz(matriz, LINHAS, COLUNAS);

    tempo_sequencial = 0.0;
    resultado_sequencial = 0;

    for (repeticao = 0; repeticao < REPETICOES; repeticao++) {
        inicio = tempo_atual();
        resultado_sequencial = contar_objetos_sequencial(matriz, LINHAS, COLUNAS);
        fim = tempo_atual();

        tempo_sequencial += fim - inicio;
    }

    tempo_sequencial = tempo_sequencial / REPETICOES;

    for (i = 0; i < 3; i++) {
        tempos_paralelos[i] = 0.0;
        resultados_paralelos[i] = 0;

        for (repeticao = 0; repeticao < REPETICOES; repeticao++) {
            inicio = tempo_atual();
            resultado = contar_objetos_paralelo(matriz, LINHAS, COLUNAS, threads_testadas[i]);
            fim = tempo_atual();

            tempos_paralelos[i] += fim - inicio;
            resultados_paralelos[i] = resultado;
        }

        tempos_paralelos[i] = tempos_paralelos[i] / REPETICOES;
    }

    printf("\n");
    printf("Teste de desempenho - matriz %dx%d - %d repeticoes\n\n", LINHAS, COLUNAS, REPETICOES);

    printf("+------------+---------+----------+-------------+---------+----------+\n");
    printf("| Versao     | Threads | Objetos  | Tempo medio | Speedup | Status   |\n");
    printf("+------------+---------+----------+-------------+---------+----------+\n");

    printf("| %-10s | %-7s | %-8d | %-11.6f | %-7.2f | %-8s |\n", "Sequencial", "-", resultado_sequencial, tempo_sequencial, 1.0, resultado_sequencial >= 0 ? "OK" : "ERRO");

    for (i = 0; i < 3; i++) {
        speedup = tempo_sequencial / tempos_paralelos[i];
        printf("| %-10s | %-7d | %-8d | %-11.6f | %-7.2f | %-8s |\n", "Paralela", threads_testadas[i], resultados_paralelos[i], tempos_paralelos[i], speedup, resultados_paralelos[i] == resultado_sequencial ? "OK" : "ERRO");
    }

    printf("+------------+---------+----------+-------------+---------+----------+\n");
    printf("\n");

    free(matriz);

    return 0;
}