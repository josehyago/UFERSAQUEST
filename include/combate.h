#ifndef COMBATE_H
#define COMBATE_H

#include "entidades.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct{
    char texto[200];
    char opcoes[4][100];
    int respostaCorreta; // Índice (0 a 3)
} Pergunta;

Pergunta* criarBancoPerguntas(int *qtdPerguntas);
void liberarBancoPerguntas(Pergunta *banco);
bool resolverTurno(Jogador *j, Inimigo *i, Pergunta p, int escolha);
void desenharInterfaceCombate(Pergunta p, Jogador j, Inimigo i);

#endif