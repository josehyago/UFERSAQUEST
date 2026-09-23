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

void moverJogador ( Jogador *jogador, int matrizMapa [LINHA][COLUNA]){
    int linhaAtual= jogador -> pos.y; // posição do jogador no eixo y
    int colunaAtual =  jogador -> pos.x; // posição do jogador no eixo x;

    int novaLinha= linhaAtual; // Atualização do antiga posição. A novaLinha e novaColuna são variáveis temporáveis
    int novaColuna= colunaAtual;
        
    if (IsKeyPressed (KEY_S)) novaLinha ++; // Teclas sendo pressionadas
    if (IsKeyPressed (KEY_A)) novaColuna --;
    if (IsKeyPressed (KEY_W)) novaLinha --;
    if (IsKeyPressed (KEY_D)) novaColuna ++;

    // Irá verificar se o mapa não encontrou uma parede. Se não,
    // irá dá prosseguimento ao movimento do jogador. 

    if (matrizMapa [novaLinha][novaColuna]!=1){
        jogador -> pos.y= novaLinha;
        jogador -> pos.x = novaColuna;
          }


    }

   
    
    

int main (){

    // outras funções aqui:



While (!WindowShouldClose()){
     
    moverJogador (&jogador, matrizMapa); // Chamar o mover jogador 
       
    }

}
