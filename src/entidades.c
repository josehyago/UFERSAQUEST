// Entidades.c
#include <stdio.h>
#include "entidades.h"

// FUNÇÃO: inicializarJogador
// PARÂMETROS: j - ponteiro para o jogador a preencher; posInicial - posição (pixels) onde ele nasce.
// POR QUE: toda struct precisa de valores iniciais; sem isso ficaria com "lixo" de memória.
// PARA QUE SERVE: define nome, vida, tamanhos, velocidade e zera mochila e animação. A textura (imagem) é carregada depois, na main.
void inicializarJogador(Jogador *j, Vector2 posInicial){
    snprintf(j->nome, sizeof(j->nome), "%s", "Ana Paula"); // copia o nome respeitando o tamanho do vetor
    j->hp = HP_MAXIMO;        // vida inicial
    j->score = 0;
    j->pos = posInicial;      // posição inicial passada como parâmetro
    j->tamanho = 80.0f;       // sprite desenhado com 80x80 pixels
    j->hitbox = 56.0f;        // área de colisão menor que o sprite, para caber em portas e corredores
    j->velocidade = 4.0f;     // pixels por frame
    j->qtdItens = 0;          // mochila começa vazia
    j->frameAtual = 0;
    j->tempoAnimacao = 0.0f;
    j->linhaAnimacao = 0;
    j->espelharSprite = false;
}

// FUNÇÃO: inicializarInimigo
// PARÂMETROS: i - ponteiro para o inimigo a preencher; posInicial - onde ele fica; nome - nome exibido.
// POR QUE: mesmo motivo de inicializarJogador, e assim dá para criar vários inimigos com a mesma função.
// PARA QUE SERVE: define nome, vida, posição e tamanho do inimigo e o marca como vivo.
void inicializarInimigo(Inimigo *i, Vector2 posInicial, const char *nome){
    snprintf(i->nome, sizeof(i->nome), "%s", nome); // copia o nome (que vem de fora) com limite de tamanho
    i->hp = 60;           // vida do inimigo
    i->pos = posInicial;  // posição inicial
    i->tamanho = 60.0f;   // lado do quadrado do inimigo
    i->ativo = true;      // começa vivo
}

// FUNÇÃO: obterHitbox
// PARÂMETROS: j - ponteiro (somente leitura) para o jogador.
// POR QUE: o sprite (80px) é maior que a área de colisão (56px). Com o sprite inteiro, portas e corredores de 1 tile (100px) ficariam difíceis de atravessar.
// PARA QUE SERVE: RETORNA o retângulo de colisão, centralizado na horizontal e colado nos "pés" do sprite. Toda colisão do jogador (mapa, inimigo, itens, portas) deve usar esta função.
Rectangle obterHitbox(const Jogador *j){
    return (Rectangle){
        j->pos.x + (j->tamanho - j->hitbox) / 2.0f, // x: centraliza (a sobra é dividida entre os dois lados)
        j->pos.y + (j->tamanho - j->hitbox),        // y: base da hitbox alinhada com a base do sprite
        j->hitbox,                                  // largura da colisão
        j->hitbox                                   // altura da colisão
    };
}

