// entidades.h
#ifndef ENTIDADES_H
#define ENTIDADES_H

#include "raylib.h"
#include "inventario.h"
#include "tipos.h"

#define HP_MAXIMO 100 // vida máxima (e inicial) do jogador

// Cria as características do jogador
typedef struct Jogador{
    char nome[20];
    int hp; // pontos de vida
    int score; // pontuação atual do jogador
    Vector2 pos; // posição (x,y) do canto superior esquerdo do SPRITE
    float tamanho; // lado do quadrado do sprite desenhado na tela (80px)
    float hitbox; // lado do quadrado de colisão (56px, menor que o sprite)
    float velocidade; // quantidade de pixels que o jogador se move por frame
    Item inventario[MAX_MOCHILA]; // Mochila com MAX_MOCHILA espaços máximos.
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
    Vector2 pos;
    float tamanho;
    bool ativo; // flag booleana: true se o inimigo estiver vivo, false se for derrotado
} Inimigo;

// Funções de entidades
void inicializarJogador(Jogador *j, Vector2 posInicial);
void inicializarInimigo(Inimigo *i, Vector2 posInicial, const char *nome);
void moverJogador(Jogador *j, int **mapa, int linhas, int colunas);
Rectangle obterHitbox(const Jogador *j);
bool jogadorTocaInimigo(const Jogador *j, const Inimigo *i);
void desenharJogador(const Jogador *j);
void desenharInimigo(const Inimigo *i);

#endif