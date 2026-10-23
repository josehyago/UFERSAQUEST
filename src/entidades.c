// Entidades.c
#include <string.h>
#include <stdio.h>
#include "entidades.h"

// Inicializar o jogador no jogo
void inicializarJogador (Jogador *j, Vector2 posInicial){
    snprintf(j->nome, sizeof(j->nome), "%s", "Ana Paula"); // Copia o texto "Ana Paula" para o atributo nome do jogador
    j->hp = 100; // vida inicial
    j->score = 0;
    j->pos = posInicial; // define a posição inicial passada como parâmetro
    j->hitbox = 56.0f;
    j->tamanho = 80.0f; // define a largura e altura
    j->velocidade= 4.0f; // pixels por frame
    j->qtdItens = 0; // Mochila começa vazia
    j->frameAtual = 0;
    j->tempoAnimacao = 0.0f;
    j->linhaAnimacao = 0;
    j->espelharSprite = false;
}

// Inicializar o inimigo no jogo
void inicializarInimigo(Inimigo *i, Vector2 posInicial, const char *nome){
    snprintf(i->nome, sizeof(i->nome), "%s", nome); // Copia o nome do inimigo passado como parâmetro
    i->hp = 60; // vida do inimigo
    i->pos = posInicial; // posição inicial do inimigo 
    i->tamanho = 60.0f; // dimensão do inimigo
    i->ativo = true; // inicializa vivo
}

// Colocar o jogador para se mover com tratamento de colisão em dois eixos (AABB)
void moverJogador(Jogador *j, int **vetor_mapa, int linhas, int colunas){
    float dx = 0, dy = 0;
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) dx += 1;
    if (IsKeyDown(KEY_LEFT)  || IsKeyDown(KEY_A)) dx -= 1;
    if (IsKeyDown(KEY_DOWN)  || IsKeyDown(KEY_S)) dy += 1;
    if (IsKeyDown(KEY_UP)    || IsKeyDown(KEY_W)) dy -= 1;

    bool movendo = (dx != 0 || dy != 0);
    if (dx != 0 && dy != 0){ dx *= 0.7071f; dy *= 0.7071f; } // diagonal na mesma velocidade

    // Eixo X
    float xAnterior = j->pos.x;
    j->pos.x += dx * j->velocidade;
    if (checarColisaoComMapa(vetor_mapa, linhas, colunas, obterHitbox(j))) j->pos.x = xAnterior;

    // Eixo Y
    float yAnterior = j->pos.y;
    j->pos.y += dy * j->velocidade;
    if (checarColisaoComMapa(vetor_mapa, linhas, colunas, obterHitbox(j))) j->pos.y = yAnterior;

    // Direção da animação
    if (dx != 0){ j->linhaAnimacao = 2; j->espelharSprite = (dx > 0); }
    if (dy > 0) j->linhaAnimacao = 0;
    if (dy < 0) j->linhaAnimacao = 1;

    if (movendo){
        j->tempoAnimacao += GetFrameTime();
        if (j->tempoAnimacao >= 0.15f){
            j->tempoAnimacao = 0.0f;
            j->frameAtual = (j->frameAtual + 1) % 4;
        }
    } else {
        j->frameAtual = 0;
        j->tempoAnimacao = 0.0f;
    }
}

// Retângulo de colisão: menor que o sprite, centralizado e colado nos "pés"
Rectangle obterHitbox(const Jogador *j){
    return (Rectangle){
        j->pos.x + (j->tamanho - j->hitbox) / 2.0f,
        j->pos.y + (j->tamanho - j->hitbox),
        j->hitbox,
        j->hitbox
    };
}

// Função para desenhar a caloura cortando a grade 4x3 corretamente.
void desenharJogador(Jogador *j){
    // Como a caloura.png tem 4 colunas e 3 linhas:
    float larguraQuadro = (float)j->textura.width / 4.0f;
    float alturaQuadro = (float)j->textura.height / 3.0f;

    // Se estiver olhando para a esquerda, inverta a largura do recorte para espelhar o gráfico
    float larguraRecorte = j->espelharSprite ? -larguraQuadro : larguraQuadro;

    Rectangle fonte = { 
        j->frameAtual * larguraQuadro, 
        j->linhaAnimacao * alturaQuadro, 
        larguraRecorte, 
        alturaQuadro 
    };

    Rectangle destino = { j->pos.x, j->pos.y, j->tamanho, j->tamanho };
    Vector2 origem = { 0.0f, 0.0f };

    DrawTexturePro(j->textura, fonte, destino, origem, 0.0f, WHITE);
}