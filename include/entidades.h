// entidades.h
#ifndef ENTIDADES_H
#define ENTIDADES_H

#include "raylib.h"
#include "inventario.h"
#include "tipos.h"

// Cria as características do jogador
struct Jogador{
    char nome[20];
    int hp; // pontos de vida
    int score; // pontuação atual do jogador
    Vector2 pos; // posição (x,y) do jogador na tela
    float hitbox; 
    float tamanho; // dimensão física do jogador (largura e altura do quadrado de colisão)
    float velocidade; // quantidade de pixels que o jogador se move por frame
    Item inventario[10]; // Mochila com 10 espaços máximos.
    int qtdItens; // Contador essencial para saber quantos espaços estão ocupados.

    // Variáveis para a animação da spritesheet
    Texture2D textura; 
    int frameAtual;       // Quadro atual (0 a 3)
    float tempoAnimacao;  // Controla a velocidade da animação
    int linhaAnimacao;    // 0 = Frente, 1 = Costas, 2 = Lados
    bool espelharSprite; // Controla o espelhamento horizontal
};

// Cria as características do inimigo
typedef struct{
    char nome[20];
    int hp;
    int score;
    Vector2 pos;
    float tamanho;
    bool ativo; // flag booleana: true se o inimigo estiver vivo, false se for derrotado
} Inimigo;

// Funções de entidades
void inicializarJogador(Jogador *j, Vector2 posInicial);
void inicializarInimigo(Inimigo *i, Vector2 posInicial, const char *nome);
void moverJogador(Jogador *j, int **mapa, int linhas, int colunas);
Rectangle obterHitbox(const Jogador *j);

void desenharJogador(Jogador *j);

#endif