// FUNÇÃO: moverJogador
// PARÂMETROS: j - jogador a mover; vetor_mapa, linhas, colunas - o mapa, para testar colisão.
// POR QUE: o movimento precisa respeitar paredes e manter a mesma velocidade em qualquer direção.
// PARA QUE SERVE: lê o teclado (WASD/setas), move o jogador testando colisão em cada eixo separado (assim ele "desliza" na parede) e atualiza a animação do sprite.
void moverJogador(Jogador *j, int **vetor_mapa, int linhas, int colunas){
    // Lê a direção desejada: dx/dy ficam em -1, 0 ou +1.
    float dx = 0, dy = 0;
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) dx += 1;
    if (IsKeyDown(KEY_LEFT)  || IsKeyDown(KEY_A)) dx -= 1;
    if (IsKeyDown(KEY_DOWN)  || IsKeyDown(KEY_S)) dy += 1;
    if (IsKeyDown(KEY_UP)    || IsKeyDown(KEY_W)) dy -= 1;

    bool movendo = (dx != 0 || dy != 0);

    // Na diagonal, multiplica por ~0.7071 (1/raiz de 2) para não andar mais rápido que na reta.
    if (dx != 0 && dy != 0){ dx *= 0.7071f; dy *= 0.7071f; }

    // Eixo X: move e, se a hitbox bater em algo, volta para a posição anterior.
    float xAnterior = j->pos.x;
    j->pos.x += dx * j->velocidade;
    if (checarColisaoComMapa(vetor_mapa, linhas, colunas, obterHitbox(j))) j->pos.x = xAnterior;

    // Eixo Y: mesma lógica (já usando o X validado).
    float yAnterior = j->pos.y;
    j->pos.y += dy * j->velocidade;
    if (checarColisaoComMapa(vetor_mapa, linhas, colunas, obterHitbox(j))) j->pos.y = yAnterior;

    // Escolhe a linha da spritesheet: 2 = lados, 0 = frente, 1 = costas.
    // O sprite lateral original olha para a esquerda, então ao ir para a direita ele é espelhado.
    if (dx != 0){ j->linhaAnimacao = 2; j->espelharSprite = (dx > 0); }
    if (dy > 0) j->linhaAnimacao = 0;
    if (dy < 0) j->linhaAnimacao = 1;

    if (movendo){
        // Acumula o tempo e troca de quadro a cada 0,15 s (ciclo de 4 quadros).
        j->tempoAnimacao += GetFrameTime();
        if (j->tempoAnimacao >= 0.15f){
            j->tempoAnimacao = 0.0f;
            j->frameAtual = (j->frameAtual + 1) % 4;
        }
    } else {
        // Parado: volta para o quadro em pé.
        j->frameAtual = 0;
        j->tempoAnimacao = 0.0f;
    }
}

// FUNÇÃO: jogadorTocaInimigo
// PARÂMETROS: j - jogador; i - inimigo (ambos somente leitura).
// POR QUE: a main não precisa conhecer como a colisão é calculada.
// PARA QUE SERVE: RETORNA true se a hitbox do jogador encosta no quadrado do inimigo.
bool jogadorTocaInimigo(const Jogador *j, const Inimigo *i){
    Rectangle recInimigo = { i->pos.x, i->pos.y, i->tamanho, i->tamanho };
    return CheckCollisionRecs(obterHitbox(j), recInimigo);
}

// FUNÇÃO: desenharJogador
// PARÂMETROS: j - jogador (somente leitura).
// POR QUE: a imagem do personagem é uma grade de quadros; é preciso recortar só o quadro certo.
// PARA QUE SERVE: desenha o quadro atual da caloura.png (grade de 4 colunas x 3 linhas), escalado para o tamanho do sprite e espelhado quando necessário.
void desenharJogador(const Jogador *j){
    // Tamanho de um quadro dentro da imagem.
    float larguraQuadro = (float)j->textura.width / 4.0f;
    float alturaQuadro = (float)j->textura.height / 3.0f;

    // Largura negativa no recorte faz a Raylib espelhar a imagem horizontalmente.
    float larguraRecorte = j->espelharSprite ? -larguraQuadro : larguraQuadro;

    // Fonte = pedaço da imagem a copiar (coluna = frameAtual, linha = linhaAnimacao).
    Rectangle fonte = {
        j->frameAtual * larguraQuadro,
        j->linhaAnimacao * alturaQuadro,
        larguraRecorte,
        alturaQuadro
    };

    // Destino = onde e com que tamanho o quadro aparece no mapa.
    Rectangle destino = { j->pos.x, j->pos.y, j->tamanho, j->tamanho };
    Vector2 origem = { 0.0f, 0.0f }; // sem rotação, então a origem é o canto superior esquerdo

    DrawTexturePro(j->textura, fonte, destino, origem, 0.0f, WHITE);
}

// FUNÇÃO: desenharInimigo
// PARÂMETROS: i - inimigo (somente leitura).
// POR QUE: mantém a main só chamando funções, sem detalhes de desenho.
// PARA QUE SERVE: desenha o inimigo como um quadrado vermelho (até ele ganhar um sprite).
void desenharInimigo(const Inimigo *i){
    DrawRectangleV(i->pos, (Vector2){ i->tamanho, i->tamanho }, RED);
}