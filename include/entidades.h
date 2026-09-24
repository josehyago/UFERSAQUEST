#ifndef ENTIDADES_H
#define ENTIDADES_H

#include "mapa.h"
#include <stdio.h>
#include <string.h>

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

// Funções
void inicializarJogador(Jogador *j, Vector2 posInicial);
void inicializarInimigo(Inimigo *i, Vector2 posInicial, const char *nome);
void moverJogador(Jogador *j, int mapa[MAPA_LINHAS][MAPA_COLUNAS]);

#endif