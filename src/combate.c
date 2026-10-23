// Combate.h
#include <stdio.h>
#include <stdlib.h>
#include "combate.h"
#include "ui.h"

// FUNÇÃO: definirPergunta (uso interno deste arquivo)
// PARÂMETROS: p - pergunta a preencher; texto - enunciado; op0 a op3 - as quatro alternativas;
//             respostaCorreta - índice (0 a 3) da alternativa certa.
// POR QUE: cada pergunta repetia cinco strcpy; agora é uma chamada só, e snprintf respeita o tamanho dos vetores.
// PARA QUE SERVE: preenche uma Pergunta e deixa todas as alternativas visíveis.
static void definirPergunta(Pergunta *p, const char *texto, const char *op0, const char *op1, const char *op2, const char *op3, int respostaCorreta){
    snprintf(p->texto, sizeof(p->texto), "%s", texto);
    snprintf(p->opcoes[0], sizeof(p->opcoes[0]), "%s", op0);
    snprintf(p->opcoes[1], sizeof(p->opcoes[1]), "%s", op1);
    snprintf(p->opcoes[2], sizeof(p->opcoes[2]), "%s", op2);
    snprintf(p->opcoes[3], sizeof(p->opcoes[3]), "%s", op3);
    p->respostaCorreta = respostaCorreta;

    // Garante que todas as opções comecem visíveis.
    for (int k = 0; k < 4; k++) p->opcaoOculta[k] = false;
}

// FUNÇÃO: criarBancoPerguntas
// PARÂMETROS: qtdPerguntas - ponteiro que recebe quantas perguntas foram criadas (parâmetro de saída).
// POR QUE: o banco fica no heap (malloc), então pode crescer no futuro (ex: ler perguntas de arquivo).
// PARA QUE SERVE: aloca dinamicamente o vetor de perguntas e o preenche. RETORNA o vetor, ou NULL se faltar memória. Quem chamar deve liberar com liberarBancoPerguntas.
Pergunta* criarBancoPerguntas(int *qtdPerguntas){
    *qtdPerguntas = 3; // quantidade de perguntas de disciplinas

    // Pede memória para o vetor de structs.
    Pergunta *banco = (Pergunta*) malloc((*qtdPerguntas) * sizeof(Pergunta));
    if (banco == NULL){
        printf("Erro ao alocar memoria para as perguntas!\n");
        return NULL;
    }

    // Pergunta 1: Algoritmos (resposta: Fila, índice 1)
    definirPergunta(&banco[0], "Qual estrutura segue o conceito FIFO (First In, First Out)?",
                    "1) Pilha", "2) Fila", "3) Árvore", "4) Grafo", 1);

    // Pergunta 2: AED 1 (resposta: malloc, índice 0)
    definirPergunta(&banco[1], "Qual função usamos para alocar memória dinamicamente em C?",
                    "1) malloc", "2) printf", "3) scanf", "4) sizeof", 0);

    // Pergunta 3: Computação (resposta: 8 bits, índice 2)
    definirPergunta(&banco[2], "Quantos bits tem 1 Byte?",
                    "1) 4 bits", "2) 16 bits", "3) 8 bits", "4) 32 bits", 2);

    return banco;
}

// FUNÇÃO: liberarBancoPerguntas
// PARÂMETROS: banco - vetor criado por criarBancoPerguntas.
// POR QUE: memória alocada com malloc precisa de free.
// PARA QUE SERVE: devolve a memória do banco de perguntas.
void liberarBancoPerguntas(Pergunta *banco){
    free(banco);
}

// FUNÇÃO: inicializarCombate
// PARÂMETROS: c - estado do combate a preencher.
// POR QUE: antes, essas variáveis ficavam soltas na main; agora ficam juntas numa struct.
// PARA QUE SERVE: cria o banco de perguntas e zera pergunta atual, temporizador e mensagem. RETORNA false se não conseguiu criar o banco.
bool inicializarCombate(EstadoCombate *c){
    c->banco = criarBancoPerguntas(&c->qtdPerguntas);
    c->perguntaAtual = 0;
    c->tempoMensagem = 0.0f;
    c->mensagem[0] = '\0';
    c->corMensagem = BLANK;
    c->mudarPergunta = false;
    return c->banco != NULL;
}

