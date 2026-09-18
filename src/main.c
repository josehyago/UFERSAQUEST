#include <stdio.h>
#include "raylib.h"
// Aqui faremos os #include dos arquivos acima (mapa.h, entidades.h, combate.h)

// ---------------------------------------------------------
// TRABALHO CONJUNTO (Montagem do Jogo)
// ---------------------------------------------------------
// O que adicionar aqui:
// 1. Criar a Enumeração (enum) dos estados: TELA_MENU, TELA_EXPLORACAO, TELA_COMBATE.
// 2. Inicializar a janela da Raylib (InitWindow).
// 3. Instanciar o jogador e o inimigo (usando as structs feitas no entidades.c).
// 4. Criar o 'while (!WindowShouldClose())' (Game Loop).
// 5. Fazer o switch(estadoAtual) e chamar as funções de desenhar mapa, mover jogador
//    ou resolver combate dependendo da tela em que estamos.

int main(void)
{

    const int LARGURA_TELA = 800; 
    const int ALTURA_TELA = 450;
    // Para mudar o tamanho da tela é só mudar os valores aqui.

    InitWindow(LARGURA_TELA, ALTURA_TELA, "UFERSAQUEST");
    // InitWindow: inicializa a janela do programa, coloca-se a largura, altura e nome do programa.

    Vector2 POSICAO_BOLA = { (float)LARGURA_TELA/2, (float)ALTURA_TELA/2 };
    // Vector2: Estrutura do Raylib que guarda dois valores: x e y. Muito usado para posições 2D.
    // Usamos (float) para converter a divisão inteira em um número decimal, garantindo precisão na tela.
    // Ao dividirmos a largura e a altura da tela por 2, colocamos a bola exatamente no centro.

    float RAIO_BOLA = 50.0f;
    // Váriavel que guarda o raio da bola, para alterar o tamanho dela é só mudar o valor aqui. Para evitar ficar mudando lá no DrawCircleV.
    Color COR_BOLA = MAROON;
    // Color: Dá a cor.
    // Váriavel que guardar a cor da bola. Para evitar ficar mudando lá no DrawCircleV.
    float VELOCIDADE = 10.0f;
    // Váriavel que guarda a velocidade da bola. Para evitar ficar mudando no if(ISKeyDown()).

    SetTargetFPS(60);
    // SetTargetFPS: Define a quantidade de quadros por segundo (FPS) que o programa vai rodar.

    while (!WindowShouldClose()){
    // Loop principal que garante o funcionamento do programa.
    // ! = Diferente; 
    // WindowShouldClose: Checa se o usuário clicou no x ou ESC para que o programa feche.

        // Movimentação:

        // Note que os "if"s abaixo não usam chaves { }. 
        // Quando um "if" tem apenas uma instrução, as chaves são opcionais. 

        // IsKeyDown: Checa se a tecla especificada está sendo pressionada e mantida.
        // O eixo X cresce para a direita. O eixo Y cresce para BAIXO.
        if (IsKeyDown(KEY_RIGHT)) POSICAO_BOLA.x += VELOCIDADE; // Direita (Aumentamos porque o eixo x cresce pra direita)
        if (IsKeyDown(KEY_LEFT)) POSICAO_BOLA.x -= VELOCIDADE; // Esquerda (Diminuimos porque o eixo x cresce pra direita)
        if (IsKeyDown(KEY_UP)) POSICAO_BOLA.y -= VELOCIDADE; // Cima (Diminuimos porque o eixo y cresce pra baixo)
        if (IsKeyDown(KEY_DOWN)) POSICAO_BOLA.y += VELOCIDADE; // Baixo (Aumentamos porque o eixo y cresce pra baixo)

        // Sistema de Colisão (Limites da tela):

        // Se a posição da bola passar do limite, nós a forçamos a ficar no limite máximo permitido.
        // Detalhes: A posição X e Y da POSICAO_BOLA representa o centro dela;
        // Se usássemos apenas "LARGURA_TELA" ou "0" como limite, metade da bola sairia da tela antes de parar.
        // Para evitar isso, usamos o RAIO_BOLA em todos os lados.

        if (POSICAO_BOLA.x >= (LARGURA_TELA - RAIO_BOLA)) POSICAO_BOLA.x = LARGURA_TELA - RAIO_BOLA;
        // Barreira da Direita: Limite é a LARGURA total menos a metade da bola (o raio).
        // Se a bola tentar passar desse limite, nós a travamos nele.
        
        if (POSICAO_BOLA.x <= RAIO_BOLA) POSICAO_BOLA.x = RAIO_BOLA;
        // Barreira da Esquerda: Limite é o próprio tamanho do raio.
        // Impede que o X chegue a 0 ou fique negativo.
        
        if (POSICAO_BOLA.y >= (ALTURA_TELA - RAIO_BOLA)) POSICAO_BOLA.y = ALTURA_TELA - RAIO_BOLA;
        // Barreira de Baixo: Limite é a ALTURA total menos a metade da bola (o raio).
        // Se a bola tentar passar desse limite, nós a travamos nele.
        
        if (POSICAO_BOLA.y <= RAIO_BOLA) POSICAO_BOLA.y = RAIO_BOLA;
        // Barreira de Cima: Limite é o próprio tamanho do raio.
        // Impede que o Y chegue a 0 ou fique negativo.

        BeginDrawing();
        // BeginDrawing: Prepara a tela para desenhar o que for posto no código.

        ClearBackground(RAYWHITE);
        // ClearBackground: Define a cor do fundo.

        DrawText("Mova a bola com as seta", 10, 10, 20, DARKGRAY);
        // DrawText: Desenha um texto na tela, coloca-se o texto, posição de x, y, tamanho da fonte e cor.

        DrawCircleV(POSICAO_BOLA, RAIO_BOLA, COR_BOLA);
        // DrawCircleV: Desenha um círculo usando um Vector2 para a posição, utiliza-se Posição(Vector2), Raio, Cor.

        EndDrawing();
        // EndDrawing: Finaliza a renderização e exibe o desenho na tela.
    }

    CloseWindow();
    // CloseWindow: Fecha a janela do programa e libera a memória alocada.

    return 0;
}