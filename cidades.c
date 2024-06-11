#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cidades.h"

int comparaPosicao(const void *a, const void *b) {
    return ((Cidade *)a)->Posicao - ((Cidade *)b)->Posicao;
}

Estrada *getEstrada(const char *nomeArquivo) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo '%s'. Certifique-se de que o arquivo existe e está acessível.\n", nomeArquivo);
        return NULL;
    }

    Estrada *estrada = malloc(sizeof(Estrada));
    if (estrada == NULL) {
        fclose(arquivo);
        printf("Erro de alocação de memória.\n");
        return NULL;
    }

    if (fscanf(arquivo, "%d", &estrada->T) != 1 || fscanf(arquivo, "%d", &estrada->N) != 1) {
        fclose(arquivo);
        free(estrada);
        return NULL;
    }

    // Verificar se T e N estão dentro dos limites especificados
    if (estrada->T < 3 || estrada->T > 1000000 || estrada->N < 2 || estrada->N > 10000) {
        fclose(arquivo);
        free(estrada);
        return NULL;
    }

    estrada->C = malloc(estrada->N * sizeof(Cidade));
    if (estrada->C == NULL) {
        fclose(arquivo);
        free(estrada);
        return NULL;
    }

    // Ler e processar cada linha do arquivo para obter as informações das cidades
    for (int i = 0; i < estrada->N; i++) {
        if (fscanf(arquivo, "%d %[^\n]", &estrada->C[i].Posicao, estrada->C[i].Nome) != 2) {
            fclose(arquivo);
            free(estrada->C);
            free(estrada);
            return NULL;
        }
    }

    fclose(arquivo);

    // Ordena as cidades por posição ao longo da estrada
    qsort(estrada->C, estrada->N, sizeof(Cidade), comparaPosicao);

    return estrada;
}

double calcularMenorVizinhanca(const char *nomeArquivo) {
    Estrada *estrada = getEstrada(nomeArquivo);
    if (estrada == NULL) {
        return -1.0;
    }

    double menorVizinhanca = estrada->T;

    if (estrada->N == 1) {
        free(estrada->C);
        free(estrada);
        return menorVizinhanca;
    }

    for (int i = 0; i < estrada->N - 1; i++) {
        double vizinhanca = (estrada->C[i + 1].Posicao - estrada->C[i].Posicao) / 2.0;

        if (vizinhanca < menorVizinhanca) {
            menorVizinhanca = vizinhanca;
        }
    }

    free(estrada->C);
    free(estrada);

    return menorVizinhanca;
}

char *cidadeMenorVizinhanca(const char *nomeArquivo) {
    Estrada *estrada = getEstrada(nomeArquivo);
    if (estrada == NULL) {
        return NULL;
    }

    double menorVizinhanca = estrada->T;
    char *cidadeMenor = malloc(strlen(estrada->C[0].Nome) + 1);

    if (cidadeMenor == NULL) {
        free(estrada->C);
        free(estrada);
        return NULL;
    }

    strcpy(cidadeMenor, estrada->C[0].Nome);

    for (int i = 0; i < estrada->N - 1; i++) {
        double vizinhanca = (estrada->C[i + 1].Posicao - estrada->C[i].Posicao) / 2.0;

        if (vizinhanca < menorVizinhanca) {
            menorVizinhanca = vizinhanca;
            free(cidadeMenor);
            cidadeMenor = malloc(strlen(estrada->C[i].Nome) + 1);
            if (cidadeMenor == NULL) {
                free(estrada->C);
                free(estrada);
                return NULL;
            }
            strcpy(cidadeMenor, estrada->C[i].Nome);
        }
    }

    free(estrada->C);
    free(estrada);

    return cidadeMenor;
}
