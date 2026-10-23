#ifndef UI_H
#define UI_H
#include "raylib.h"

void carregarFonteUI(const char *caminho);
void descarregarFonteUI(void);
void desenharTexto(const char *texto, int x, int y, int tamanho, Color cor);
int medirTexto(const char *texto, int tamanho);

#endif