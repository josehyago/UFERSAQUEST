#ifndef COMBATE_H
#define COMBATE_H

#include "entidades.h"
#include <stdlib.h>

// Struct Pergunta com texto, 4 opcoes, resposta correta e controle de opções ocultas
typedef struct{
    char texto[200];
    char opcoes[4][100];
    int respostaCorreta; // Índice (0 a 3)
    bool opcaoOculta[4]; // Controla quais opções sumiram após errar
} Pergunta;

Pergunta* criarBancoPerguntas(int *qtdPerguntas);
void liberarBancoPerguntas(Pergunta *banco);
bool resolverTurno(Jogador *j, Inimigo *i, Pergunta *p, int escolha, char *mensagem, Color *corMensagem, bool *mudarPergunta);
void desenharInterfaceCombate(Pergunta p, Jogador j, Inimigo i);

#endif