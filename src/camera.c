// Camera.h
#include "camera.h"

// FUNÇÃO: criarCamera
// OBJETIVO: Configurar as propriedades iniciais da câmera 2D.
Camera2D criarCamera(int larguraTela, int alturaTela){
    Camera2D camera = { 0 };

    // O offset define onde o alvo da câmera vai ficar desenhado na sua tela.
    // Colocando metade da largura e altura, garantimos que o jogador fique sempre no centro da janela.
    camera.offset = (Vector2){ larguraTela / 2.0f, alturaTela / 2.0f };
    camera.zoom = 1.0f; // Zoom normal (100%)
    return camera;
}

// FUNÇÃO: atualizarCamera
// OBJETIVO: Fazer a câmera seguir o jogador, mas sem mostrar o vazio fora do mapa.
void atualizarCamera(Camera2D *camera, Vector2 playerPos, float playerSize, int larguraTela, int alturaTela, int linhas, int colunas){

    // Define que a câmera deve mirar exatamente no centro do quadrado do jogador
    float alvoX = playerPos.x + (playerSize / 2);
    float alvoY = playerPos.y + (playerSize / 2);

    // Calcula os limites máximos que a câmera pode ir sem revelar o que está fora da matriz.
    // Ex: Ela não pode ir mais para a esquerda do que a metade da sua tela.
    float minX = larguraTela / 2.0f;
    float maxX = (colunas * MAPA_TILE_SIZE) - (larguraTela / 2.0f);
    float minY = alturaTela / 2.0f;
    float maxY = (linhas * MAPA_TILE_SIZE) - (alturaTela / 2.0f);

    if (alvoX < minX) alvoX = minX;
    if (alvoX > maxX) alvoX = maxX;
    if (alvoY < minY) alvoY = minY;
    if (alvoY > maxY) alvoY = maxY;

    // Atualiza o alvo final da câmera após as correções.
    camera->target = (Vector2){ alvoX, alvoY };
}