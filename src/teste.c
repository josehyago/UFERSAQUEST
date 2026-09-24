#include "raylib.h"
#include "mapa.h"

// Definimos o tamanho da nossa grade (matriz) e o tamanho de cada bloco (Tile) em pixels.
#define MAPA_LINHAS 25
#define MAPA_COLUNAS 35
#define MAPA_TILE_SIZE 40 // Cada bloco terá 40x40 pixels na tela

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

    // Configurações do jogador
    // Ele nasce na coordenada (40, 40), que é exatamente dentro do primeiro chão livre (linha 1, coluna 1).
    Vector2 jogador = { 40.0f, 40.0f };
    float velocidade = 4.0f;       // Anda 4 pixels a cada frame (loop)
    float tamanhoJogador = 30.0f;  // É um pouco menor que o bloco (40) para passar fácil nas portas

    while (!WindowShouldClose()) {

        // LÓGICA DE MOVIMENTO E COLISÃO (EIXO X - HORIZONTAL)
        float posXAnterior = jogador.x; // Guarda onde ele estava antes de tentar andar

        if (IsKeyDown(KEY_RIGHT)) jogador.x += velocidade;
        if (IsKeyDown(KEY_LEFT))  jogador.x -= velocidade;

        // Cria uma caixa para o jogador na nova posição e testa se ele bateu na matriz
        Rectangle recJogadorX = { jogador.x, jogador.y, tamanhoJogador, tamanhoJogador };
        if (checarColisaoComMapa(mapa, recJogadorX)) {
            jogador.x = posXAnterior; // Se bateu na parede, desfazemos o passo horizontal
        }

        // LÓGICA DE MOVIMENTO E COLISÃO (EIXO Y - VERTICAL)
        // Separar X de Y faz o personagem "deslizar" pela parede se você segurar duas setas ao mesmo tempo na diagonal.

        float posYAnterior = jogador.y; // Guarda a posição antes de andar

        if (IsKeyDown(KEY_DOWN)) jogador.y += velocidade;
        if (IsKeyDown(KEY_UP))   jogador.y -= velocidade;

        // Testa novamente a caixa, agora com a posição Y alterada
        Rectangle recJogadorY = { jogador.x, jogador.y, tamanhoJogador, tamanhoJogador };
        if (checarColisaoComMapa(mapa, recJogadorY)) {
            jogador.y = posYAnterior; // Se bateu, desfazemos o passo vertical
        }

        // CÂMERA
        // Agora que o jogador já se moveu (e foi barrado se necessário), mandamos a câmera segui-lo.
        atualizarCamera(&camera, jogador, tamanhoJogador, larguraTela, alturaTela);

        BeginDrawing();
        ClearBackground(RAYWHITE);

            // MUNDO DA CÂMERA
            // Tudo desenhado entre BeginMode2D e EndMode2D é afetado pela câmera. Ou seja, se move conforme o jogador anda.
            BeginMode2D(camera);
                
                desenharMapa(mapa); // Desenha o cenário primeiro (fica por baixo)
                
                // Desenha o jogador (um quadrado azul) por cima do mapa
                DrawRectangleV(jogador, (Vector2){ tamanhoJogador, tamanhoJogador }, BLUE);
                
            EndMode2D();

            // Tudo desenhado aqui (fora do Mode2D) fica grudado na tela.
            // Ideal para interfaces, barras de vida, pontuação e textos.
            DrawText("Controles: Setas do teclado", 10, 10, 20, BLACK);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}