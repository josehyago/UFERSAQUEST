// combate.h
#ifndef COMBATE_H
#define COMBATE_H

#include "raylib.h"
#include "entidades.h"

#define DANO_POR_TURNO 20    // HP perdido por quem perde a rodada (jogador ao errar, inimigo ao ser acertado)
#define TEMPO_MENSAGEM 2.0f  // segundos que o resultado da resposta fica na tela

// Struct Pergunta com texto, 4 opcoes, resposta correta e controle de opções ocultas
typedef struct{
    char texto[200];
    char opcoes[4][100];
    int respostaCorreta; // Índice (0 a 3)
    bool opcaoOculta[4]; // Controla quais opções sumiram após errar
} Pergunta;

// Struct Estado Combate, agrupa tudo que o combate precisa guardar entre um quadro e outro
typedef struct{
    Pergunta *banco;      // vetor de perguntas alocado dinamicamente (malloc)
    int qtdPerguntas;     // quantas perguntas existem no banco
    int perguntaAtual;    // índice da pergunta que está na tela
    float tempoMensagem;  // segundos restantes mostrando o resultado (0 = esperando resposta)
    char mensagem[100];   // texto do resultado ("RESPOSTA CORRETA!...")
    Color corMensagem;    // cor do texto do resultado
    bool mudarPergunta;   // true = ao fim da mensagem, passa para a próxima pergunta
} EstadoCombate;

// O que aconteceu no quadro atual do combate
typedef enum { COMBATE_EM_ANDAMENTO, COMBATE_VITORIA, COMBATE_DERROTA } ResultadoCombate;

// Funções de combate
Pergunta* criarBancoPerguntas(int *qtdPerguntas);
void liberarBancoPerguntas(Pergunta *banco);
bool inicializarCombate(EstadoCombate *c);
void liberarCombate(EstadoCombate *c);
bool resolverTurno(Jogador *j, Inimigo *i, EstadoCombate *c, int escolha);
ResultadoCombate atualizarCombate(EstadoCombate *c, Jogador *j, Inimigo *i);
void desenharCombate(const EstadoCombate *c, const Jogador *j, const Inimigo *i);

#endif