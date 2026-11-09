// main.c
#include <stdio.h>
#include "mapa.h"
#include "camera.h"
#include "entidades.h"
#include "inventario.h"
#include "combate.h"
#include "ui.h"
#include "menu.h"

int main(){

    const int larguraTela = 1280;
    const int alturaTela = 720;
    

    InitWindow(larguraTela, alturaTela, "UFERSAQUEST");
    SetTargetFPS(60);
    SetExitKey(KEY_NULL);

    // Cria a matriz e chama a função para preencher ela com paredes e chão
    int linhas = 0, colunas = 0;
    int faseAtual = 1;
    int **mapa = NULL;

    // Inicializa a câmera usando o tamanho da janela
    Camera2D camera = criarCamera(larguraTela, alturaTela);

    Jogador jogador = {0};
    // Carrega a textura do jogador
    jogador.textura = LoadTexture("../assets/images/caloura.png");

    Inimigo inimigo = {0};
    // carrega a textura do inimigo
    inimigo.textura = LoadTexture("../assets/images/bug_de_c.png");

    
    // Prepara o menu do inventário e carrega o texto do manual a partir do arquivo.
    EstadoMenuInventario menuInventario = { 0 };
    carregarTextoManual(&menuInventario, "manual.txt");
    
    // Vetor com os itens que estão no chão (começa zerado, sem lixo de memória).
    ItemNoChao itensChao[MAX_ITENS_CHAO] = {0};
    int qtdItensChao = 0;
    
    // Carrega a fonte com acentos.
    carregarFonteUI("../assets/fonts/open-sans.bold.ttf");
    
    // Carrega a imagem com todas as texturas do cenário (Tileset)
    Texture2D tilesetMapa = LoadTexture("../assets/images/ground_05.png"); // imagem da grama
    Texture2D texParede = LoadTexture("../assets/images/parede.png");
    
    // Prepara o combate (banco de perguntas, temporizador, mensagem). Falhar aqui encerra o jogo.
    EstadoCombate combate;
    if (!inicializarCombate(&combate)){
        liberarMapa(mapa, linhas);
        UnloadTexture(jogador.textura);
        UnloadTexture(inimigo.textura);
        UnloadTexture(tilesetMapa);
        UnloadTexture(texParede);
        descarregarFonteUI();
        CloseWindow();
        return 1;
    }

    // Controle dos menus
    EstadoJogo estadoAtual = ESTADO_MENU; // O jogo agora COMEÇA no menu inicial
    int opcaoMenu = 0;
    int opcaoPausa = 0;
    bool sair = false;

    while (!sair && !WindowShouldClose()){

        switch (estadoAtual){

            case ESTADO_MENU:
                // Menu inicial: Novo Jogo / Sair do Jogo
                AcaoMenu acao = atualizarMenuInicial(&opcaoMenu);
                if (acao == ACAO_NOVO_JOGO){
                    // Mapa: libera o antigo (se existir) e carrega o da fase 1
                    if (mapa != NULL) liberarMapa(mapa, linhas);
                    faseAtual = 1;
                    mapa = inicializarMapa("./mapa1.txt", &linhas, &colunas);

                     if (mapa == NULL){
                        printf("Nao foi possivel carregar o mapa inicial.\n");
                        UnloadTexture(jogador.textura);
                        UnloadTexture(inimigo.textura);
                        UnloadTexture(tilesetMapa);
                        UnloadTexture(texParede);
                        CloseWindow();
                        return 1;
                    }

                    // Inicializar Entidades
                    inicializarJogador(&jogador, (Vector2){ 1 * MAPA_TILE_SIZE, 1 * MAPA_TILE_SIZE });

                    // Colocando o inimigo no meio do mapa (Coluna 10, Linha 10)
                    inicializarInimigo(&inimigo, (Vector2){ 10 * MAPA_TILE_SIZE, 10 * MAPA_TILE_SIZE }, "Bug de C");
                    
                    liberarCombate(&combate); // Limpa o banco de perguntas do combate anterior (se houver)
                    if (!inicializarCombate(&combate)){
                        liberarMapa(mapa, linhas);
                        UnloadTexture(jogador.textura);
                        UnloadTexture(inimigo.textura);
                        UnloadTexture(tilesetMapa);
                        UnloadTexture(texParede);
                        descarregarFonteUI();
                        CloseWindow();
                        return 1;
                    }
 
                    // Menu do inventário (mantém o textoManual)
                    menuInventario.itemSelecionado = 0;
                    menuInventario.menuOpcoesAberto = false;
                    menuInventario.opcaoSelecionada = 0;
                    menuInventario.lendoManual = false;
                    menuInventario.examinandoItemGeral = false;
 
                    // Itens no chão: volta tudo para o estado original
                    qtdItensChao = 0;

                    // Coloca o Manual no mapa 1 e a Coxinha no mapa 2.
                    soltarItemNoChao(itensChao, &qtdItensChao, criarItem("Manual de C", ITEM_MANUAL, 0, "Um manual com as respostas das provas."), (Vector2){ 15 * MAPA_TILE_SIZE, 15 * MAPA_TILE_SIZE }, 1);
                    soltarItemNoChao(itensChao, &qtdItensChao, criarItem("Coxinha", ITEM_CURA, 40, "Uma coxinha do RU fresquinha.\nRecupera parte da vida perdida."), (Vector2){ 15 * MAPA_TILE_SIZE, 10 * MAPA_TILE_SIZE }, 2);
 
                    atualizarCamera(&camera, jogador.pos, jogador.tamanho, larguraTela, alturaTela, linhas, colunas);

                    estadoAtual = ESTADO_EXPLORACAO;
                } else if (acao == ACAO_SAIR){
                    sair = true;
                }
                break;

            case ESTADO_EXPLORACAO:
                // Anda pelo mapa (teclado + colisão + animação).
                moverJogador(&jogador, mapa, linhas, colunas);

                // TAB abre o inventário.
                if (IsKeyPressed(KEY_TAB)) estadoAtual = ESTADO_INVENTARIO;

                // ENTER pega o item do chão e/ou destrava a porta, se o jogador tiver a chave.
                if (IsKeyPressed(KEY_ENTER)){
                    interagirComAmbiente(&jogador, itensChao, qtdItensChao, faseAtual, mapa, linhas, colunas);
                }

                // Se o jogador pisou numa porta, troca de fase e recebe o novo mapa.
                mapa = checarTransicaoDeFase(mapa, &linhas, &colunas, &jogador, &faseAtual);

                // A câmera segue o jogador, sem sair das bordas do mapa.
                atualizarCamera(&camera, jogador.pos, jogador.tamanho, larguraTela, alturaTela, linhas, colunas);

                // Se o jogador tocou no inimigo (vivo, na fase 1), começa o combate.
                if (inimigo.ativo && faseAtual == 1 && jogadorTocaInimigo(&jogador, &inimigo)){
                    estadoAtual = ESTADO_COMBATE;
                }

                // ESC no mapa abre a pausa
                if (IsKeyPressed(KEY_ESCAPE)){
                estadoAtual = ESTADO_PAUSA;
                opcaoPausa = 0; // Sempre abre com "Retomar" selecionado
                }
                break;

            case ESTADO_PAUSA:
                // Menu de pausa: Retomar / Voltar ao Menu Inicial
                AcaoMenu acaoPausa = atualizarMenuPausa(&opcaoPausa);
                if (acaoPausa == ACAO_RETOMAR){
                    estadoAtual = ESTADO_EXPLORACAO;
                } else if (acaoPausa == ACAO_VOLTAR_MENU){
                    estadoAtual = ESTADO_MENU;
                    opcaoMenu = 0; // Sempre abre com "Novo Jogo" selecionado
                }
                break;

            case ESTADO_INVENTARIO:
                // Navegação do menu (setas, ENTER, ESC, TAB).
                atualizarMenuInventario(&jogador, &menuInventario, itensChao, &qtdItensChao, faseAtual, &estadoAtual);
                break;

            case ESTADO_COMBATE: {
                // Roda um quadro do combate e reage ao resultado.
                ResultadoCombate resultado = atualizarCombate(&combate, &jogador, &inimigo);

                if (resultado == COMBATE_DERROTA){
                    estadoAtual = ESTADO_GAMEOVER;
                } else if (resultado == COMBATE_VITORIA){
                    // Inimigo derrotado: some do mapa, o jogo volta a explorar e a chave cai onde ele estava.
                    inimigo.ativo = false;
                    estadoAtual = ESTADO_EXPLORACAO;
                    soltarItemNoChao(itensChao, &qtdItensChao,
                                     criarItem("Chave Dourada", ITEM_CHAVE, 0, "Uma chave dourada.\nParece se encaixar na porta final."),
                                     inimigo.pos, faseAtual);
                }
                break;
            }

            case ESTADO_GAMEOVER:
                 // ESC no game over volta ao menu inicial
                if (IsKeyPressed(KEY_ESCAPE)){
                    estadoAtual = ESTADO_MENU;
                    opcaoMenu = 0;
                }
                break;
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        switch (estadoAtual){

            case ESTADO_MENU:
                desenharMenuInicial(opcaoMenu, larguraTela, alturaTela);
                break;

            case ESTADO_EXPLORACAO:
            case ESTADO_PAUSA:
                // Desenha o mapa, o jogador, os itens no chão e o inimigo (se estiver vivo e na fase 1). Tudo é movido pela câmera.
                // A câmera é atualizada antes de desenhar, então o jogador sempre fica no centro da tela (exceto nas bordas do mapa).
                // Tudo entre BeginMode2D e EndMode2D usa coordenadas do mapa e é movido pela câmera.
                BeginMode2D(camera);
                    desenharMapa(mapa,  linhas, colunas, tilesetMapa, texParede); // adicionado recentemente
                    desenharItensNoChao(itensChao, qtdItensChao, faseAtual);
                    desenharJogador(&jogador);
                    if (inimigo.ativo && faseAtual == 1) desenharInimigo(&inimigo);
                EndMode2D();
                if (estadoAtual == ESTADO_PAUSA) desenharMenuPausa(opcaoPausa, larguraTela, alturaTela);
                break;

            case ESTADO_COMBATE:
                desenharCombate(&combate, &jogador, &inimigo);
                break;

            case ESTADO_GAMEOVER:
                desenharTextoCentralizado("GAME OVER!", 250, 40, RED);
                desenharTextoCentralizado("Você foi reprovado...", 300, 20, DARKGRAY);
                break;

            case ESTADO_INVENTARIO:
                desenharMenuInventario(&jogador, &menuInventario, larguraTela, alturaTela);
                break;
        }

        EndDrawing();
    }

    if (mapa != NULL){
        liberarMapa(mapa, linhas);
    }
    liberarCombate(&combate);
    UnloadTexture(jogador.textura);
    UnloadTexture(inimigo.textura);
    UnloadTexture(tilesetMapa); // adicionado recentemente
    UnloadTexture(texParede);   // adicionado recentemente
    descarregarFonteUI();
    CloseWindow();
    return 0;
}
