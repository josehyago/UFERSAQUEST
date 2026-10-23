// Camera.h
#include "camera.h"

// FUNÇÃO: limitarValor (uso interno deste arquivo)
// PARÂMETROS: valor - número a limitar; minimo e maximo - os limites.
// POR QUE: o mesmo "não deixar passar do limite" é usado nos dois eixos da câmera.
// PARA QUE SERVE: RETORNA o valor, ou o limite mais próximo se ele estiver fora do intervalo.
static float limitarValor(float valor, float minimo, float maximo){
    if (valor < minimo) return minimo;
    if (valor > maximo) return maximo;
    return valor;
}

// FUNÇÃO: criarCamera
// PARÂMETROS: larguraTela e alturaTela - tamanho da janela em pixels.
// POR QUE: a Camera2D da Raylib precisa de uma configuração inicial.
// PARA QUE SERVE: RETORNA uma câmera 2D com o alvo centralizado na tela e zoom normal.
Camera2D criarCamera(int larguraTela, int alturaTela){
    Camera2D camera = { 0 };

    // O offset define em qual ponto da TELA o alvo da câmera aparece.
    // Colocando metade da largura e da altura, o jogador fica sempre no centro da janela.
    camera.offset = (Vector2){ larguraTela / 2.0f, alturaTela / 2.0f };
    camera.zoom = 1.0f; // Zoom normal (100%)
    return camera;
}

// FUNÇÃO: atualizarCamera
// PARÂMETROS: camera - a câmera a ser movida; playerPos e playerSize - posição e tamanho do sprite do jogador; larguraTela e alturaTela - tamanho da janela; linhas e colunas - tamanho do mapa.
// POR QUE: a câmera precisa seguir o jogador sem mostrar o "vazio" fora do mapa.
// PARA QUE SERVE: aponta a câmera para o centro do jogador, limitada às bordas do mapa. Se o mapa for menor que a tela em algum eixo, centraliza o mapa nesse eixo (evita a câmera tremer).
void atualizarCamera(Camera2D *camera, Vector2 playerPos, float playerSize, int larguraTela, int alturaTela, int linhas, int colunas){

    // Define que a câmera deve mirar exatamente no centro do quadrado do jogador
    float alvoX = playerPos.x + (playerSize / 2);
    float alvoY = playerPos.y + (playerSize / 2);

    // Tamanho total do mapa em pixels.
    float larguraMapa = colunas * MAPA_TILE_SIZE;
    float alturaMapa  = linhas  * MAPA_TILE_SIZE;

    // Eixo X: mapa menor que a tela -> centraliza; senão, limita entre metade da tela e (mapa - metade da tela).
    if (larguraMapa <= larguraTela) alvoX = larguraMapa / 2.0f;
    else alvoX = limitarValor(alvoX, larguraTela / 2.0f, larguraMapa - larguraTela / 2.0f);

    // Eixo Y: mesma lógica do eixo X.
    if (alturaMapa <= alturaTela) alvoY = alturaMapa / 2.0f;
    else alvoY = limitarValor(alvoY, alturaTela / 2.0f, alturaMapa - alturaTela / 2.0f);

    // Atualiza o alvo final da câmera após as correções.
    camera->target = (Vector2){ alvoX, alvoY };
}
