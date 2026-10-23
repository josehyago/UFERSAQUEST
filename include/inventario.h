// inventario.h
#ifndef INVENTARIO_H
#define INVENTARIO_H

#include "raylib.h"
#include "mapa.h"
#include "tipos.h"


#define MAX_ITENS_CHAO 50
#define TAMANHO_ITEM 50.0f

// Estados possíveis do jogo
typedef enum { ESTADO_EXPLORACAO, ESTADO_INVENTARIO, ESTADO_COMBATE, ESTADO_GAMEOVER } EstadoJogo;

// Enumeração para classificar o que cada item faz no jogo.
// Isso ajuda o sistema a saber se o item cura, se abre porta ou se é texto.
typedef enum { ITEM_CURA, ITEM_CHAVE, ITEM_MANUAL } TipoItem;

// Struct para itens do jogo
typedef struct{
    char nome[30]; // Nome do item que vai aparecer na tela (ex: "Coxinha")
    TipoItem tipo; // A categoria dele (baseado no enum acima)
    int poder; // Um valor numérico (Se for cura = quanto de HP recupera)
} Item;

// Struct para gerenciar os itens que ficam fisicamente no mapa
typedef struct{
    Item item; // Qual é o item real (traz o nome, tipo e poder)
    Vector2 pos; // Posição X e Y no mapa (usando Vector2 do Raylib)
    int fase; // Em qual sala/fase esse item está (para não aparecer em todas)
    bool ativo; // true = está no chão. false = alguém pegou, não deve ser desenhado.
} ItemNoChao;

// Struct que agrupa as variáveis de controle do menu
typedef struct{
    int itemSelecionado; // Qual slot (0 a 9) a seta amarela está apontando
    bool menuOpcoesAberto; // true = abriu a janelinha lateral "Usar / Examinar / Descartar"
    int opcaoSelecionada; // Controla a seta dentro da janelinha lateral (0, 1 ou 2)
    bool lendoManual; // true = trava a tela com o texto grande do manual
    bool examinandoItemGeral; // true = trava a tela mostrando a descrição de um item comum
    char textoManual[2048]; // Um vetor gigante para guardar o texto lido do arquivo .txt
} EstadoMenuInventario;

// Funções do inventário
bool adicionarItem(Jogador *j, Item novoItem);
bool soltarItemNoChao(ItemNoChao *itens, int *qtd, Item item, Vector2 pos, int fase);
void usarItem(Jogador *j, int posicao, EstadoMenuInventario *menuState);
void removerItem(Jogador *j, int posicao);
bool temItem(Jogador *j, TipoItem tipo);
void removerItemPorTipo(Jogador *j, TipoItem tipo);
void interagirComAmbiente(Jogador *jogador, ItemNoChao *itensChao, int *qtdItensChao, int faseAtual, int **mapa, int linhas, int colunas);
void atualizarMenuInventario(Jogador *jogador, EstadoMenuInventario *menu, ItemNoChao *itensChao, int *qtdItensChao, int faseAtual, EstadoJogo *estadoAtual);
void desenharMenuInventario(Jogador *jogador, EstadoMenuInventario *menu, int larguraTela, int alturaTela);

#endif