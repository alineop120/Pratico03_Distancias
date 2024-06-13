#include <stdio.h>
#include <stdlib.h>
#include "cidades.h"

int main() {
    const char *nomeArquivo = "teste02.txt";

    double menorVizinhanca = calcularMenorVizinhanca(nomeArquivo);
    if (menorVizinhanca != -1.0) {
        printf("Menor vizinhanca: %.2f\n", menorVizinhanca);
    } else {
        printf("Erro ao calcular a menor vizinhanca.\n");
    }

    char *cidadeMenor = cidadeMenorVizinhanca(nomeArquivo);
    if (cidadeMenor != NULL) {
        printf("Cidade com menor vizinhanca: %s\n", cidadeMenor);
        free(cidadeMenor);
    } else {
        printf("Erro ao encontrar a cidade com menor vizinhanca.\n");
    }

    return 0;
}