#include "raylib.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Definimos o tamanho da nossa grade (matriz) e o tamanho de cada bloco (Tile) em pixels.
#define MAPA_LINHAS 25
#define MAPA_COLUNAS 35
#define MAPA_TILE_SIZE 40 // Cada bloco terá 40x40 pixels na tela

typedef struct{
    char nome[20];
    int hp;
    int score;
    Vector2 pos;
    float tamanho;
    float velocidade;
} Jogador;

typedef struct{
    char nome[20];
    int hp;
    Vector2 pos;
    float tamanho;
    bool ativo; // Define se o inimigo ainda está vivo no mapa
} Inimigo;

typedef struct{
    char texto[200];
    char opcoes[4][100];
    int respostaCorreta; // Índice (0 a 3)
    bool opcaoOculta[4]; // Controla quais opções sumiram após errar
} Pergunta;

// FUNÇÃO: inicializarMapa
// OBJETIVO: Preencher a matriz com 0 (chão) e 1 (parede).
// Como funciona: Ele percorre cada linha e coluna. Se for a borda do mapa, transforma em parede. O resto vira chão.
void inicializarMapa(int mapa[MAPA_LINHAS][MAPA_COLUNAS]){
    for (int i = 0; i < MAPA_LINHAS; i++){
        for(int j = 0; j < MAPA_COLUNAS; j++){

            // Checa se é a primeira linha (i==0), última linha (MAPA_LINHAS-1), primeira coluna (j==0) ou última coluna (MAPA_COLUNAS-1).
            if(i == 0 || i == MAPA_LINHAS - 1 || j == 0 || j == MAPA_COLUNAS - 1){
                mapa[i][j] = 1; // 1 representa a Parede/Borda
            } else {
                mapa[i][j] = 0; // 0 representa o Chão livre
            }
        }
    }
    
    // Adicionando obstáculos manuais no meio do cenário.
    mapa[5][5] = 1;
    mapa[5][6] = 1;
    mapa[10][15] = 1;
}

// FUNÇÃO: desenharMapa
// OBJETIVO: Ler a matriz e desenhar os quadrados coloridos na tela.
void desenharMapa(int mapa[MAPA_LINHAS][MAPA_COLUNAS]){
    for (int i = 0; i < MAPA_LINHAS; i++){
        for(int j = 0; j < MAPA_COLUNAS; j++){

            // Converte a posição da matriz (índices 0, 1, 2...) para pixels na tela.
            // Ex: coluna 2 * 40px = posição 80px no eixo X.
            int posX = j * MAPA_TILE_SIZE;
            int posY = i * MAPA_TILE_SIZE;
            
            // Desenha o bloco dependendo do número salvo na matriz
            if(mapa[i][j] == 1){
                DrawRectangle(posX, posY, MAPA_TILE_SIZE, MAPA_TILE_SIZE, DARKGRAY); // Parede escura
            } else if(mapa[i][j] == 0){
                DrawRectangle(posX, posY, MAPA_TILE_SIZE, MAPA_TILE_SIZE, LIGHTGRAY); // Chão claro
            }
            // Desenha as linhas de grade para facilitar a visualização
            DrawRectangleLines(posX, posY, MAPA_TILE_SIZE, MAPA_TILE_SIZE, GRAY);
        } 
    }
}


// FUNÇÃO: checarColisaoComMapa
// OBJETIVO: Impedir que o jogador atravesse os blocos de valor "1".
// RETORNA: 'true' se bateu em uma parede, 'false' se o caminho estiver livre.
bool checarColisaoComMapa(int mapa[MAPA_LINHAS][MAPA_COLUNAS], Rectangle playerRec){

    // Em vez de checar todos os blocos do mapa, checamos apenas os blocos que estão imediatamente ao redor e debaixo do jogador.
    // Convertendo a posição em pixels do jogador de volta para índices da matriz:
    int minX = playerRec.x / MAPA_TILE_SIZE;
    int minY = playerRec.y / MAPA_TILE_SIZE;
    int maxX = (playerRec.x + playerRec.width) / MAPA_TILE_SIZE;
    int maxY = (playerRec.y + playerRec.height) / MAPA_TILE_SIZE;

    // Vasculha apenas a área próxima ao jogador.
    for (int i = minY; i <= maxY; i++){
        for (int j = minX; j <= maxX; j++){
            if (mapa[i][j] == 1){ // Se o bloco verificado for uma parede.

                // Cria um Rectangle para esse bloco de parede.
                Rectangle bloco = { j * MAPA_TILE_SIZE, i * MAPA_TILE_SIZE, MAPA_TILE_SIZE, MAPA_TILE_SIZE };
                
                // A função CheckCollisionRecs da Raylib cruza as duas caixas (jogador e bloco).
                // Se elas se sobrepuserem, houve colisão.
                if (CheckCollisionRecs(playerRec, bloco)){
                    return true; // Colisão confirmada. Interrompe a função.
                }
            }
        }
    }
    return false; // Se o loop terminar sem achar parede, o caminho está livre.
}

