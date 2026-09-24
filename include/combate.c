#include "combate.h"

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