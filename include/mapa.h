// mapa.h
#ifndef MAPA_H
#define MAPA_H

#include "raylib.h"
#include "tipos.h"

// Definimos de cada bloco (Tile) em pixels.
#define MAPA_TILE_SIZE 100 // Cada bloco terá 100x100 pixels na tela

// Funções do mapa
int **inicializarMapa(const char *MapaTxt, int *linhas, int *colunas);
void liberarMapa(int **vetor_mapa, int linhas);
void desenharMapa(int **vetor_mapa, int linhas, int colunas);
bool checarColisaoComMapa(int **vetor_mapa, int linhas, int colunas, Rectangle playerRec);
int **checarTransicaoDeFase(int **vetor_mapa, int *linhas, int *colunas, Jogador *j, int *faseAtual);
void registrarPortaAberta(int fase);
static void aplicarPortasAbertas(int **mapa, int linhas, int colunas, int fase);

#endif