// FUNÇÃO: liberarCombate
// PARÂMETROS: c - estado do combate.
// POR QUE: o banco de perguntas é memória dinâmica.
// PARA QUE SERVE: libera o banco e zera o ponteiro, evitando uso de memória já liberada.
void liberarCombate(EstadoCombate *c){
    liberarBancoPerguntas(c->banco);
    c->banco = NULL;
}

// FUNÇÃO: resolverTurno
// PARÂMETROS: j - jogador; i - inimigo; c - estado do combate; escolha - alternativa escolhida (0 a 3).
// POR QUE: acertou, o inimigo perde vida; errou, o jogador perde.
// PARA QUE SERVE: aplica o dano, atualiza o score, esconde a alternativa errada, prepara a mensagem e liga o temporizador de exibição. RETORNA true se a batalha acabou (alguém chegou a 0 HP).
bool resolverTurno(Jogador *j, Inimigo *i, EstadoCombate *c, int escolha){
    Pergunta *p = &c->banco[c->perguntaAtual];

    if (escolha == p->respostaCorreta){
        // Acertou: o inimigo perde vida e o jogador ganha pontos.
        i->hp -= DANO_POR_TURNO;
        if (i->hp < 0) i->hp = 0;
        j->score += 10;

        snprintf(c->mensagem, sizeof(c->mensagem), "%s", "RESPOSTA CORRETA! Dano no chefe!");
        c->corMensagem = GREEN;
        c->mudarPergunta = true; // depois da mensagem, avança para a próxima pergunta
    } else {
        // Errou: o jogador perde vida e a alternativa escolhida some.
        j->hp -= DANO_POR_TURNO;
        if (j->hp < 0) j->hp = 0;
        p->opcaoOculta[escolha] = true;

        snprintf(c->mensagem, sizeof(c->mensagem), "%s", "RESPOSTA ERRADA! Você levou dano.");
        c->corMensagem = RED;
    }

    // Liga a pausa para o jogador ler o resultado.
    c->tempoMensagem = TEMPO_MENSAGEM;

    return (j->hp == 0 || i->hp == 0);
}

// FUNÇÃO: lerEscolhaTeclado (uso interno deste arquivo)
// PARÂMETROS: nenhum.
// POR QUE: separa a leitura das teclas da lógica do combate.
// PARA QUE SERVE: RETORNA 0 a 3 conforme a tecla 1, 2, 3 ou 4 pressionada neste quadro, ou -1 se nenhuma.
static int lerEscolhaTeclado(void){
    if (IsKeyPressed(KEY_ONE))   return 0;
    if (IsKeyPressed(KEY_TWO))   return 1;
    if (IsKeyPressed(KEY_THREE)) return 2;
    if (IsKeyPressed(KEY_FOUR))  return 3;
    return -1;
}

// FUNÇÃO: proximaPergunta (uso interno deste arquivo)
// PARÂMETROS: c - estado do combate.
// POR QUE: ao acertar, o jogador deve enfrentar uma pergunta nova, sem alternativas escondidas.
// PARA QUE SERVE: avança para a próxima pergunta (voltando à primeira após a última) e a deixa com todas as alternativas visíveis.
static void proximaPergunta(EstadoCombate *c){
    c->perguntaAtual = (c->perguntaAtual + 1) % c->qtdPerguntas; // o % faz a volta ao início
    for (int k = 0; k < 4; k++) c->banco[c->perguntaAtual].opcaoOculta[k] = false;
    c->mudarPergunta = false;
}

