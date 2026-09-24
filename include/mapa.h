// mapa.h
#ifndef MAPA_H
#define MAPA_H

#include "raylib.h"

// Definimos o tamanho da nossa grade (matriz) e o tamanho de cada bloco (Tile) em pixels.
#define MAPA_LINHAS 25
#define MAPA_COLUNAS 35
#define MAPA_TILE_SIZE 40 // Cada bloco terá 40x40 pixels na tela

// Funções do mapa
void inicializarMapa(int mapa[MAPA_LINHAS][MAPA_COLUNAS]);
void desenharMapa(int mapa[MAPA_LINHAS][MAPA_COLUNAS]);
bool checarColisaoComMapa(int mapa[MAPA_LINHAS][MAPA_COLUNAS], Rectangle playerRec);

// Funções de Câmera
Camera2D criarCamera(int larguraTela, int alturaTela);
void atualizarCamera(Camera2D *camera, Vector2 playerPos, float playerSize, int larguraTela, int alturaTela);

#endif