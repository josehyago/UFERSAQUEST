// Entidades.c
#include <string.h>
#include "entidades.h"

// Inicializar o jogador no jogo
void inicializarJogador (Jogador *j, Vector2 posInicial){
    strcpy (j->nome, "Ana Paula"); // Copia o texto "Ana Paula" para o atributo nome do jogador
    j->hp = 100; // vida inicial
    j->score = 0;
    j->pos = posInicial; // define a posição inicial passada como parâmetro
    j->tamanho = 70.0f; // define a largura e altura
    j->velocidade= 4.0f; // pixels por frame
    j->qtdItens = 0; // Mochila começa vazia
    j->frameAtual = 0;
    j->tempoAnimacao = 0.0f;
    j->linhaAnimacao = 0;
    j->olhandoEsquerda = false;
}

// Inicializar o inimigo no jogo
void inicializarInimigo(Inimigo *i, Vector2 posInicial, const char *nome){
    strcpy (i->nome, nome); // Copia o nome do inimigo passado como parâmetro
    i->hp = 60; // vida do inimigo
    i->pos = posInicial; // posição inicial do inimigo 
    i->tamanho = 30.0f; // dimensão do inimigo
    i->ativo = true; // inicializa vivo
}

// Colocar o jogador para se mover com tratamento de colisão em dois eixos (AABB)
void moverJogador(Jogador *j, int **vetor_mapa, int linhas, int colunas){

    bool movendo = false;

    // EIXO X
    float posXAnterior = j->pos.x; // Guarda a posição X antes de mover

    // Verifica se as teclas da direita e a D estão posicionadas para mover para a direita
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)){ 
        j->pos.x += j->velocidade;
        j->linhaAnimacao = 2; // Linha dos lados na caloura.png
        j->olhandoEsquerda = true;
        movendo = true; 
    }
    // Verifica se as teclas da esquerda e a A estão  pressionadas para mover para a esquerda
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)){  
        j->pos.x -= j->velocidade;
        j->linhaAnimacao = 2; // Linha dos lados
        j->olhandoEsquerda = false; // Espelha o sprite para olhar para a esquerda
        movendo = true;
    }

    // Cria um retângulo de colisão com a nova posição do eixo X
    Rectangle recX = { j->pos.x, j->pos.y, j->tamanho, j->tamanho };
    
    // Se a nova posição colidir com algum obstáculo do mapa, restaura a posição X anterior
    if (checarColisaoComMapa(vetor_mapa, linhas, colunas, recX)) j->pos.x = posXAnterior;
    
    // EIXO Y
    float posYAnterior = j->pos.y; // Guarda a posição Y antes de mover

    // Verifica se as teclas 'S' ou 'Seta para Baixo' estão pressionadas para mover para baixo
    if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)){ 
        j->pos.y += j->velocidade;
        j->linhaAnimacao = 0; // Linha de frente
        movendo = true;
    }
    // Verifica se as teclas 'W' ou 'Seta para Cima' estão pressionadas para mover para cima
    if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)){   
        j->pos.y -= j->velocidade;
        j->linhaAnimacao = 1; // Linha de costas
        movendo = true;
    }
    
    // Cria um retângulo de colisão com a nova posição no eixo Y (já validada no eixo X)
    Rectangle recY = { j->pos.x, j->pos.y, j->tamanho, j->tamanho };
    
    // Se a nova posição colidir com o mapa, restaura a posição Y anterior
    if (checarColisaoComMapa(vetor_mapa, linhas, colunas, recY)) j->pos.y = posYAnterior;

    // Atualiza o ciclo da animação apenas se o jogador estiver se movimentando
    if (movendo){
        j->tempoAnimacao += GetFrameTime();
        if (j->tempoAnimacao >= 0.15f){ // Velocidade de troca de quadros
            j->tempoAnimacao = 0.0f;
            j->frameAtual = (j->frameAtual + 1) % 4; // 4 colunas de animação na grade
        }
    } else {
        j->frameAtual = 0; // Retorna ao quadro estático quando parado
        j->tempoAnimacao = 0.0f;
    }
}

// Função para desenhar a caloura cortando a grade 4x3 corretamente.
void desenharJogador(Jogador *j){
    // Como a caloura.png tem 4 colunas e 3 linhas:
    float larguraQuadro = (float)j->textura.width / 4.0f;
    float alturaQuadro = (float)j->textura.height / 3.0f;

    // Se estiver olhando para a esquerda, inverta a largura do recorte para espelhar o gráfico
    float larguraRecorte = j->olhandoEsquerda ? -larguraQuadro : larguraQuadro;

    Rectangle fonte = { 
        j->frameAtual * larguraQuadro, 
        j->linhaAnimacao * alturaQuadro, 
        larguraRecorte, 
        alturaQuadro 
    };

    Rectangle destino = { j->pos.x, j->pos.y, j->tamanho, j->tamanho };
    Vector2 origem = { 0.0f, 0.0f };

    DrawTexturePro(j->textura, fonte, destino, origem, 0.0f, WHITE);
}

/*
//Função: desenhar o Jogador

void desenharJogador(Jogador *jogador, int *contador, int *frame){
    //1. Calcula o tamanho exato de 1 quadro (1 das 7 bonequinhas na horizontal)
    float larguraQuadro = (float)jogador->textura.width / 7.0f;
    float alturaQuadro = (float)jogador->textura.height;

    //2. Incrementa o contador de tempo
    (*contador)++;

    //3. A cada 10 ciclos do jogo
    if (*contador>10){
        *contador=0; // resetar o contador
        (*frame)++; // Avança para a próxima bonequinha

        // se passar das poses de caminhada (quadros 2, 3, 4 e 5), volta para a pose 2
        if (*frame>5 || *frame <2) {
            *frame=2;
        }
    }

    //4. Define o retângulo de corte (fonte)
    Rectangle fonte ={
        (float)(*frame) * larguraQuadro, // x
        0.0f, //y
        larguraQuadro,
        alturaQuadro
    };


    float escala = 8.0f;

    // Calculamos a proporção exata entre a largura e a altura originais
    float proporcao = larguraQuadro / alturaQuadro;
    // A altura na tela será baseada no j->tamanho e na escala
    float alturaFinal = jogador->tamanho * escala;
    // A largura na tela é ajustada pela proporção para NÃO achatar!
    float larguraFinal = alturaFinal * proporcao;

    //5. Posição e tamanho final onde o personagem vai aparecer no mapa
    Rectangle destino = { jogador->pos.x, jogador->pos.y, larguraFinal, alturaFinal};
    // Origem da rotação (canto superior esquerdo: 0,0)
    Vector2 origem = { 0.0f, 0.0f };

    //6.Desenha na tela o recorte atualizado
    DrawTexturePro(jogador->textura, fonte, destino, origem, 0.0f, WHITE);
}*/