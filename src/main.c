#include "mapa.h"
#include "camera.h"
#include "entidades.h"
#include "inventario.h"
#include "combate.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

int main(){

    const int larguraTela = 800;
    const int alturaTela = 600;

    InitWindow(larguraTela, alturaTela, "UFERSAQUEST");
    SetTargetFPS(60);

    // Cria a matriz e chama a função para preencher ela com paredes e chão
    int linhas = 0, colunas = 0;
    int faseAtual = 1;
    int **mapa = inicializarMapa("./mapa1.txt", &linhas, &colunas);

    //Texture2D  = LoadTexture("jogador.png");
    //if (!IsTextureValid(personagem)){
    //    printf("erroo");
    //}
    //Rectangle Recorte = {(personagem.width/7)*2, 0, personagem.height, personagem.width};

    // Inicializa a câmera usando o tamanho da janela
    Camera2D camera = criarCamera(larguraTela, alturaTela);

    // Inicializar Entidades
    Jogador jogador;
    inicializarJogador(&jogador, (Vector2){ 1 * MAPA_TILE_SIZE, 1 * MAPA_TILE_SIZE });

    // Carrega a textura do jogador
    jogador.textura = LoadTexture("../assets/images/caloura.png");
    /*jogador.textura = LoadTexture("../assets/images/personagem.png");*/

    /* // inicializar variáveis de controle do jogador
    jogador.frame = 0;
    jogador.contador = 0;
    jogador.estaAndando = false;
    jogador.direcaoOlhar = 1.0f;
    */

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

    // Inicializa o controle do menu
    EstadoMenuInventario menuInventario = {0}; 
    strcpy(menuInventario.textoManual, "Erro: manual.txt nao encontrado!");
    
    FILE *arqManual = fopen("manual.txt", "r");
    if(arqManual){
        fread(menuInventario.textoManual, sizeof(char), 999, arqManual);
        fclose(arqManual);
    }
    
    // Configuração dos itens no chão
    ItemNoChao itensChao[20];
    int qtdItensChao = 0;

    // Colocando o Manual no Mapa 1
    itensChao[qtdItensChao].item = (Item){"Manual de C", ITEM_MANUAL, 0};
    itensChao[qtdItensChao].pos = (Vector2){ 15 * MAPA_TILE_SIZE, 15 * MAPA_TILE_SIZE };
    itensChao[qtdItensChao].fase = 1;
    itensChao[qtdItensChao].ativo = true;
    qtdItensChao++;

    // Colocando a Coxinha no Mapa 2
    itensChao[qtdItensChao].item = (Item){"Coxinha", ITEM_CURA, 40};
    itensChao[qtdItensChao].pos = (Vector2){ 15 * MAPA_TILE_SIZE, 10 * MAPA_TILE_SIZE };
    itensChao[qtdItensChao].fase = 2;
    itensChao[qtdItensChao].ativo = true;
    qtdItensChao++;

    while (!WindowShouldClose()){
        /*int contadorAnimacao = 0;
        int frameAnimacao = 0;*/

        if (estadoAtual == ESTADO_EXPLORACAO){
            
            moverJogador(&jogador, mapa, linhas, colunas);

            if (IsKeyPressed(KEY_TAB)) estadoAtual = ESTADO_INVENTARIO;

            if (IsKeyPressed(KEY_ENTER)){
                interagirComAmbiente(&jogador, itensChao, &qtdItensChao, faseAtual, mapa, linhas, colunas);
            }

            // Checar a troca de fase (Atualiza o ponteiro do mapa se ele pisar na porta)
            mapa = checarTransicaoDeFase(mapa, &linhas, &colunas, &jogador, &faseAtual);

            atualizarCamera(&camera, jogador.pos, jogador.tamanho, larguraTela, alturaTela, linhas, colunas);

            // Checar se o jogador tocou no inimigo para iniciar a batalha
            if (inimigo.ativo && faseAtual == 1){
                Rectangle recJogador = { jogador.pos.x, jogador.pos.y, jogador.tamanho, jogador.tamanho };
                Rectangle recInimigo = { inimigo.pos.x, inimigo.pos.y, inimigo.tamanho, inimigo.tamanho };
                
                if (CheckCollisionRecs(recJogador, recInimigo)){
                    estadoAtual = ESTADO_COMBATE; // Muda a tela
                }
            }
            
        }else if (estadoAtual == ESTADO_INVENTARIO){
            atualizarMenuInventario(&jogador, &menuInventario, itensChao, &qtdItensChao, faseAtual, &estadoAtual);
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
                        itensChao[qtdItensChao].item = (Item){"Chave Dourada", ITEM_CHAVE, 0};
                        itensChao[qtdItensChao].pos = inimigo.pos; // A chave cai exatamente onde o inimigo estava
                        itensChao[qtdItensChao].fase = faseAtual;
                        itensChao[qtdItensChao].ativo = true;
                        qtdItensChao++;
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

                desenharMapa(mapa, linhas, colunas);

                for (int i = 0; i < qtdItensChao; i++){
                    if (itensChao[i].ativo && itensChao[i].fase == faseAtual){
                        Color corItem = WHITE;
                        if (itensChao[i].item.tipo == ITEM_CHAVE) corItem = GOLD; // Chave dourada
                        if (itensChao[i].item.tipo == ITEM_MANUAL) corItem = DARKBLUE; // Manual azul
                        if (itensChao[i].item.tipo == ITEM_CURA) corItem = GREEN; // Cura verde

                        DrawRectangleV(itensChao[i].pos, (Vector2){ MAPA_TILE_SIZE, MAPA_TILE_SIZE }, corItem);
                    }
                }

                /*
                // Desenha o jogador (Azul)
                DrawRectangleV(jogador.pos, (Vector2){ jogador.tamanho, jogador.tamanho }, BLUE);
                */

                // Desenha o jogador com animação
                desenharJogador(&jogador);
                /*desenharJogador(&jogador, &contadorAnimacao, &frameAnimacao);*/
                
                // Desenha o inimigo (Vermelho) se estiver vivo
                if (inimigo.ativo && faseAtual == 1){
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
        
        } else if (estadoAtual == ESTADO_INVENTARIO){
            desenharMenuInventario(&jogador, &menuInventario, larguraTela, alturaTela);
        }
        //DrawTexturePro(personagem, Recorte, (Rectangle){100, 100, Recorte.width*0.5, Recorte.height*0.5}, (Vector2){0,0}, 0, WHITE);

        EndDrawing();
    }

    liberarMapa(mapa, linhas);
    liberarBancoPerguntas(bancoPerguntas);
    UnloadTexture(jogador.textura); // descarregar jogador
    CloseWindow();
    return 0;
}
    