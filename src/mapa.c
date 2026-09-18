#include <stdio.h>
#include "raylib.h"
// ---------------------------------------------------------
// RESPONSÁVEL PELO MAPA: [Ryon]
// ---------------------------------------------------------
// O que adicionar aqui:
// 1. Criar as variáveis ou #define para LINHAS e COLUNAS da matriz.
// 2. Declarar a Matriz bidimensional que vai representar o mapa do portão da UFERSA.
//    (Ex: 0 = chão livre, 1 = parede/muro, 2 = catraca/porta).
// 3. Criar uma função void desenharMapa() que percorre a matriz usando dois 'for' (i, j).
// 4. Dentro da função, usar as funções da Raylib (ex: DrawRectangle) para pintar
//    quadrados na tela dependendo do número da matriz (se for 1, desenha cinza, etc.).