// FUNÇÃO: criarCamera
// OBJETIVO: Configurar as propriedades iniciais da câmera 2D.
Camera2D criarCamera(int larguraTela, int alturaTela){
    Camera2D camera = { 0 };

    // O offset define onde o alvo da câmera vai ficar desenhado na sua tela.
    // Colocando metade da largura e altura, garantimos que o jogador fique sempre no centro da janela.
    camera.offset = (Vector2){ larguraTela / 2.0f, alturaTela / 2.0f };
    camera.zoom = 1.0f; // Zoom normal (100%)
    return camera;
}

// FUNÇÃO: atualizarCamera
// OBJETIVO: Fazer a câmera seguir o jogador, mas sem mostrar o vazio fora do mapa.
void atualizarCamera(Camera2D *camera, Vector2 playerPos, float playerSize, int larguraTela, int alturaTela){

    // Define que a câmera deve mirar exatamente no centro do quadrado do jogador
    float alvoX = playerPos.x + (playerSize / 2);
    float alvoY = playerPos.y + (playerSize / 2);

    // Calcula os limites máximos que a câmera pode ir sem revelar o que está fora da matriz.
    // Ex: Ela não pode ir mais para a esquerda do que a metade da sua tela.
    float minX = larguraTela / 2.0f;
    float maxX = (MAPA_COLUNAS * MAPA_TILE_SIZE) - (larguraTela / 2.0f);
    float minY = alturaTela / 2.0f;
    float maxY = (MAPA_LINHAS * MAPA_TILE_SIZE) - (alturaTela / 2.0f);

    if (alvoX < minX) alvoX = minX;
    if (alvoX > maxX) alvoX = maxX;
    if (alvoY < minY) alvoY = minY;
    if (alvoY > maxY) alvoY = maxY;

    // Atualiza o alvo final da câmera após as correções.
    camera->target = (Vector2){ alvoX, alvoY };
}

void inicializarJogador(Jogador *j, Vector2 posInicial){
    strcpy(j->nome, "Florzinha");
    j->hp = 100;
    j->score = 0;
    j->pos = posInicial;
    j->tamanho = 30.0f;
    j->velocidade = 4.0f;
}

