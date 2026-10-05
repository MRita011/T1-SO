#define _POSIX_C_SOURCE 199309L

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../src/lib/contagem.h"

#define REPETICOES 5
#define QUANTIDADE_TAMANHOS 3
#define QUANTIDADE_THREADS 3

double tempo_atual(void) {
    struct timespec tempo;

    if (clock_gettime(CLOCK_MONOTONIC, &tempo) != 0) {
        return -1.0;
    }

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

double calcular_media(double *tempos) {
    double soma;
    int i;

    soma = 0.0;

    for (i = 0; i < REPETICOES; i++) {
        soma += tempos[i];
    }

    return soma / REPETICOES;
}

double calcular_minimo(double *tempos) {
    double minimo;
    int i;

    minimo = tempos[0];

    for (i = 1; i < REPETICOES; i++) {
        if (tempos[i] < minimo) {
            minimo = tempos[i];
        }
    }

    return minimo;
}

double calcular_maximo(double *tempos) {
    double maximo;
    int i;

    maximo = tempos[0];

    for (i = 1; i < REPETICOES; i++) {
        if (tempos[i] > maximo) {
            maximo = tempos[i];
        }
    }

    return maximo;
}

int executar_teste(int linhas, int colunas, FILE *arquivo) {
    int *matriz;
    int total_celulas;
    int threads_testadas[QUANTIDADE_THREADS] = {2, 3, 4};

    int resultados_sequenciais[REPETICOES];
    int resultados_paralelos[QUANTIDADE_THREADS][REPETICOES];

    double tempos_sequenciais[REPETICOES];
    double tempos_paralelos[QUANTIDADE_THREADS][REPETICOES];

    double inicio;
    double fim;
    double media_sequencial;
    double media_paralela;
    double minimo;
    double maximo;
    double speedup;
    double eficiencia;

    int resultado_referencia;
    int repeticao;
    int i;
    int status;
    int status_paralelo;

    total_celulas = linhas * colunas;

    matriz = (int *)malloc(total_celulas * sizeof(int));

    if (matriz == NULL) {
        printf("Erro ao alocar matriz de desempenho.\n");
        return 0;
    }

    preencher_matriz(matriz, linhas, colunas);

    for (repeticao = 0; repeticao < REPETICOES; repeticao++) {
        inicio = tempo_atual();

        if (inicio < 0.0) {
            printf("Erro ao obter tempo inicial.\n");
            free(matriz);
            return 0;
        }

        resultados_sequenciais[repeticao] = contar_objetos_sequencial(matriz, linhas, colunas);

        fim = tempo_atual();

        if (fim < 0.0) {
            printf("Erro ao obter tempo final.\n");
            free(matriz);
            return 0;
        }

        if (resultados_sequenciais[repeticao] < 0) {
            free(matriz);
            return 0;
        }

        tempos_sequenciais[repeticao] = (fim - inicio) * 1000.0;
    }

    resultado_referencia = resultados_sequenciais[0];

    for (i = 0; i < QUANTIDADE_THREADS; i++) {
        for (repeticao = 0; repeticao < REPETICOES; repeticao++) {
            inicio = tempo_atual();

            if (inicio < 0.0) {
                printf("Erro ao obter tempo inicial.\n");
                free(matriz);
                return 0;
            }

            resultados_paralelos[i][repeticao] = contar_objetos_paralelo(matriz, linhas, colunas, threads_testadas[i]);

            fim = tempo_atual();

            if (fim < 0.0) {
                printf("Erro ao obter tempo final.\n");
                free(matriz);
                return 0;
            }

            if (resultados_paralelos[i][repeticao] < 0) {
                free(matriz);
                return 0;
            }

            tempos_paralelos[i][repeticao] = (fim - inicio) * 1000.0;
        }
    }

    for (repeticao = 0; repeticao < REPETICOES; repeticao++) {
        status = resultados_sequenciais[repeticao] == resultado_referencia;
        fprintf(arquivo, "%dx%d,%d,%d,sequencial,1,%d,%.6f,%d,%s\n", linhas, colunas, linhas, colunas, repeticao + 1, tempos_sequenciais[repeticao], resultados_sequenciais[repeticao], status ? "true" : "false");
    }

    for (i = 0; i < QUANTIDADE_THREADS; i++) {
        for (repeticao = 0; repeticao < REPETICOES; repeticao++) {
            status = resultados_paralelos[i][repeticao] == resultado_referencia;
            fprintf(arquivo, "%dx%d,%d,%d,paralela,%d,%d,%.6f,%d,%s\n", linhas, colunas, linhas, colunas, threads_testadas[i], repeticao + 1, tempos_paralelos[i][repeticao], resultados_paralelos[i][repeticao], status ? "true" : "false");
        }
    }

    media_sequencial = calcular_media(tempos_sequenciais);
    minimo = calcular_minimo(tempos_sequenciais);
    maximo = calcular_maximo(tempos_sequenciais);

    printf("\n");
    printf("Matriz %dx%d\n\n", linhas, colunas);

    printf("+------------+---------+----------+----------+----------+----------+----------+----------+----------+----------+\n");
    printf("| Versao     | Threads | Rep. 1   | Rep. 2   | Rep. 3   | Rep. 4   | Rep. 5   | Media    | Minimo   | Maximo   |\n");
    printf("+------------+---------+----------+----------+----------+----------+----------+----------+----------+----------+\n");

    printf("| %-10s | %-7s | %-8.3f | %-8.3f | %-8.3f | %-8.3f | %-8.3f | %-8.3f | %-8.3f | %-8.3f |\n", "Sequencial", "-", tempos_sequenciais[0], tempos_sequenciais[1], tempos_sequenciais[2], tempos_sequenciais[3], tempos_sequenciais[4], media_sequencial, minimo, maximo);

    for (i = 0; i < QUANTIDADE_THREADS; i++) {
        media_paralela = calcular_media(tempos_paralelos[i]);
        minimo = calcular_minimo(tempos_paralelos[i]);
        maximo = calcular_maximo(tempos_paralelos[i]);

        printf("| %-10s | %-7d | %-8.3f | %-8.3f | %-8.3f | %-8.3f | %-8.3f | %-8.3f | %-8.3f | %-8.3f |\n", "Paralela", threads_testadas[i], tempos_paralelos[i][0], tempos_paralelos[i][1], tempos_paralelos[i][2], tempos_paralelos[i][3], tempos_paralelos[i][4], media_paralela, minimo, maximo);
    }

    printf("+------------+---------+----------+----------+----------+----------+----------+----------+----------+----------+\n");

    printf("\n");
    printf("+------------+---------+----------+-------------+---------+------------+----------+\n");
    printf("| Versao     | Threads | Objetos  | Media (ms)  | Speedup | Eficiencia | Status   |\n");
    printf("+------------+---------+----------+-------------+---------+------------+----------+\n");

    printf("| %-10s | %-7s | %-8d | %-11.3f | %-7.2f | %-10.2f | %-8s |\n", "Sequencial", "-", resultado_referencia, media_sequencial, 1.0, 1.0, "OK");

    for (i = 0; i < QUANTIDADE_THREADS; i++) {
        media_paralela = calcular_media(tempos_paralelos[i]);
        speedup = media_sequencial / media_paralela;
        eficiencia = speedup / threads_testadas[i];
        status_paralelo = 1;

        for (repeticao = 0; repeticao < REPETICOES; repeticao++) {
            if (resultados_paralelos[i][repeticao] != resultado_referencia) {
                status_paralelo = 0;
            }
        }

        printf("| %-10s | %-7d | %-8d | %-11.3f | %-7.2f | %-10.2f | %-8s |\n", "Paralela", threads_testadas[i], resultados_paralelos[i][0], media_paralela, speedup, eficiencia, status_paralelo ? "OK" : "ERRO");
    }

    printf("+------------+---------+----------+-------------+---------+------------+----------+\n");

    free(matriz);

    return 1;
}

int main(void) {
    int tamanhos[QUANTIDADE_TAMANHOS] = {1000, 2000, 4000};
    FILE *arquivo;
    int i;

    arquivo = fopen("results/medicoes.csv", "w");

    if (arquivo == NULL) {
        printf("Erro ao criar results/medicoes.csv.\n");
        return 1;
    }

    fprintf(arquivo, "matriz,linhas,colunas,versao,trabalhadores,repeticao,tempo_ms,objetos,resultado_correto\n");

    printf("\n");
    printf("TESTES DE DESEMPENHO\n");
    printf("Repeticoes por configuracao: %d\n", REPETICOES);
    printf("Unidade de tempo: milissegundos\n");

    for (i = 0; i < QUANTIDADE_TAMANHOS; i++) {
        if (!executar_teste(tamanhos[i], tamanhos[i], arquivo)) {
            fclose(arquivo);
            return 1;
        }
    }

    if (fclose(arquivo) != 0) {
        printf("Erro ao fechar results/medicoes.csv.\n");
        return 1;
    }

    printf("\nDados brutos salvos em results/medicoes.csv\n\n");

    return 0;
}