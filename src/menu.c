// menu.c
#include "menu.h"
#include "ui.h"

// FUNÇÃO (interna): navegarOpcoes
// OBJETIVO: Mover a seleção com as setas/W/S (com loop) e avisar se apertou ENTER.
static bool navegarOpcoes(int *opcao, int qtdOpcoes){
    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) (*opcao)++;
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) (*opcao)--;

    // Loop cíclico: passou do último volta pro primeiro e vice-versa
    if (*opcao < 0) *opcao = qtdOpcoes - 1;
    if (*opcao >= qtdOpcoes) *opcao = 0;

    return IsKeyPressed(KEY_ENTER);
}

// FUNÇÃO (interna): desenharOpcoes
// OBJETIVO: Desenhar título + lista de opções centralizadas. Serve para os dois menus.
static void desenharOpcoes(const char *titulo, const char *opcoes[], int qtdOpcoes, int selecionada, int alturaTela){
    desenharTextoCentralizado(titulo, alturaTela / 4, 50, GOLD);

    for (int i = 0; i < qtdOpcoes; i++){
        const char *texto = opcoes[i];
        Color cor = WHITE;

        if (i == selecionada){
            texto = TextFormat("> %s <", opcoes[i]); // Destaca a opção atual
            cor = YELLOW;
        }

        desenharTextoCentralizado(texto, alturaTela / 2 + i * 50, 30, cor);
    }

    const char *dica = "Setas ou W/S para navegar - ENTER para selecionar";
    desenharTextoCentralizado(dica, alturaTela - 40, 16, LIGHTGRAY);
}

// MENU INICIAL

AcaoMenu atualizarMenuInicial(int *opcao){
    if (navegarOpcoes(opcao, 2)){
        if (*opcao == 0) return ACAO_NOVO_JOGO;
        if (*opcao == 1) return ACAO_SAIR;
    }
    return ACAO_NENHUMA;
}

void desenharMenuInicial(int opcao, int larguraTela, int alturaTela){
    const char *opcoes[] = { "Novo Jogo", "Sair do Jogo" };

    DrawRectangle(0, 0, larguraTela, alturaTela, DARKBLUE); // Fundo do menu
    desenharOpcoes("UFERSAQUEST", opcoes, 2, opcao, alturaTela);
}

// MENU DE PAUSA

AcaoMenu atualizarMenuPausa(int *opcao){
    // ESC também retoma o jogo (comportamento comum em jogos)
    if (IsKeyPressed(KEY_ESCAPE)) return ACAO_RETOMAR;

    if (navegarOpcoes(opcao, 2)){
        if (*opcao == 0) return ACAO_RETOMAR;
        if (*opcao == 1) return ACAO_VOLTAR_MENU;
    }
    return ACAO_NENHUMA;
}

void desenharMenuPausa(int opcao, int larguraTela, int alturaTela){
    const char *opcoes[] = { "Retomar", "Voltar ao Menu Inicial" };

    // Película escura por cima do jogo, que continua visível "congelado" por baixo
    DrawRectangle(0, 0, larguraTela, alturaTela, Fade(BLACK, 0.7f));
    desenharOpcoes("PAUSA", opcoes, 2, opcao, alturaTela);
}
