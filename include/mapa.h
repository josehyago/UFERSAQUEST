// mapa.h
#ifndef MAPA_H
#define MAPA_H

#include "raylib.h"
#include "tipos.h"

// Definimos de cada bloco (Tile) em pixels.
#define MAPA_TILE_SIZE 100 // Cada bloco terá 100x100 pixels na tela

// Valores que cada célula da matriz do mapa (arquivos mapaN.txt) pode assumir.
// Os números continuam iguais aos dos arquivos .txt; o enum só dá nome a eles.
typedef enum {
    TILE_VOLTA = -1,         // porta de volta: leva para a fase anterior
    TILE_CHAO = 0,           // chão: o jogador anda livremente
    TILE_PAREDE = 1,         // parede: bloqueia o jogador
    TILE_PORTA = 2,          // porta aberta: leva para a próxima fase
    TILE_PORTA_TRANCADA = 3  // porta trancada: bloqueia até o jogador usar a chave
} TipoTile;

// Funções do mapa
int **inicializarMapa(const char *MapaTxt, int *linhas, int *colunas);
void liberarMapa(int **vetor_mapa, int linhas);
void desenharMapa(int **vetor_mapa, int linhas, int colunas);
bool checarColisaoComMapa(int **vetor_mapa, int linhas, int colunas, Rectangle playerRec);
int **checarTransicaoDeFase(int **vetor_mapa, int *linhas, int *colunas, Jogador *j, int *faseAtual);
void registrarPortaAberta(int fase);

#endif