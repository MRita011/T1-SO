#include <stdio.h>
#include <stdlib.h>

#include "lib/contagem.h"

int main(int argc, char *argv[]) {

    int teste_diagonal[] = {
        0, 1, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 1, 0
    };

    int teste_tres_regioes[] = {
        0, 1, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 1, 0,
        0, 1, 0, 0,
        0, 1, 0, 0
    };

    int teste_zeros[] = {
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0
    };

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

    int *matrizes[5];
    int linhas[5] = {5, 6, 8, 9, 12};
    int colunas[5] = {5, 8, 8, 12, 12};
    int esperados[5] = {3, 4, 5, 6, 7};

    int teste;
    int num_threads;
    int resultado;
    int thread_inicio;
    int thread_fim;

    matrizes[0] = matriz1;
    matrizes[1] = matriz2;
    matrizes[2] = matriz3;
    matrizes[3] = matriz4;
    matrizes[4] = matriz5;

    if (argc == 2) {
        num_threads = atoi(argv[1]);

        if (num_threads < 2 || num_threads > 4) {
            printf("Uso: ./paralelo [2|3|4]\n");
            return 1;
        }

        thread_inicio = num_threads;
        thread_fim = num_threads;
    }
    else if (argc == 1) {
        thread_inicio = 2;
        thread_fim = 4;
    }
    else {
        printf("Uso: ./paralelo [2|3|4]\n");
        return 1;
    }

    printf("\n");
    printf("+--------+----------+---------+----------+--------+----------+\n");
    printf("| Matriz | Dimensao | Threads | Esperado | Obtido | Status   |\n");
    printf("+--------+----------+---------+----------+--------+----------+\n");

    for (teste = 0; teste < 5; teste++) {

        for (num_threads = thread_inicio; num_threads <= thread_fim; num_threads++) {
            resultado = contar_objetos_paralelo(matrizes[teste], linhas[teste], colunas[teste], num_threads);
            printf("| %-6d | %2dx%-5d | %-7d | %-8d | %-6d | %-8s |\n", teste + 1, linhas[teste], colunas[teste], num_threads, esperados[teste], resultado, resultado == esperados[teste] ? "OK" : "ERRO");
        }

        if (teste < 4) {
            printf("+--------+----------+---------+----------+--------+----------+\n");
        }
    }

    printf("+--------+----------+---------+----------+--------+----------+\n");
    printf("\n");

    printf("Testes adicionais\n\n");
    printf("+---------------+----------+---------+----------+--------+----------+\n");
    printf("| Tipo          | Dimensao | Threads | Esperado | Obtido | Status   |\n");
    printf("+---------------+----------+---------+----------+--------+----------+\n");

    resultado = contar_objetos_paralelo(teste_diagonal, 4, 4, 2);
    printf("| %-13s | %2dx%-5d | %-7d | %-8d | %-6d | %-8s |\n", "Diagonal", 4, 4, 2, 1, resultado, resultado == 1 ? "OK" : "ERRO");

    resultado = contar_objetos_paralelo(teste_tres_regioes, 6, 4, 3);
    printf("| %-13s | %2dx%-5d | %-7d | %-8d | %-6d | %-8s |\n", "3 regioes", 6, 4, 3, 1, resultado, resultado == 1 ? "OK" : "ERRO");

    resultado = contar_objetos_paralelo(teste_zeros, 4, 4, 2);
    printf("| %-13s | %2dx%-5d | %-7d | %-8d | %-6d | %-8s |\n", "Matriz zerada", 4, 4, 2, 0, resultado, resultado == 0 ? "OK" : "ERRO");

    printf("+---------------+----------+---------+----------+--------+----------+\n");
    printf("\n");

    return 0;
}