void inicializarInimigo(Inimigo *i, Vector2 posInicial, const char *nome){
    strcpy(i->nome, nome);
    i->hp = 60;
    i->pos = posInicial;
    i->tamanho = 30.0f;
    i->ativo = true;
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

Pergunta* criarBancoPerguntas(int *qtdPerguntas){
    *qtdPerguntas = 3;
    Pergunta *banco = (Pergunta*) malloc((*qtdPerguntas) * sizeof(Pergunta));

    if (banco == NULL){
        printf("Erro ao alocar memoria para as perguntas!\n");
        return NULL;
    }

    strcpy(banco[0].texto, "Qual estrutura segue o conceito FIFO?");
    strcpy(banco[0].opcoes[0], "1) Pilha");
    strcpy(banco[0].opcoes[1], "2) Fila");
    strcpy(banco[0].opcoes[2], "3) Arvore");
    strcpy(banco[0].opcoes[3], "4) Grafo");
    banco[0].respostaCorreta = 1; // Fila (índice 1)

    strcpy(banco[1].texto, "Qual ponteiro usamos para alocar memoria em C?");
    strcpy(banco[1].opcoes[0], "1) malloc");
    strcpy(banco[1].opcoes[1], "2) printf");
    strcpy(banco[1].opcoes[2], "3) scanf");
    strcpy(banco[1].opcoes[3], "4) sizeof");
    banco[1].respostaCorreta = 0; // malloc (índice 0)

    strcpy(banco[2].texto, "Quantos bits tem 1 Byte?");
    strcpy(banco[2].opcoes[0], "1) 4 bits");
    strcpy(banco[2].opcoes[1], "2) 16 bits");
    strcpy(banco[2].opcoes[2], "3) 8 bits");
    strcpy(banco[2].opcoes[3], "4) 32 bits");
    banco[2].respostaCorreta = 2; // 8 bits (índice 2)

    // Garantir que todas as opções comecem visíveis
    for(int i = 0; i < *qtdPerguntas; i++){
        for(int j = 0; j < 4; j++){
            banco[i].opcaoOculta[j] = false;
        }
    }

    return banco;
}

void liberarBancoPerguntas(Pergunta *banco){
    if (banco != NULL){
        free(banco);
    }
}

// Retorna true se a batalha acabou (Alguém chegou a 0 HP)
// Processa o resultado da resposta, oculta a opção em caso de erro e prepara os textos
bool resolverTurno(Jogador *j, Inimigo *i, Pergunta *p, int escolha, char *mensagem, Color *corMensagem, bool *mudarPergunta){
    int dano = 20;

    if (escolha == p->respostaCorreta){
        i->hp -= dano;
        if (i->hp < 0) i->hp = 0;
        j->score += 10; // Aumenta a pontuação do jogador por acertar

        strcpy(mensagem, "RESPOSTA CORRETA! Dano no chefe!");
        *corMensagem = GREEN;
        *mudarPergunta = true; // Avança para a próxima pergunta
    } else{
        j->hp -= dano;
        if (j->hp < 0) j->hp = 0;

        p->opcaoOculta[escolha] = true; // Esconde a opção errada escolhida

        strcpy(mensagem, "ERROU! Voce sofreu dano!");
        *corMensagem = RED;
        *mudarPergunta = false; // Continua na mesma pergunta
    }
    
    return (j->hp == 0 || i->hp == 0);
}

void desenharInterfaceCombate(Pergunta p, Jogador j, Inimigo i){
    DrawText(TextFormat("Jogador: %s | HP: %d | Score: %d", j.nome, j.hp, j.score), 50, 40, 20, GREEN);
    DrawText(TextFormat("Chefe: %s | HP: %d", i.nome, i.hp), 500, 40, 20, RED);

    DrawRectangle(40, 300, 720, 250, LIGHTGRAY);
    DrawRectangleLines(40, 300, 720, 250, DARKGRAY);

    DrawText(p.texto, 60, 320, 20, BLACK);

    // Só desenha se não estiver oculta
    if (!p.opcaoOculta[0]) DrawText(p.opcoes[0], 70, 380, 18, DARKBLUE);
    if (!p.opcaoOculta[1]) DrawText(p.opcoes[1], 400, 380, 18, DARKBLUE);
    if (!p.opcaoOculta[2]) DrawText(p.opcoes[2], 70, 440, 18, DARKBLUE);
    if (!p.opcaoOculta[3]) DrawText(p.opcoes[3], 400, 440, 18, DARKBLUE);

    DrawText("Pressione as teclas (1, 2, 3 ou 4) para responder!", 60, 510, 15, DARKGRAY);
}

// Estados possíveis do jogo
typedef enum { ESTADO_EXPLORACAO, ESTADO_COMBATE, ESTADO_GAMEOVER } EstadoJogo;

int main(){

    const int larguraTela = 800;
    const int alturaTela = 600;

    InitWindow(larguraTela, alturaTela, "UFERSAQUEST");
    SetTargetFPS(60);

    // Cria a matriz e chama a função para preencher ela com paredes e chão
    int mapa[MAPA_LINHAS][MAPA_COLUNAS];
    inicializarMapa(mapa);

    // Inicializa a câmera usando o tamanho da janela
    Camera2D camera = criarCamera(larguraTela, alturaTela);

    // Inicializar Entidades
    Jogador jogador;
    inicializarJogador(&jogador, (Vector2){ 1 * MAPA_TILE_SIZE, 1 * MAPA_TILE_SIZE });

    Inimigo inimigo;
    // Colocando o inimigo no meio do mapa (Coluna 10, Linha 10)
    inicializarInimigo(&inimigo, (Vector2){ 10 * MAPA_TILE_SIZE, 10 * MAPA_TILE_SIZE }, "Bug de C");

    // Inicializar Combate
    int qtdPerguntas;
    Pergunta *bancoPerguntas = criarBancoPerguntas(&qtdPerguntas);
    int perguntaAtual = 0;

    // Variáveis de Controle do Combate e Tempo
    float tempoMensagem = 0.0f;
    char mensagemCombate[100] = "";
    Color corMensagem = BLANK;
    bool mudarPergunta = false;

    EstadoJogo estadoAtual = ESTADO_EXPLORACAO;

    while (!WindowShouldClose()){

        if (estadoAtual == ESTADO_EXPLORACAO){
            
            moverJogador(&jogador, mapa);
            atualizarCamera(&camera, jogador.pos, jogador.tamanho, larguraTela, alturaTela);

            // Checar se o jogador tocou no inimigo para iniciar a batalha
            if (inimigo.ativo){
                Rectangle recJogador = { jogador.pos.x, jogador.pos.y, jogador.tamanho, jogador.tamanho };
                Rectangle recInimigo = { inimigo.pos.x, inimigo.pos.y, inimigo.tamanho, inimigo.tamanho };
                
                if (CheckCollisionRecs(recJogador, recInimigo)){
                    estadoAtual = ESTADO_COMBATE; // Muda a tela
                }
            }
            
        } else if (estadoAtual == ESTADO_COMBATE){
            
            // Se o temporizador está ativo, aguarda 2 segundos exibindo a mensagem
            if (tempoMensagem > 0){
                tempoMensagem -= GetFrameTime();

                if (tempoMensagem <= 0){
                    if (jogador.hp <= 0){
                        estadoAtual = ESTADO_GAMEOVER;
                    } else if (inimigo.hp <= 0){
                        inimigo.ativo = false;
                        estadoAtual = ESTADO_EXPLORACAO;
                    } else if (mudarPergunta){
                        perguntaAtual++;
                        if (perguntaAtual >= qtdPerguntas) perguntaAtual = 0;
                        mudarPergunta = false;
                    }
                }
            } else {
                // Leitura dos botões de resposta
                int escolha = -1;
                if (IsKeyPressed(KEY_ONE)) escolha = 0;
                if (IsKeyPressed(KEY_TWO)) escolha = 1;
                if (IsKeyPressed(KEY_THREE)) escolha = 2;
                if (IsKeyPressed(KEY_FOUR)) escolha = 3;

                // Processa apenas escolhas válidas e não ocultas
                if (escolha != -1 && !bancoPerguntas[perguntaAtual].opcaoOculta[escolha]){
                    
                    // Executa a lógica de rodada via função
                    resolverTurno(&jogador, &inimigo, &bancoPerguntas[perguntaAtual], escolha, mensagemCombate, &corMensagem, &mudarPergunta);

                    // Ativa a pausa de 2 segundos para o jogador ler o resultado
                    tempoMensagem = 2.0f;
                }
            }
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        if (estadoAtual == ESTADO_EXPLORACAO){
            BeginMode2D(camera);

                desenharMapa(mapa);

                // Desenha o jogador (Azul)
                DrawRectangleV(jogador.pos, (Vector2){ jogador.tamanho, jogador.tamanho }, BLUE);
                
                // Desenha o inimigo (Vermelho) se estiver vivo
                if (inimigo.ativo){
                    DrawRectangleV(inimigo.pos, (Vector2){ inimigo.tamanho, inimigo.tamanho }, RED);
                }

            EndMode2D();
            
            DrawText("Ande pelo mapa e encoste no quadrado VERMELHO!", 10, 10, 20, BLACK);

        } else if (estadoAtual == ESTADO_COMBATE){
            
            desenharInterfaceCombate(bancoPerguntas[perguntaAtual], jogador, inimigo);

            if (tempoMensagem > 0){
                DrawRectangle(0, 200, larguraTela, 80, Fade(BLACK, 0.7f));
                int tamanhoTexto = MeasureText(mensagemCombate, 30);
                DrawText(mensagemCombate, (larguraTela - tamanhoTexto) / 2, 225, 30, corMensagem);
            }
            
        } else if (estadoAtual == ESTADO_GAMEOVER){
            DrawText("GAME OVER!", 300, 250, 40, RED);
            DrawText("Você foi reprovado...", 300, 300, 20, DARKGRAY);
        }

        EndDrawing();
    }

    liberarBancoPerguntas(bancoPerguntas);
    CloseWindow();
    return 0;
}