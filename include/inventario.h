// inventario.h
#ifndef INVENTARIO_H
#define INVENTARIO_H

#include "raylib.h"
#include "mapa.h"
#include "tipos.h"

#define MAX_MOCHILA 10        // quantidade de slots da mochila do jogador
#define MAX_ITENS_CHAO 50     // quantidade máxima de itens espalhados pelo mapa
#define TAMANHO_ITEM 50.0f    // lado do quadrado do item no mapa (menor que o tile de 100px)

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
    char descricao[120]; // Texto mostrado na opção "Examinar" do menu
} Item;

// Struct para gerenciar os itens que ficam fisicamente no mapa
typedef struct{
    Item item; // Qual é o item real (traz o nome, tipo,poder e descrição)
    Vector2 pos; // Canto do "tile" do item; o quadrado de TAMANHO_ITEM é centralizado dentro dele
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
    char textoManual[2048]; // Guarda o texto lido do arquivo manual.txt
} EstadoMenuInventario;

// Criação e aparência dos itens
Item criarItem(const char *nome, TipoItem tipo, int poder, const char *descricao);
Rectangle obterRetanguloItem(const ItemNoChao *it);
Color corDoItem(TipoItem tipo);

// Mochila do jogador
bool adicionarItem(Jogador *j, Item novoItem);
void removerItem(Jogador *j, int posicao);
void usarItem(Jogador *j, int posicao);
bool temItem(const Jogador *j, TipoItem tipo);
void removerItemPorTipo(Jogador *j, TipoItem tipo);

// Itens no chão e interação com o ambiente
bool soltarItemNoChao(ItemNoChao *itens, int *qtd, Item item, Vector2 pos, int fase);
void interagirComAmbiente(Jogador *jogador, ItemNoChao *itensChao, int qtdItensChao, int faseAtual, int **mapa, int linhas, int colunas);
void desenharItensNoChao(const ItemNoChao *itens, int qtd, int fase);

// Menu do inventário
void carregarTextoManual(EstadoMenuInventario *menu, const char *caminho);
void atualizarMenuInventario(Jogador *jogador, EstadoMenuInventario *menu, ItemNoChao *itensChao, int *qtdItensChao, int faseAtual, EstadoJogo *estadoAtual);
void desenharMenuInventario(Jogador *jogador, EstadoMenuInventario *menu, int larguraTela, int alturaTela);

#endif