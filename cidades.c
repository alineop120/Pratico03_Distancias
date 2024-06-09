#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cidades.h"

// Função auxiliar para ordenar as cidades por posição ao longo da estrada
int comparaPosicao(const void *a, const void *b) {
    return ((Cidade *)a)->Posicao - ((Cidade *)b)->Posicao;
}

// Função para inicializar a estrutura Estrada a partir do arquivo
Estrada *getEstrada(const char *nomeArquivo) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo '%s'. Certifique-se de que o arquivo existe e está acessivel.\n", nomeArquivo);
        return NULL;
    }

    Estrada *estrada = malloc(sizeof(Estrada));
    if (estrada == NULL) {
        fclose(arquivo);
        printf("Erro de alocacao de memoria.\n");
        return NULL;
    }

    fscanf(arquivo, "%d", &estrada->T);
    fscanf(arquivo, "%d", &estrada->N);

    // Verifica as restrições
    if (estrada->T < 3 || estrada->T > 1000000 || estrada->N < 2 || estrada->N > 10000) {
        fclose(arquivo);
        free(estrada);
        return NULL;
    }

    estrada->C = malloc(estrada->N * sizeof(Cidade));
    if (estrada->C == NULL) {
        fclose(arquivo);
        free(estrada);
        printf("Erro de alocacao de memoria.\n");
        return NULL;
    }

    for (int i = 0; i < estrada->N; i++) {
        fscanf(arquivo, "%d", &estrada->C[i].Posicao);
        fscanf(arquivo, "%s", estrada->C[i].Nome);

        // Verifica as restrições
        if (estrada->C[i].Posicao <= 0 || estrada->C[i].Posicao >= estrada->T) {
            fclose(arquivo);
            free(estrada->C);
            free(estrada);
            return NULL;
        }

        // Verifica se a posição é única
        for (int j = 0; j < i; j++) {
            if (estrada->C[j].Posicao == estrada->C[i].Posicao) {
                fclose(arquivo);
                free(estrada->C);
                free(estrada);
                return NULL;
            }
        }
    }

    fclose(arquivo);

    // Ordena as cidades por posição ao longo da estrada
    qsort(estrada->C, estrada->N, sizeof(Cidade), comparaPosicao);

    return estrada;
}

// Função para calcular a menor vizinhança
double calcularMenorVizinhanca(const char *nomeArquivo) {
    Estrada *estrada = getEstrada(nomeArquivo);
    if (estrada == NULL) {
        return -1.0;
    }

    double menorVizinhanca = estrada->T; // Inicializa com o comprimento total da estrada

    for (int i = 0; i < estrada->N - 1; i++) {
        // Calcula a vizinhança entre duas cidades consecutivas
        double vizinhanca = (estrada->C[i + 1].Posicao - estrada->C[i].Posicao) / 2.0;

        // Atualiza a menor vizinhança
        if (vizinhanca < menorVizinhanca) {
            menorVizinhanca = vizinhanca;
        }
    }

    free(estrada->C);
    free(estrada);

    return menorVizinhanca;
}

// Função para encontrar a cidade com a menor vizinhança
char *cidadeMenorVizinhanca(const char *nomeArquivo) {
    Estrada *estrada = getEstrada(nomeArquivo);
    if (estrada == NULL) {
        return NULL;
    }

    double menorVizinhanca = estrada->T; // Inicializa com o comprimento total da estrada
    char *cidadeMenor = estrada->C[0].Nome;

    for (int i = 0; i < estrada->N - 1; i++) {
        // Calcula a vizinhança entre duas cidades consecutivas
        double vizinhanca = (estrada->C[i + 1].Posicao - estrada->C[i].Posicao) / 2.0;

        // Atualiza a menor vizinhança e a cidade correspondente
        if (vizinhanca < menorVizinhanca) {
            menorVizinhanca = vizinhanca;
            cidadeMenor = estrada->C[i].Nome;
        }
    }

    // Aloca espaço para o nome da cidade e copia o nome corretamente
    char *cidadeMenorCopia = malloc(strlen(cidadeMenor) + 1);
    if (cidadeMenorCopia != NULL) {
        strcpy(cidadeMenorCopia, cidadeMenor);
    } else {
        // Em caso de falha na alocação, retorna NULL
        printf("Erro de alocacao de memoria.\n");
        free(estrada->C);
        free(estrada);
        return NULL;
    }

    free(estrada->C);
    free(estrada);

    return cidadeMenorCopia;
}
