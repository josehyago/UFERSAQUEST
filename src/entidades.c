#include "entidades.h"
#include "raylib.h"
#include <stdio.h>
#include <string.h>



// Cria as características do jogador
typedef struct{
    char nome [10]; 
    int hp; // pontos de vida
    int score; // pontuação atual do jogador
    Vector2 pos; // posição (x,y) do jogador na tela 
    float tamanho; // dimensão física do jogador (largura e altura do quadrado de colisão)
    float velocidade; // quantidade de pixels que o jogador se move por frame
} Jogador;


// Cria as características do inimigo
typedef struct{
    char nome [20];
    int hp; 
    float tamanho;
    Vector2 pos;
    bool ativo; // flag booleana: true se o inimigo estiver vivo, false se for derrotado
} Inimigo; 

// Inicializar o jogador no jogo

void inicializarJogador (Jogador *j, Vector2 posInicial){
    strcpy (j-> nome, "Florzinha"); // Copia o texto "Florzinha" para o atributo nome do jogador
    j-> hp=100; // vida inicial
    j -> score=0;
    j -> tamanho = 30.0f; // define a largura e altura
    j -> pos= posInicial; // define a posição inicial passada como parâmetro
    j -> velocidade= 4.0f; // pixels por frame
}

// Inicializar o inimigo no jogo
void inicializarInimigo(Inimigo *i, Vector2 posInicial){
    strcpy ( i -> nome, "Bug de C");
    i -> hp=100; // vida do inimigo
    i -> tamanho = 30.0f; // dimensão do inimigo
    i -> pos = posInicial; // posição inicial do inimigo 
    i -> ativo = true; // inicializa vivo
}

// Colocar o jogador para se mover com tratamento de colisão em dois eixos (AABB)

void moverJogador(Jogador *j, int mapa[MAPA_LINHAS][MAPA_COLUNAS]){
    // EIXO X

    float posXAnterior = j->pos.x; // Guarda a posição X antes de mover

    // Verifica se as teclas da direita e a D estão posicionadas para mover para a direita
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) j->pos.x += j->velocidade;
    // Verifica se as teclas da esquerda e a A estão  pressionadas para mover para a esquerda
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))  j->pos.x -= j->velocidade;
    // Cria um retângulo de colisão com a nova posição do eixo X
    Rectangle recX = { j->pos.x, j->pos.y, j->tamanho, j->tamanho };
    // Se a nova posição colidir com algum obstáculo do mapa, restaura a posição X anterior
    if (checarColisaoComMapa(mapa, recX)) j->pos.x = posXAnterior;
    // EIXO Y
    float posYAnterior = j->pos.y; // Guarda a posição Y antes de mover

    // Verifica se as teclas 'S' ou 'Seta para Baixo' estão pressionadas para mover para baixo
    if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) j->pos.y += j->velocidade;
    // Verifica se as teclas 'W' ou 'Seta para Cima' estão pressionadas para mover para cima
    if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W))   j->pos.y -= j->velocidade;
    // Cria um retângulo de colisão com a nova posição no eixo Y (já validada no eixo X)
    Rectangle recY = { j->pos.x, j->pos.y, j->tamanho, j->tamanho };
    // Se a nova posição colidir com o mapa, restaura a posição Y anterior
    if (checarColisaoComMapa(mapa, recY)) j->pos.y = posYAnterior;
}