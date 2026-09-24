#include "entidades.h"
#include "raylib.h"
#include <stdio.h>
#include <string.h>
// ---------------------------------------------------------
// RESPONSÁVEL PELAS ENTIDADES (PLAYER E INIMIGOS): [Ana Paula]
// ---------------------------------------------------------
// O que adicionar aqui:
// 1. Criar a 'struct Posicao' com 'int x' e 'int y'. //Isso aqui não é necessário
// 2. Criar a 'struct Jogador' contendo Nome (string), HP, Score e a 'struct Posicao' aninhada.
// 3. Criar a 'struct Inimigo' contendo as mesmas coisas.
// 4. Criar a função void moverJogador(Jogador *j, int matrizMapa[][]).
// 5. Na função mover, ler as teclas WASD/Setas da Raylib (IsKeyPressed).
// 6. Antes de alterar a posição x/y do jogador, verificar o IF na matriz do mapa 
//    para impedir que ele ande por cima do número 1 (paredes).



typedef struct{
    char nome [10];
    int hp;
    int score;
    Vector2 pos;
    float tamanho;
    float velocidade;
} Jogador;



typedef struct{
    char nome [20];
    int hp;
    float tamanho;
    Vector2 pos;
    bool ativo; // define se está vivo ou morto
} Inimigo; 

void inicializarJogador (Jogador *j, Vector2 posInicial){
    strcpy (j-> nome, "Florzinha");
    j-> hp=100;
    j -> score=0;
    j -> tamanho = 30.0f;
    j -> pos= posInicial;
    j -> velocidade= 4.0f;
}

void inicializarInimigo(Inimigo *i, Vector2 posInicial, const char *nome){
    strcpy ( i -> nome, "Bug de C");
    i -> hp=100;
    i -> tamanho = 30.0f;
    i -> pos = posInicial;
    i -> ativo = true;
}

void moverJogador(Jogador *j, int mapa[MAPA_LINHAS][MAPA_COLUNAS]){
    float posXAnterior = j->pos.x;

    // Eixo X
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) j->pos.x += j->velocidade;
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))  j->pos.x -= j->velocidade;

    Rectangle recX = { j->pos.x, j->pos.y, j->tamanho, j->tamanho };
    if (checarColisaoComMapa(mapa, recX)) j->pos.x = posXAnterior;

    float posYAnterior = j->pos.y;

    // Eixo Y
    if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) j->pos.y += j->velocidade;
    if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W))   j->pos.y -= j->velocidade;

    Rectangle recY = { j->pos.x, j->pos.y, j->tamanho, j->tamanho };
    if (checarColisaoComMapa(mapa, recY)) j->pos.y = posYAnterior;
}