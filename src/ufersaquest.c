#include "ufersaquest.h"

                    // Mapa.h

// FUNÇÃO: inicializarMapa 
// OBJETIVO: Alocar a matriz dinamicamente e preencher a matriz com 0 (chão) e 1 (parede) lendo um arquivo de texto.
// POR QUE: Facilita a criação de mapas. Você pode desenhar o mapa no Bloco de Notas (usando 0 e 1).
int **inicializarMapa(const char *MapaTxt, int *linhas, int *colunas){
    // Abre o arquivo MapaTxt em modo leitura ("r").
    FILE *file_mapa = fopen(MapaTxt, "r");
    // Se for NULL, imprime um erro no console e retorna NULL.
    if (file_mapa == NULL){
        perror("Erro ao ler o arquivo do mapa");
        fclose(file_mapa);
        return NULL;
    }
    // Lê a primeira linha usando fscanf(file_mapa, "%d %d", linhas, colunas) para descobrir o tamanho.
    fscanf(file_mapa, "%d %d", linhas, colunas);
    // Usar malloc() para criar um vetor de ponteiros (linhas) e, dentro de um laço 'for', dar malloc() para cada coluna.
    int **vetor_mapa = (int **) malloc(*linhas * sizeof(int *));
    if (vetor_mapa == NULL){
        printf("Erro ao alocar memoria.\n");
        return NULL;
    }
    for (int i = 0; i < *linhas; i++){
        vetor_mapa[i] = (int *) malloc(*colunas * sizeof(int));
        if (vetor_mapa[i] == NULL){
            printf("Erro ao alocar memoria.\n");
            for (int x = 0; x < i; x++) free(vetor_mapa[x]);
            free(vetor_mapa);
            return NULL;
        }
    }
    // Usar dois laços 'for' (i e j) e usar fscanf(file_mapa, "%d", &vetor_mapa[i][j]) para ler o resto dos números e preencher a matriz.
    for (int i = 0; i < *linhas; i++){
        for (int j = 0; j < *colunas; j++){
            fscanf(file_mapa, "%d", &vetor_mapa[i][j]);
        }
    }
    // Fecha o arquivo (fclose) e retorna o ponteiro da matriz alocada.
    fclose(file_mapa);
    return vetor_mapa;
}

// FUNÇÃO: liberarMapa 
// OBJETIVO: Limpar a memória do mapa antigo antes de carregar o próximo.
void liberarMapa(int **vetor_mapa, int linhas){
    // Laço 'for' para dar free() em cada linha (mapa[i]).
    for (int i = 0; i < linhas; i++) free(vetor_mapa[i]);
    // Free() no ponteiro principal (mapa).
    free(vetor_mapa);
}

