#ifndef ENTIDADES_H
#define ENTIDADES_H

#include "raylib.h"
#include "mapa.h"

typedef struct {
    char nome[20];
    int hp;
    int score;
    Vector2 pos;
    float tamanho;
    float velocidade;
} Jogador;

typedef struct {
    char nome[20];
    int hp;
    int score;
    Vector2 pos;
    float tamanho;
    bool ativo; // Define se o inimigo ainda está vivo no mapa
} Inimigo;

// Funções
void inicializarJogador(Jogador *j, Vector2 posInicial);
void inicializarInimigo(Inimigo *i, Vector2 posInicial, const char *nome);
void moverJogador(Jogador *j, int mapa[MAPA_LINHAS][MAPA_COLUNAS]);

#endif