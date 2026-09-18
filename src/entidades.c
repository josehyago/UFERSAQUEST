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