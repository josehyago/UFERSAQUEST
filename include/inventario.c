#include "ufersaquest.h"

// NOVO: Enumerador para os tipos de item
typedef enum { ITEM_CURA, ITEM_CHAVE, ITEM_MANUAL } TipoItem;

// NOVO: Struct para itens do jogo
typedef struct {
    char nome[30];
    TipoItem tipo;
    int poder; // Se for cura, quanto de HP recupera.
} Item;

// AVISO DE SUBSTITUIÇÃO: A struct Jogador mudou para incluir o inventário!
typedef struct{
    char nome[20];
    int hp; 
    int score; 
    Vector2 pos; 
    float tamanho; 
    float velocidade; 
    
    // NOVIDADE: O jogador agora tem um vetor de Structs como Inventário.
    Item inventario[10]; // Mochila com 10 espaços máximos.
    int qtdItens; // Controle de quantos itens ele já pegou.
} Jogador;

// A struct Inimigo continua igual.

// Na função inicializarJogador que já existe, vocês precisam adicionar:
// j->qtdItens = 0; (Para garantir que a mochila começa vazia).
void inicializarJogador(Jogador *j, Vector2 posInicial);
void inicializarInimigo(Inimigo *i, Vector2 posInicial, const char *nome);
void moverJogador(Jogador *j, int mapa[25][35]); // (Lembrar de alterar para mapa dinâmico depois)

// NOVAS FUNÇÕES DO INVENTÁRIO

// FUNÇÃO: adicionarItem
// OBJETIVO: Pegar um item do chão e colocar no vetor inventario do jogador.
bool adicionarItem(Jogador *j, Item novoItem){
    // 1. Checar se a mochila está cheia verificando se (j->qtdItens >= 10).
    // 2. Se estiver cheia, retornar 'false' (não conseguiu pegar).
    // 3. Se tiver espaço, colocar o parâmetro 'novoItem' na posição livre do vetor: j->inventario[j->qtdItens] = novoItem;
    // 4. Aumentar a quantidade de itens: j->qtdItens++;
    // 5. Retornar 'true' (item pego com sucesso).
}

// FUNÇÃO: usarItem
// OBJETIVO: Consumir um item (ex: tomar café) ou equipar, removendo-o da mochila.
void usarItem(Jogador *j, int posicao){
    // 1. Receber qual é o índice (posicao) do item que o jogador clicou no menu.
    // 2. Acessar o item: Item itemUsado = j->inventario[posicao];
    // 3. Fazer um 'if' ou 'switch' baseado no tipo (itemUsado.tipo):
    //    - Se for ITEM_CURA: Somar o HP (j->hp += itemUsado.poder), garantindo que não passe de 100.
    // 4. Remover o item da mochila: fazer um laço 'for' para "puxar" todos os itens que estão depois desse,
    //    uma casa para trás (ex: inventario[i] = inventario[i+1]), cobrindo o buraco no vetor.
    // 5. Diminuir a quantidade: j->qtdItens--;
}

// FUNÇÃO: desenharInventario
// OBJETIVO: Mostrar a lista de itens na tela de Pausa usando o Raylib.
void desenharInventario(Jogador *j){
    // 1. Usar um laço 'for' que vai de 0 até j->qtdItens.
    // 2. Usar a função DrawText para desenhar o j->inventario[i].nome na tela.
    // 3. DICA: Para não desenhar um nome em cima do outro, o eixo Y (altura) do texto precisa
    //    aumentar a cada item. Ex: int posY = 150 + (i * 30);
}