// FUNÇÃO: desenharMapa
// OBJETIVO: Ler a matriz e desenhar os quadrados coloridos na tela.
void desenharMapa(int **vetor_mapa, int linhas, int colunas){
    for (int i = 0; i < linhas; i++){
        for(int j = 0; j < colunas; j++){

            // Converte a posição da matriz (índices 0, 1, 2...) para pixels na tela.
            // Ex: coluna 2 * 40px = posição 80px no eixo X.
            int posX = j * MAPA_TILE_SIZE;
            int posY = i * MAPA_TILE_SIZE;
            
            // Desenha o bloco dependendo do número salvo na matriz
            if(vetor_mapa[i][j] == 1){
                DrawRectangle(posX, posY, MAPA_TILE_SIZE, MAPA_TILE_SIZE, DARKGRAY); // Parede escura
            } else if(vetor_mapa[i][j] == 0){
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
bool checarColisaoComMapa(int **vetor_mapa, int linhas, int colunas, Rectangle playerRec){
    
    // Em vez de checar todos os blocos do mapa, checamos apenas os blocos que estão imediatamente ao redor e debaixo do jogador.
    // Convertendo a posição em pixels do jogador de volta para índices da matriz:
    int minX = playerRec.x / MAPA_TILE_SIZE;
    int minY = playerRec.y / MAPA_TILE_SIZE;
    int maxX = (playerRec.x + playerRec.width) / MAPA_TILE_SIZE;
    int maxY = (playerRec.y + playerRec.height) / MAPA_TILE_SIZE;
    
    // Vasculha apenas a área próxima ao jogador.
    for (int i = minY; i <= maxY; i++){
        for (int j = minX; j <= maxX; j++){
            if (vetor_mapa[i][j] == 1){ // Se o bloco verificado for uma parede.
                
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

// FUNÇÃO: checarTransicaoDeFase
// OBJETIVO: Mudar de sala se o jogador pisar em um bloco de "Porta" (número 2 na matriz).
int **checarTransicaoDeFase(int **vetor_mapa, int *linhas, int *colunas, Jogador *j, int *faseAtual){
    // Recebe a posição X/Y do jogador e vê em qual bloco da matriz ele está pisando.    
    // Converte a posição X/Y do jogador (em pixels) para a posição na Matriz (linha e coluna).
    int col = (int)(j->pos.x / MAPA_TILE_SIZE);
    int lin = (int)(j->pos.y / MAPA_TILE_SIZE);

    // Verificação de segurança para não tentar ler fora da matriz
    if(lin >= 0 && lin < *linhas && col >= 0 && col < *colunas){
        
        // Se a matriz nessa posição for igual a 2 (porta):
        //    - Alterar a variável faseAtual (ex: faseAtual++).
        //    - Chamar liberarMapa() para apagar o mapa atual da memória.
        //    - Chamar inicializarMapa() montando o nome do novo arquivo (ex: sprintf(nomeArq, "mapa%d.txt", faseAtual)).
        //    - Resetar a posição do jogador para o início da nova sala.

        // Verifica se o bloco em que ele pisou é uma entrada ou saída (número 2 e 1).
        int tipoPorta = vetor_mapa[lin][col];
        if(tipoPorta == 2 || tipoPorta == -1){
            
            if(tipoPorta == 2) (*faseAtual)++;
            if(tipoPorta == -1) (*faseAtual)--;

            // Limpa o mapa antigo da memória usando a função que você já criou
            liberarMapa(vetor_mapa, *linhas);

            // Monta o nome do novo arquivo automaticamente (Ex: se faseAtual for 2, vira "mapa2.txt")
            char NovoMapaTxt[30];
            sprintf(NovoMapaTxt, "./mapa%d.txt", *faseAtual);

            // Carrega o novo mapa. Note que passamos os ponteiros de linhas e colunas, 
            // então eles já vão ser atualizados lá dentro com o tamanho do novo mapa!
            int **novoMapa = inicializarMapa(NovoMapaTxt, linhas, colunas);

            // Ajusta a posição do jogador de acordo com a direção da transição
            if (tipoPorta == 2){
                // Se avançou de fase (entrada 2), surge no lado esquerdo do novo mapa
                j->pos.x = 1 * MAPA_TILE_SIZE; 
                j->pos.y = lin * MAPA_TILE_SIZE; // Mantém a altura (linha) por onde entrou
            } else if (tipoPorta == -1){
                // Se recuou de fase (saida -1), surge no lado direito, logo antes da porta
                j->pos.x = (*colunas - 2) * MAPA_TILE_SIZE; 
                j->pos.y = lin * MAPA_TILE_SIZE;
            }

            // Retorna o ponteiro do novo mapa para atualizar na main
            return novoMapa; 
        }
    }
    // Se ele não pisou na porta, a função só devolve o mapa que já estava usando
    return vetor_mapa;
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
void atualizarCamera(Camera2D *camera, Vector2 playerPos, float playerSize, int larguraTela, int alturaTela, int linhas, int colunas){

    // Define que a câmera deve mirar exatamente no centro do quadrado do jogador
    float alvoX = playerPos.x + (playerSize / 2);
    float alvoY = playerPos.y + (playerSize / 2);

    // Calcula os limites máximos que a câmera pode ir sem revelar o que está fora da matriz.
    // Ex: Ela não pode ir mais para a esquerda do que a metade da sua tela.
    float minX = larguraTela / 2.0f;
    float maxX = (colunas * MAPA_TILE_SIZE) - (larguraTela / 2.0f);
    float minY = alturaTela / 2.0f;
    float maxY = (linhas * MAPA_TILE_SIZE) - (alturaTela / 2.0f);

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
void moverJogador(Jogador *j, int **vetor_mapa, int linhas, int colunas){
    // EIXO X
    float posXAnterior = j->pos.x; // Guarda a posição X antes de mover

    // Verifica se as teclas da direita e a D estão posicionadas para mover para a direita
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) j->pos.x += j->velocidade;
    // Verifica se as teclas da esquerda e a A estão  pressionadas para mover para a esquerda
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))  j->pos.x -= j->velocidade;

    // Cria um retângulo de colisão com a nova posição do eixo X
    Rectangle recX = { j->pos.x, j->pos.y, j->tamanho, j->tamanho };
    
    // Se a nova posição colidir com algum obstáculo do mapa, restaura a posição X anterior
    if (checarColisaoComMapa(vetor_mapa, linhas, colunas, recX)) j->pos.x = posXAnterior;
    
    // EIXO Y
    float posYAnterior = j->pos.y; // Guarda a posição Y antes de mover

    // Verifica se as teclas 'S' ou 'Seta para Baixo' estão pressionadas para mover para baixo
    if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) j->pos.y += j->velocidade;
    // Verifica se as teclas 'W' ou 'Seta para Cima' estão pressionadas para mover para cima
    if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W))   j->pos.y -= j->velocidade;
    
    // Cria um retângulo de colisão com a nova posição no eixo Y (já validada no eixo X)
    Rectangle recY = { j->pos.x, j->pos.y, j->tamanho, j->tamanho };
    
    // Se a nova posição colidir com o mapa, restaura a posição Y anterior
    if (checarColisaoComMapa(vetor_mapa, linhas, colunas, recY)) j->pos.y = posYAnterior;
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