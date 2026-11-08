// Combate.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "combate.h"
#include "ui.h"

// FUNÇÃO: definirPergunta (uso interno deste arquivo)
static void definirPergunta(Pergunta *p, const char *texto, const char *op0, const char *op1, const char *op2, const char *op3, int respostaCorreta){
    snprintf(p->texto, sizeof(p->texto), "%s", texto);
    snprintf(p->opcoes[0], sizeof(p->opcoes[0]), "%s", op0);
    snprintf(p->opcoes[1], sizeof(p->opcoes[1]), "%s", op1);
    snprintf(p->opcoes[2], sizeof(p->opcoes[2]), "%s", op2);
    snprintf(p->opcoes[3], sizeof(p->opcoes[3], "%s", op3);
    p->respostaCorreta = respostaCorreta;

    // Garante que todas as opções comecem visíveis.
    for (int k = 0; k < 4; k++) p->opcaoOculta[k] = false;
}

// FUNÇÃO: criarBancoPerguntas (Lendo de arquivo .txt conforme pedido do colega)
Pergunta* criarBancoPerguntas(int *qtdPerguntas){
    // Tenta abrir o arquivo de perguntas. Certifique-se de criar um "perguntas.txt" na pasta do projeto.
    FILE *arquivo = fopen("perguntas.txt", "r");
    
    if (arquivo == NULL) {
        // Fallback caso o arquivo não exista ainda para não quebrar o jogo nos testes
        *qtdPerguntas = 3;
        Pergunta *banco = (Pergunta*) malloc((*qtdPerguntas) * sizeof(Pergunta));
        if (banco == NULL) return NULL;

        definirPergunta(&banco[0], "Qual estrutura segue o concepto FIFO?", "1) Pilha", "2) Fila", "3) Árvore", "4) Grafo", 1);
        definirPergunta(&banco[1], "Qual funcao usamos para alocar memoria em C?", "1) malloc", "2) printf", "3) scanf", "4) sizeof", 0);
        definirPergunta(&banco[2], "Quantos bits tem 1 Byte?", "1) 4 bits", "2) 16 bits", "3) 8 bits", "4) 32 bits", 2);
        return banco;
    }

    // Lê a quantidade de perguntas na primeira linha do txt
    if (fscanf(arquivo, "%d", qtdPerguntas) != 1) {
        *qtdPerguntas = 1;
    }

    Pergunta *banco = (Pergunta*) malloc((*qtdPerguntas) * sizeof(Pergunta));
    if (banco == NULL){
        fclose(arquivo);
        return NULL;
    }

    char buffer[256];
    fgets(buffer, sizeof(buffer), arquivo); // Limpa o buffer da quebra de linha

    for (int i = 0; i < *qtdPerguntas; i++) {
        char t[200], o0[100], o1[100], o2[100], o3[100];
        int resp;

        // Formato esperado no txt por bloco: Enunciado | Opção0 | Opção1 | Opção2 | Opção3 | RespostaCorreta
        // Ou você pode adaptar o formato de leitura conforme preferirem estruturar o txt.
        if (fgets(t, sizeof(t), arquivo)) {
            t[strcspn(t, "\r\n")] = 0; // Remove quebra de linha
            fgets(o0, sizeof(o0), arquivo); o0[strcspn(o0, "\r\n")] = 0;
            fgets(o1, sizeof(o1), arquivo); o1[strcspn(o1, "\r\n")] = 0;
            fgets(o2, sizeof(o2), arquivo); o2[strcspn(o2, "\r\n")] = 0;
            fgets(o3, sizeof(o3), arquivo); o3[strcspn(o3, "\r\n")] = 0;
            fscanf(arquivo, "%d\n", &resp);

            definirPergunta(&banco[i], t, o0, o1, o2, o3, resp);
        }
    }

    fclose(arquivo);
    return banco;
}

// FUNÇÃO: liberarBancoPerguntas
void liberarBancoPerguntas(Pergunta *banco){
    free(banco);
}

// FUNÇÃO: inicializarCombate
bool inicializarCombate(EstadoCombate *c){
    c->banco = criarBancoPerguntas(&c->qtdPerguntas);
    c->perguntaAtual = 0;
    c->tempoMensagem = 0.0f;
    c->tempoRestante = TEMPO_LIMITE_PERGUNTA; // Inicializa o cronômetro com 10 segundos
    c->mensagem[0] = '\0';
    c->corMensagem = BLANK;
    c->mudarPergunta = false;
    return c->banco != NULL;
}

// FUNÇÃO: liberarCombate
void liberarCombate(EstadoCombate *c){
    liberarBancoPerguntas(c->banco);
    c->banco = NULL;
}

// FUNÇÃO: resolverTurno
bool resolverTurno(Jogador *j, Inimigo *i, EstadoCombate *c, int escolha){
    Pergunta *p = &c->banco[c->perguntaAtual];

    if (escolha == p->respostaCorreta){
        i->hp -= DANO_POR_TURNO;
        if (i->hp < 0) i->hp = 0;
        j->score += 10;

        snprintf(c->mensagem, sizeof(c->mensagem), "%s", "RESPOSTA CORRETA! Dano no Bug de C!");
        c->corMensagem = GREEN;
        c->mudarPergunta = true; 
    } else {
        j->hp -= DANO_POR_TURNO;
        if (j->hp < 0) j->hp = 0;
        p->opcaoOculta[escolha] = true;

        snprintf(c->mensagem, sizeof(c->mensagem), "%s", "RESPOSTA ERRADA! Danos na Caloura!");
        c->corMensagem = RED;
    }

    c->tempoMensagem = TEMPO_MENSAGEM;
    return (j->hp == 0 || i->hp == 0);
}

// FUNÇÃO: lerEscolhaTeclado
static int lerEscolhaTeclado(void){
    if (IsKeyPressed(KEY_ONE))   return 0;
    if (IsKeyPressed(KEY_TWO))   return 1;
    if (IsKeyPressed(KEY_THREE)) return 2;
    if (IsKeyPressed(KEY_FOUR))  return 3;
    return -1;
}

// FUNÇÃO: proximaPergunta
static void proximaPergunta(EstadoCombate *c){
    c->perguntaAtual = (c->perguntaAtual + 1) % c->qtdPerguntas;
    for (int k = 0; k < 4; k++) c->banco[c->perguntaAtual].opcaoOculta[k] = false;
    c->tempoRestante = TEMPO_LIMITE_PERGUNTA; // Reseta o cronômetro para os próximos 10 segundos
    c->mudarPergunta = false;
}

// FUNÇÃO: atualizarCombate (Controla o tempo limite de resposta e as mensagens)
ResultadoCombate atualizarCombate(EstadoCombate *c, Jogador *j, Inimigo *i){

    // Mostrando o resultado da resposta.
    if (c->tempoMensagem > 0){
        c->tempoMensagem -= GetFrameTime();

        if (c->tempoMensagem <= 0){
            if (j->hp <= 0) return COMBATE_DERROTA;
            if (i->hp <= 0){
                c->mudarPergunta = false;
                return COMBATE_VITORIA;
            }
            if (c->mudarPergunta) proximaPergunta(c);
        }
        return COMBATE_EM_ANDAMENTO;
    }

    // Contagem regressiva do tempo para responder (Cronômetro de 10 segundos)
    c->tempoRestante -= GetFrameTime();
    if (c->tempoRestante <= 0) {
        // Se o tempo esgotar: Jogador recebe dano e passa para a pergunta seguinte
        j->hp -= DANO_POR_TURNO;
        if (j->hp < 0) j->hp = 0;

        snprintf(c->mensagem, sizeof(c->mensagem), "%s", "TEMPO ESGOTADO! Dano na Caloura!");
        c->corMensagem = RED;
        c->tempoMensagem = TEMPO_MENSAGEM;
        c->mudarPergunta = true;

        if (j->hp == 0) return COMBATE_DERROTA;
        return COMBATE_EM_ANDAMENTO;
    }

    // Esperando o jogador responder.
    int escolha = lerEscolhaTeclado();
    Pergunta *p = &c->banco[c->perguntaAtual];

    if (escolha != -1 && !p->opcaoOculta[escolha]){
        resolverTurno(j, i, c, escolha);
    }
    return COMBATE_EM_ANDAMENTO;
}

// FUNÇÃO: desenharInterfaceCombate
static void desenharInterfaceCombate(const EstadoCombate *c, const Pergunta *p, const Jogador *j, const Inimigo *i){
    int L = GetScreenWidth(), A = GetScreenHeight();
    int margem = L / 16;

    desenharTexto(TextFormat("Jogador: %s | HP: %d | Score: %d", j->nome, j->hp, j->score), margem, 40, 20, GREEN);
    
    // Mostra o cronômetro na tela para o jogador ver o tempo correndo
    char txtTempo[30];
    snprintf(txtTempo, sizeof(txtTempo), "Tempo: %.1f s", c->tempoRestante > 0 ? c->tempoRestante : 0.0f);
    desenharTexto(txtTempo, L / 2 - 50, 40, 20, c->tempoRestante <= 3.0f ? RED : YELLOW);

    const char *txtInimigo = TextFormat("Bug de C: %s | HP: %d", i->nome, i->hp);
    desenharTexto(txtInimigo, L - margem - medirTexto(txtInimigo, 20), 40, 20, RED);

    int cx = margem, cw = L - 2 * margem, ch = 250, cy = A - ch - margem / 2;
    
    int areaTopo = 70;
    int areaAltura = cy - areaTopo - 10;
    float escala = (float)areaAltura / i->textura.height;
    float larguraImg = i->textura.width * escala;
    Rectangle origem  = { 0, 0, (float)i->textura.width, (float)i->textura.height };
    Rectangle destino = { (L - larguraImg) / 2, (float)areaTopo, larguraImg, (float)areaAltura };
    DrawTexturePro(i->textura, origem, destino, (Vector2){ 0, 0 }, 0.0f, WHITE);
    
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

// FUNÇÃO: desenharCombate
void desenharCombate(const EstadoCombate *c, const Jogador *j, const Inimigo *i){
    desenharInterfaceCombate(c, &c->banco[c->perguntaAtual], j, i);

    if (c->tempoMensagem > 0){
        DrawRectangle(0, 200, GetScreenWidth(), 80, Fade(BLACK, 0.7f));
        desenharTextoCentralizado(c->mensagem, 225, 30, c->corMensagem);
    }
}
