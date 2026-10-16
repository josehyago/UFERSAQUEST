// mapa.h
#ifndef MAPA_H
#define MAPA_H

#include "raylib.h"

typedef struct Jogador Jogador;

// Definimos de cada bloco (Tile) em pixels.
#define MAPA_TILE_SIZE 40 // Cada bloco terá 40x40 pixels na tela

// Funções do mapa
int **inicializarMapa(const char *MapaTxt, int *linhas, int *colunas);
void liberarMapa(int **vetor_mapa, int linhas);
void desenharMapa(int **vetor_mapa, int linhas, int colunas);
bool checarColisaoComMapa(int **vetor_mapa, int linhas, int colunas, Rectangle playerRec);
int **checarTransicaoDeFase(int **vetor_mapa, int *linhas, int *colunas, Jogador *j, int *faseAtual);

#endif