// FUNÇÃO: atualizarCombate
// PARÂMETROS: c - estado do combate; j - jogador; i - inimigo.
// POR QUE: tira da main toda a lógica do combate; a main só reage ao resultado.
// PARA QUE SERVE: chamada uma vez por quadro durante o combate. Enquanto a mensagem de resultado está na tela, só conta o tempo; quando acaba, decide o que acontece. Fora da pausa, espera a tecla de resposta. RETORNA COMBATE_VITORIA, COMBATE_DERROTA ou COMBATE_EM_ANDAMENTO.
ResultadoCombate atualizarCombate(EstadoCombate *c, Jogador *j, Inimigo *i){

    // Mostrando o resultado da resposta.
    if (c->tempoMensagem > 0){
        c->tempoMensagem -= GetFrameTime();

        // Quando o tempo acaba, decide o próximo passo (a ordem importa: derrota vem primeiro).
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

    // Esperando o jogador responder.
    int escolha = lerEscolhaTeclado();
    Pergunta *p = &c->banco[c->perguntaAtual];

    // Só processa tecla válida e de alternativa que ainda não foi escondida.
    if (escolha != -1 && !p->opcaoOculta[escolha]){
        resolverTurno(j, i, c, escolha);
    }
    return COMBATE_EM_ANDAMENTO;
}

// FUNÇÃO: desenharInterfaceCombate (uso interno deste arquivo)
// PARÂMETROS: p - pergunta atual; j - jogador; i - inimigo (todos somente leitura).
// POR QUE: separa o desenho da tela de combate da lógica.
// PARA QUE SERVE: desenha a vida dos dois lados, a caixa da pergunta e as alternativas visíveis. As posições são calculadas pelo tamanho real da janela.
static void desenharInterfaceCombate(const Pergunta *p, const Jogador *j, const Inimigo *i){
    int L = GetScreenWidth(), A = GetScreenHeight();
    int margem = L / 16;

    // Status do jogador (esquerda, verde) e do inimigo (direita, vermelho).
    desenharTexto(TextFormat("Jogador: %s | HP: %d | Score: %d", j->nome, j->hp, j->score), margem, 40, 20, GREEN);
    const char *txtInimigo = TextFormat("Chefe: %s | HP: %d", i->nome, i->hp);
    desenharTexto(txtInimigo, L - margem - medirTexto(txtInimigo, 20), 40, 20, RED);

    // Caixa de fundo da pergunta, encostada na parte de baixo da tela.
    int cx = margem, cw = L - 2 * margem, ch = 250, cy = A - ch - margem / 2;
    DrawRectangle(cx, cy, cw, ch, LIGHTGRAY);
    DrawRectangleLines(cx, cy, cw, ch, DARKGRAY);

    // Enunciado da pergunta.
    desenharTexto(p->texto, cx + 20, cy + 20, 20, BLACK);

    // Quatro alternativas em duas colunas; as escondidas (erradas) não são desenhadas.
    int col2 = cx + cw / 2;
    if (!p->opcaoOculta[0]) desenharTexto(p->opcoes[0], cx + 30, cy + 80,  18, DARKBLUE);
    if (!p->opcaoOculta[1]) desenharTexto(p->opcoes[1], col2,    cy + 80,  18, DARKBLUE);
    if (!p->opcaoOculta[2]) desenharTexto(p->opcoes[2], cx + 30, cy + 140, 18, DARKBLUE);
    if (!p->opcaoOculta[3]) desenharTexto(p->opcoes[3], col2,    cy + 140, 18, DARKBLUE);

    desenharTexto("Pressione as teclas (1, 2, 3 ou 4) para responder!", cx + 20, cy + ch - 40, 15, DARKGRAY);
}

// FUNÇÃO: desenharCombate
// PARÂMETROS: c - estado do combate; j - jogador; i - inimigo (todos somente leitura).
// POR QUE: a main só precisa chamar uma função para desenhar o combate inteiro.
// PARA QUE SERVE: desenha a interface do combate e, durante a pausa, a faixa escura com o resultado da resposta.
void desenharCombate(const EstadoCombate *c, const Jogador *j, const Inimigo *i){
    desenharInterfaceCombate(&c->banco[c->perguntaAtual], j, i);

    // Enquanto o temporizador estiver ativo, mostra a faixa com a mensagem centralizada.
    if (c->tempoMensagem > 0){
        DrawRectangle(0, 200, GetScreenWidth(), 80, Fade(BLACK, 0.7f));
        desenharTextoCentralizado(c->mensagem, 225, 30, c->corMensagem);
    }
}