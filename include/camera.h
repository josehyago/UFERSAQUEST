// camera.h
#ifndef CAMERA_H
#define CAMERA_H

#include "raylib.h"
#include "mapa.h"

// Funções de Câmera
Camera2D criarCamera(int larguraTela, int alturaTela);
void atualizarCamera(Camera2D *camera, Vector2 playerPos, float playerSize, int larguraTela, int alturaTela, int linhas, int colunas);

#endif