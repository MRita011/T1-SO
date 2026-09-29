#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct {
    int *matriz;
    int *rotulos; /* onde registra os objetos encontrados por cada thread */
    int linhas;
    int colunas;
    int linha_inicio;
    int linha_fim;
    int id_thread; /* qual thread é*/
    int objetos_locais; /* quantos objetos foram encontrados na região da thread */
} ThreadArgs;

int contar_objetos(int *matriz, int linhas, int colunas);

void flood_fill(
    int *matriz,
    int *visitado,
    int linhas,
    int colunas,
    int linha,
    int coluna
);

void flood_fill_regiao(
    int *matriz,
    int *rotulos,
    int colunas,
    int linha_inicio,
    int linha_fim,
    int linha,
    int coluna,
    int rotulo
);

void *processar_regiao(void *arg);

int main (void) {
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

    pthread_t threads[2];
    ThreadArgs args[2];
    int i, retorno;
    int *rotulos;

    rotulos = (int *) calloc(8 * 8, sizeof(int));
    if (rotulos == NULL) {
        printf("Erro ao alocar memória para os rótulos.\n");
        return 1;
    }

    /* thread que percorre da linha 0 ate a linha 3 (matriz 3) */
    args[0].matriz = matriz3;
    args[0].rotulos = rotulos;
    args[0].id_thread = 0;
    args[0].linhas = 8;
    args[0].colunas = 8;
    args[0].linha_inicio = 0;
    args[0].linha_fim = 3;
    args[0].objetos_locais = 0;

    /* thread que percorre da linha 4 ate a linha 7 (matriz 3) */
    args[1].matriz = matriz3;
    args[1].rotulos = rotulos;
    args[1].id_thread = 1;
    args[1].linhas = 8;
    args[1].colunas = 8;
    args[1].linha_inicio = 4;
    args[1].linha_fim = 7;
    args[1].objetos_locais = 0;


    for (i = 0; i < 2; i++) {
        retorno = pthread_create(&threads[i], NULL, processar_regiao, &args[i]);
        if (retorno != 0) {
            printf("Erro ao criar thread %d\n", i);
            free(rotulos);
            return 1;
        }
    }

    for (i = 0; i < 2; i++) {
        retorno = pthread_join(threads[i], NULL);

        if(retorno != 0) {
            printf("Erro ao aguardar thread %d\n", i);
            free(rotulos);
            return 1;
        }
    }
    
    printf("Total local antes da contagem final: %d\n", args[0].objetos_locais + args[1].objetos_locais);
    free(rotulos);
    return 0;
}

void flood_fill(int *matriz, int *visitado, int linhas, int colunas, int linha, int coluna) {
    int dl[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int dc[8] = {-1,  0,  1, -1, 1, -1, 0, 1};

    int nova_linha, nova_coluna;
    int k,indice, novo_indice;

    indice = linha * colunas + coluna;

    visitado[indice] = 1;

    for (k = 0; k < 8; k++) {
        nova_linha = linha + dl[k];
        nova_coluna = coluna + dc[k];

        if (nova_linha >= 0 && nova_linha < linhas && nova_coluna >= 0 && nova_coluna < colunas) {
            novo_indice = nova_linha * colunas + nova_coluna;

            if (matriz[novo_indice] == 1 && visitado[novo_indice] == 0) {
                flood_fill(matriz, visitado, linhas, colunas, nova_linha, nova_coluna);
            }
        }
    }
}

int contar_objetos(int *matriz, int linhas, int colunas) {
    int *visitado;
    int i, j, objetos, indice;

    visitado = (int *) calloc(linhas * colunas, sizeof(int));

    if (visitado == NULL) {
        printf("Erro ao aclocar memória.\n");
        return -1;
    }

    objetos = 0;

    for (i = 0; i < linhas; i++) {

        for (j = 0; j < colunas; j++) {
            indice = i * colunas + j;

            if (matriz[indice] == 1 && visitado[indice] == 0) {
                objetos++;

                flood_fill(matriz, visitado, linhas, colunas, i,j);
            }
        }
    }
    free(visitado);
    return objetos;
}

void flood_fill_regiao(
    int *matriz,
    int *rotulos,
    int colunas,
    int linha_inicio,
    int linha_fim,
    int linha,
    int coluna,
    int rotulo
) {
    int dl[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int dc[8] = {-1,  0,  1, -1, 1, -1, 0, 1};

    int nova_linha, nova_coluna;
    int k,indice, novo_indice;

    indice = linha * colunas + coluna;

    rotulos[indice] = rotulo;

    for (k = 0; k < 8; k++) {
        nova_linha = linha + dl[k];
        nova_coluna = coluna + dc[k];

        if (nova_linha >= linha_inicio && nova_linha <= linha_fim && nova_coluna >= 0 && nova_coluna < colunas) {
            novo_indice = nova_linha * colunas + nova_coluna;

            if (matriz[novo_indice] == 1 && rotulos[novo_indice] == 0) {
                flood_fill_regiao(matriz, rotulos, colunas, linha_inicio, linha_fim, nova_linha, nova_coluna, rotulo);
            }
        }
    }
}

void *processar_regiao(void *arg) {
    ThreadArgs *dados;
    int i, j, indice, rotulo;

    dados = (ThreadArgs *) arg;
    dados -> objetos_locais = 0;
    
    for (i = dados -> linha_inicio; i <= dados -> linha_fim; i++) {
        for (j = 0; j < dados -> colunas; j++) {
            indice = i * dados -> colunas + j;

            if (dados -> matriz[indice] == 1 && dados -> rotulos[indice] == 0) {
                
                dados -> objetos_locais++;

                rotulo = dados -> id_thread * (dados -> linhas * dados -> colunas) + dados -> objetos_locais;

                flood_fill_regiao(dados -> matriz, dados -> rotulos, dados -> colunas, dados -> linha_inicio, dados -> linha_fim, i, j, rotulo);
            }
        }
    }

    printf("Thread %d: linhas %d a %d - %d componentes locais\n", dados -> id_thread, dados -> linha_inicio, dados -> linha_fim, dados -> objetos_locais);
    return NULL;
}