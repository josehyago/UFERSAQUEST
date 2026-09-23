#include "raylib.h"
#include <stdio.h>
#include <string.h>
// ---------------------------------------------------------
// RESPONSÁVEL PELAS ENTIDADES (PLAYER E INIMIGOS): [Ana Paula]
// ---------------------------------------------------------
// O que adicionar aqui:
// 1. Criar a 'struct Posicao' com 'int x' e 'int y'.
// 2. Criar a 'struct Jogador' contendo Nome (string), HP, Score e a 'struct Posicao' aninhada.
// 3. Criar a 'struct Inimigo' contendo as mesmas coisas.
// 4. Criar a função void moverJogador(Jogador *j, int matrizMapa[][]).
// 5. Na função mover, ler as teclas WASD/Setas da Raylib (IsKeyPressed).
// 6. Antes de alterar a posição x/y do jogador, verificar o IF na matriz do mapa 
//    para impedir que ele ande por cima do número 1 (paredes).
typedef struct{
    float x;              
    float y;               
} Vector2;

typedef struct{
    Vector2 pos;
} Posicao;

typedef struct{
    char nome [10];
    int HP;
    int score;
    Vector2 pos;
} Jogador;

typedef struct{
    char nome [10];
    int HP;
    int score;
    Vector2 pos;
} Inimigo; 

void moverJogador ( Jogador *jogador, int matrizMapa){

    int **matrizMapa= (Jogador*) malloc ( jogador * sizeof (Jogador*));
    if (matrizMapa=NULL) return NULL;
    
    Jogador jogadorx;
    Jogador jogadory;

    for (int i=0; i< jogadorx ; i++){
        matrizMapa [i]= (Jogador*) malloc (jodadory * sizeof (jogador));
           for (int j=0; j< jogadory; j++){
               

           }
       
    }

}
