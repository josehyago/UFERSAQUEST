// menu.h
#ifndef MENU_H
#define MENU_H

#include "raylib.h"

typedef enum {
    ACAO_NENHUMA,
    ACAO_NOVO_JOGO,
    ACAO_SAIR,
    ACAO_RETOMAR,
    ACAO_VOLTAR_MENU
} AcaoMenu;

// Menu inicial: Novo Jogo / Sair do Jogo
AcaoMenu atualizarMenuInicial(int *opcao);
void desenharMenuInicial(int opcao, int larguraTela, int alturaTela);

// Menu de pausa: Retomar / Voltar ao Menu Inicial
AcaoMenu atualizarMenuPausa(int *opcao);
void desenharMenuPausa(int opcao, int larguraTela, int alturaTela);

#endif
