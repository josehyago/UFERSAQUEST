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
    float x;              
    float y;               
} Vector2; //Não é necessario

typedef struct{
    Vector2 pos;
} Posicao; //Não é necessario

typedef struct{
    char nome [10];
    int HP;
    int score;
    Vector2 pos;
} Jogador;

/* typedef struct{
    char nome[20];
    int hp;
    int score;
    Vector2 pos;
    float tamanho;
    float velocidade;
} Jogador;
*/

typedef struct{
    char nome [10];
    int HP;
    int score;
    Vector2 pos;
} Inimigo; 

/*typedef struct{
    char nome[20];
    int hp;
    int score;
    Vector2 pos;
    float tamanho;
    bool ativo; // Define se o inimigo ainda está vivo no mapa
} Inimigo;
 */

/*Faltou: 
void inicializarJogador(Jogador *j, Vector2 posInicial);
void inicializarInimigo(Inimigo *i, Vector2 posInicial, const char *nome);
*/

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

/*void inicializarJogador(Jogador *j, Vector2 posInicial){
    strcpy(j->nome, "Aluno");
    j->hp = 100;
    j->score = 0;
    j->pos = posInicial;
    j->tamanho = 30.0f;
    j->velocidade = 4.0f;
}

void inicializarInimigo(Inimigo *i, Vector2 posInicial, const char *nome){
    strcpy(i->nome, nome);
    i->hp = 60;
    i->score = 0;
    i->pos = posInicial;
    i->tamanho = 30.0f;
    i->ativo = true;
}

* Essa função de mover jogador precisava das funções que eu fiz no mapa
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
    
*/

// Isso aqui de baixo apaga tudo.
int main (){

    // outras funções aqui:



While (!WindowShouldClose()){
     
    moverJogador (&jogador, matrizMapa); // Chamar o mover jogador 
       
    }

}
