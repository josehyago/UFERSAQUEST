// Combate.h
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "combate.h"
#include "ui.h"

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

void desenharInterfaceCombate(const Pergunta *p, const Jogador *j, const Inimigo *i){
    int L = GetScreenWidth(), A = GetScreenHeight();
    int margem = L / 16;

    desenharTexto(TextFormat("Jogador: %s | HP: %d | Score: %d", j->nome, j->hp, j->score), margem, 40, 20, GREEN);
    const char *txtInimigo = TextFormat("Chefe: %s | HP: %d", i->nome, i->hp);
    desenharTexto(txtInimigo, L - margem - medirTexto(txtInimigo, 20), 40, 20, RED);

    int cx = margem, cw = L - 2 * margem, ch = 250, cy = A - ch - margem / 2;
    DrawRectangle(cx, cy, cw, ch, LIGHTGRAY);
    DrawRectangleLines(cx, cy, cw, ch, DARKGRAY);

    desenharTexto(p->texto, cx + 20, cy + 20, 20, BLACK);

    int col2 = cx + cw / 2;
    if (!p->opcaoOculta[0]) desenharTexto(p->opcoes[0], cx + 30, cy + 80,  18, DARKBLUE);
    if (!p->opcaoOculta[1]) desenharTexto(p->opcoes[1], col2,    cy + 80,  18, DARKBLUE);
    if (!p->opcaoOculta[2]) desenharTexto(p->opcoes[2], cx + 30, cy + 140, 18, DARKBLUE);
    if (!p->opcaoOculta[3]) desenharTexto(p->opcoes[3], col2,    cy + 140, 18, DARKBLUE);

    desenharTexto("Pressione as teclas (1, 2, 3 ou 4) para responder!", cx + 20, cy + ch - 40, 15, DARKGRAY);
}