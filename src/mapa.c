#include <stdio.h>
#include "raylib.h"

// O que adicionar aqui:
// 1. Criar as variáveis ou #define para LINHAS e COLUNAS da matriz.
// 2. Declarar a Matriz bidimensional que vai representar o mapa do portão da UFERSA.
//    (Ex: 0 = chão livre, 1 = parede/muro, 2 = catraca/porta).
// 3. Criar uma função void desenharMapa() que percorre a matriz usando dois 'for' (i, j).
// 4. Dentro da função, usar as funções da Raylib (ex: DrawRectangle) para pintar
//    quadrados na tela dependendo do número da matriz (se for 1, desenha cinza, etc.).

#define LINHAS 10
#define COLUNAS 10
#define TILE_SIZE 40 // Cada quadrado da matriz terá 40x40 pixels na tela

void desenharMapa(int mapa[LINHAS][COLUNAS]){
    for (int i = 0; i < LINHAS; i++){
        for(int j = 0; j < COLUNAS; j++){
            int posX = j * TILE_SIZE; // Posição em pixels no eixo X
            int posY = i * TILE_SIZE; // Posição em pixels no eixo Y
            if(mapa[i][j] == 1){
                DrawRectangle(posX, posY, TILE_SIZE, TILE_SIZE, DARKGRAY); // Desenha parede/muro cinza
            }else if(mapa[i][j] == 0){
                    DrawRectangle(posX, posY, TILE_SIZE, TILE_SIZE, LIGHTGRAY); // Chão
                }
                // Desenha a grade do mapa
                DrawRectangleLines(posX, posY, TILE_SIZE, TILE_SIZE, GRAY);
        }    
    }
}
    
int main(){
    // Calcula o tamanho da janela com base no tamanho da matriz
    const int larguraTela = COLUNAS * TILE_SIZE; // 600px
    const int alturaTela = LINHAS * TILE_SIZE;   // 400px

    InitWindow(larguraTela, alturaTela, "UFERSAQUEST - Protótipo Unidade 1");
    SetTargetFPS(60);

    int mapa[LINHAS][COLUNAS] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
    };

    int playerX = 1;
    int playerY = 1;

    while (!WindowShouldClose()) {
        // --- 1. ENTRADAS E COLISÃO COM A MATRIZ ---
        if (IsKeyPressed(KEY_RIGHT) && mapa[playerY][playerX + 1] != 1) playerX++;
        if (IsKeyPressed(KEY_LEFT)  && mapa[playerY][playerX - 1] != 1) playerX--;
        if (IsKeyPressed(KEY_DOWN)  && mapa[playerY + 1][playerX] != 1) playerY++;
        if (IsKeyPressed(KEY_UP)    && mapa[playerY - 1][playerX] != 1) playerY--;

        // --- 2. DESENHO DOS GRÁFICOS ---
        BeginDrawing();
        ClearBackground(RAYWHITE);
        desenharMapa(mapa);

    DrawRectangle(playerX * TILE_SIZE + 5, playerY * TILE_SIZE + 5, TILE_SIZE - 10, TILE_SIZE - 10, BLUE);

        EndDrawing();
    }

    CloseWindow();
        return 0;
}