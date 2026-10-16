// ufersaquest.h
#ifndef UFERSAQUEST_H
#define UFERSAQUEST_H

#include "raylib.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

// Definimos de cada bloco (Tile) em pixels.
#define MAPA_TILE_SIZE 40 // Cada bloco terá 40x40 pixels na tela

// Cria as características do jogador
typedef struct{
    char nome[20];
    int hp; // pontos de vida
    int score; // pontuação atual do jogador
    Vector2 pos; // posição (x,y) do jogador na tela 
    float tamanho; // dimensão física do jogador (largura e altura do quadrado de colisão)
    float velocidade; // quantidade de pixels que o jogador se move por frame
    Texture2D textura; // para o desenho do jogador
    float frame; // para animação do jogador
    int contador; // contador de tempo para animação
    bool estaAndando; // flag para saber se o jogador está andando ou parado
    float direcaoOlhar; // direção que o jogador está olhando (1.0f para direita, -1.0f para esquerda)
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
int **inicializarMapa(const char *MapaTxt, int *linhas, int *colunas);
void liberarMapa(int **vetor_mapa, int linhas);
void desenharMapa(int **vetor_mapa, int linhas, int colunas);
bool checarColisaoComMapa(int **vetor_mapa, int linhas, int colunas, Rectangle playerRec);
int **checarTransicaoDeFase(int **vetor_mapa, int *linhas, int *colunas, Jogador *j, int *faseAtual);

// Funções de Câmera
Camera2D criarCamera(int larguraTela, int alturaTela);
void atualizarCamera(Camera2D *camera, Vector2 playerPos, float playerSize, int larguraTela, int alturaTela, int linhas, int colunas);

// Funções de entidades

void inicializarInimigo(Inimigo *i, Vector2 posInicial, const char *nome);


// Funções de combate
Pergunta* criarBancoPerguntas(int *qtdPerguntas);
void liberarBancoPerguntas(Pergunta *banco);
bool resolverTurno(Jogador *j, Inimigo *i, Pergunta *p, int escolha, char *mensagem, Color *corMensagem, bool *mudarPergunta);
void desenharInterfaceCombate(Pergunta p, Jogador j, Inimigo i);

//Funções do jogador
void inicializarJogador(Jogador *j, Vector2 posInicial);
void moverJogador(Jogador *j, int **mapa, int linhas, int colunas);
void desenharJogador(Jogador *jogador, int *contador, int *frame);


#endif