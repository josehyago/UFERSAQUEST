#include "ufersaquest.h"

                    // Mapa.h

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

                    // Entidades.h

// Inicializar o jogador no jogo
void inicializarJogador (Jogador *j, Vector2 posInicial){
    strcpy (j->nome, "Florzinha"); // Copia o texto "Florzinha" para o atributo nome do jogador
    j->hp = 100; // vida inicial
    j->score = 0;
    j->pos = posInicial; // define a posição inicial passada como parâmetro
    j->tamanho = 30.0f; // define a largura e altura
    j->velocidade= 4.0f; // pixels por frame
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

                        // Combates.h

// Função que usa alocação dinâmica (malloc) para criar o banco de perguntas
Pergunta* criarBancoPerguntas(int *qtdPerguntas){
    *qtdPerguntas = 3; // Definindo quantidade de perguntas de disciplinas

    // Alocando espaço na memória usando malloc
    Pergunta *banco = (Pergunta*) malloc((*qtdPerguntas) * sizeof(Pergunta));

    if (banco == NULL){
        printf("Erro ao alocar memoria para as perguntas!\n");
        return NULL;
    }

    // Pergunta 1: Algoritmos
    strcpy(banco[0].texto, "Qual estrutura segue o conceito FIFO (First In, First Out)?");
    strcpy(banco[0].opcoes[0], "1) Pilha");
    strcpy(banco[0].opcoes[1], "2) Fila");
    strcpy(banco[0].opcoes[2], "3) Arvore");
    strcpy(banco[0].opcoes[3], "4) Grafo");
    banco[0].respostaCorreta = 1; // "Fila" (indice 1)

    // Pergunta 2: AED 1
    strcpy(banco[1].texto, "Qual ponteiro usamos para alocar memoria dinamicamente em C?");
    strcpy(banco[1].opcoes[0], "1) malloc");
    strcpy(banco[1].opcoes[1], "2) printf");
    strcpy(banco[1].opcoes[2], "3) scanf");
    strcpy(banco[1].opcoes[3], "4) sizeof");
    banco[1].respostaCorreta = 0; // "malloc" (indice 0)

    // Pergunta 3: Computacao
    strcpy(banco[2].texto, "Quantos bits tem 1 Byte?");
    strcpy(banco[2].opcoes[0], "1) 4 bits");
    strcpy(banco[2].opcoes[1], "2) 16 bits");
    strcpy(banco[2].opcoes[2], "3) 8 bits");
    strcpy(banco[2].opcoes[3], "4) 32 bits");
    banco[2].respostaCorreta = 2; // "8 bits" (indice 2)

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

// Função bool resolverTurno usando ponteiros
// Se acertar -> diminui HP do Inimigo. Se errar -> diminui HP do Jogador (j->hp).
// Retorna true se a batalha acabou (Alguém chegou a 0 HP)
// Processa o resultado da resposta, oculta a opção em caso de erro e prepara os textos
bool resolverTurno(Jogador *j, Inimigo *i, Pergunta *p, int escolha,  char *mensagem, Color *corMensagem, bool *mudarPergunta){
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

        p->opcaoOculta[escolha] = true; // Oculta a opção escolhida

        strcpy(mensagem, "RESPOSTA ERRADA! Você levou dano.");
        *corMensagem = RED;
    }
    
    return (j->hp == 0 || i->hp == 0);
}

// Interface de combate usando Raylib (DrawText para mostrar pergunta e opções)
void desenharInterfaceCombate(Pergunta p, Jogador j, Inimigo i){
    // Exibindo HP dos personagens
    DrawText(TextFormat("Jogador: %s | HP: %d | Score: %d", j.nome, j.hp, j.score), 50, 40, 20, GREEN);
    DrawText(TextFormat("Chefe: %s | HP: %d", i.nome, i.hp), 500, 40, 20, RED);

    // Caixa de fundo para o Quiz
    DrawRectangle(40, 300, 720, 250, LIGHTGRAY);
    DrawRectangleLines(40, 300, 720, 250, DARKGRAY);

    // Mostra o enunciado da Pergunta
    DrawText(p.texto, 60, 320, 20, BLACK);

    // Mostra as 4 Opções usando DrawText em posições diferentes
    // Só desenha se não estiver oculta
    if (!p.opcaoOculta[0]) DrawText(p.opcoes[0], 70, 380, 18, DARKBLUE);
    if (!p.opcaoOculta[1]) DrawText(p.opcoes[1], 400, 380, 18, DARKBLUE);
    if (!p.opcaoOculta[2]) DrawText(p.opcoes[2], 70, 440, 18, DARKBLUE);
    if (!p.opcaoOculta[3]) DrawText(p.opcoes[3], 400, 440, 18, DARKBLUE);

    DrawText("Pressione as teclas (1, 2, 3 ou 4) para responder!", 60, 510, 15, DARKGRAY);
}