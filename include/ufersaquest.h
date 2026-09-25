// ufersaquest.h
#ifndef UFERSAQUEST_H
#define UFERSAQUEST_H

#include "raylib.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Definimos o tamanho da nossa grade (matriz) e o tamanho de cada bloco (Tile) em pixels.
#define MAPA_LINHAS 25
#define MAPA_COLUNAS 35
#define MAPA_TILE_SIZE 40 // Cada bloco terá 40x40 pixels na tela

// Cria as características do jogador
typedef struct{
    char nome[20];
    int hp; // pontos de vida
    int score; // pontuação atual do jogador
    Vector2 pos; // posição (x,y) do jogador na tela 
    float tamanho; // dimensão física do jogador (largura e altura do quadrado de colisão)
    float velocidade; // quantidade de pixels que o jogador se move por frame
} Jogador;

// Cria as características do inimigo
typedef struct{
    char nome[20];
    int hp;
    int score;
    Vector2 pos;
    float tamanho;
    bool ativo; // flag booleana: true se o inimigo estiver vivo, false se for derrotado
} Inimigo;

// Struct Pergunta com texto, 4 opcoes, resposta correta e controle de opções ocultas
typedef struct{
    char texto[200];
    char opcoes[4][100];
    int respostaCorreta; // Índice (0 a 3)
    bool opcaoOculta[4]; // Controla quais opções sumiram após errar
} Pergunta;

// Estados possíveis do jogo
typedef enum { ESTADO_EXPLORACAO, ESTADO_COMBATE, ESTADO_GAMEOVER } EstadoJogo;

// Funções do mapa
void inicializarMapa(int mapa[MAPA_LINHAS][MAPA_COLUNAS]);
void desenharMapa(int mapa[MAPA_LINHAS][MAPA_COLUNAS]);
bool checarColisaoComMapa(int mapa[MAPA_LINHAS][MAPA_COLUNAS], Rectangle playerRec);

// Funções de Câmera
Camera2D criarCamera(int larguraTela, int alturaTela);
void atualizarCamera(Camera2D *camera, Vector2 playerPos, float playerSize, int larguraTela, int alturaTela);

// Funções de entidades
void inicializarJogador(Jogador *j, Vector2 posInicial);
void inicializarInimigo(Inimigo *i, Vector2 posInicial, const char *nome);
void moverJogador(Jogador *j, int mapa[MAPA_LINHAS][MAPA_COLUNAS]);

// Funções de combate
Pergunta* criarBancoPerguntas(int *qtdPerguntas);
void liberarBancoPerguntas(Pergunta *banco);
bool resolverTurno(Jogador *j, Inimigo *i, Pergunta *p, int escolha, char *mensagem, Color *corMensagem, bool *mudarPergunta);
void desenharInterfaceCombate(Pergunta p, Jogador j, Inimigo i);

#endif