#include "ufersaquest.h"

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