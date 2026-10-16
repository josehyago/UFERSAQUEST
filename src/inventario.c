// Inventario.c
#include <stdbool.h>
#include "inventario.h"
#include "entidades.h"

// FUNÇÃO: adicionarItem
// OBJETIVO: Tentar colocar um item coletado do chão dentro do vetor (mochila) do jogador.
// PARÂMETROS: Ponteiro para o Jogador (j) e o Item novo a ser adicionado.
bool adicionarItem(Jogador *j, Item novoItem){

    // Verificação de segurança: se a quantidade atual de itens for 10 ou mais, a mochila está cheia.
    if(j->qtdItens >= 10) return false;

    // Se houver espaço, armazena o novo item na primeira posição livre do vetor (índice igual a j->qtdItens).
    j->inventario[j->qtdItens] = novoItem;

    // Incrementa o contador de itens da mochila.
    j->qtdItens++;

    // Retorna true indicando que o item foi coletado com sucesso.
    return true;
}

// FUNÇÃO: removerItem
// OBJETIVO: Apagar um item de uma posição específica da mochila e puxar os demais para tapar o buraco.
// PARÂMETROS: Ponteiro para o Jogador (j) e o índice inteiro da posição.
void removerItem(Jogador *j, int posicao){
    
    // Validação para evitar acessar posições inválidas no vetor.
    if (posicao < 0 || posicao >= j->qtdItens) return;

    // Laço for: começa na posição do item removido e vai até o penúltimo item.
    for (int i = posicao; i < j->qtdItens - 1; i++)

    // Copia o item da frente [i + 1] para a posição atual [i], deslocando tudo para a esquerda.
    j->inventario[i] = j->inventario[i + 1];

    // Decrementa o total de itens válidos na mochila.
    j->qtdItens--;
}

// FUNÇÃO: usarItem
// OBJETIVO: Consumir ou interagir com um item da mochila (como curar vida ou abrir manuais).
// PARÂMETROS: Ponteiro para o Jogador (j), índice do item (posicao) e ponteiro para o estado do menu (menuState).
void usarItem(Jogador *j, int posicao, EstadoMenuInventario *menuState){

    // Cria uma cópia temporária do item localizado na posição selecionada.
    Item itemUsado = j->inventario[posicao];

    // Avalia o tipo do item utilizando a estrutura switch baseada no Enum TipoItem.
    switch (itemUsado.tipo){
        case ITEM_CURA: 
        (j->hp += itemUsado.poder); // Soma o valor de poder ao HP atual.
        if (j->hp >= 100) j->hp = 100; // Trava de segurança para o HP não ultrapassar 100.
        removerItem(j, posicao); // Remove o item consumido da mochila.
        break;
        case ITEM_MANUAL:
            // O manual não é consumido, ele ativa a flag booleana que exibe o texto de leitura na tela.
            menuState->lendoManual = true;
            break;
        case ITEM_CHAVE:
            // A chave não é usada diretamente pelo menu (apenas ao interagir com a porta), então ao tentar usá-la pelo menu, ela ativa a flag de examinar descrição.
            menuState->examinandoItemGeral = true;
            break;
    }
}

// FUNÇÃO: temItem
// OBJETIVO: Verificar se o jogador possui pelo menos um item de um determinado TipoItem na mochila.
// RETORNA: true se encontrar o item, ou false caso contrário.
bool temItem(Jogador *j, TipoItem tipo){

    for (int i = 0; i < j->qtdItens; i++){
        if (j->inventario[i].tipo == tipo) return true; 
    }
    return false;
}

// FUNÇÃO: removerItemPorTipo
// OBJETIVO: Localizar e deletar automaticamente o primeiro item correspondente a um TipoItem (ex: gastar a chave).
void removerItemPorTipo(Jogador *j, TipoItem tipo){
    
    for (int i = 0; i < j->qtdItens; i++){
        if (j->inventario[i].tipo == tipo){
            removerItem(j, i); // Chama a função de apagar e reorganizar o vetor.
            return; // Encerra a função logo após remover para evitar apagar múltiplos itens de uma vez.
        }
    }
}

// FUNÇÃO: interagirComAmbiente
// OBJETIVO: Processar a ação de apertar ENTER no mapa (pegar itens do chão ou destrancar portas).
void interagirComAmbiente(Jogador *jogador, ItemNoChao *itensChao, int *qtdItensChao, int faseAtual, int **mapa, int linhas, int colunas){
    // Cria uma caixa de colisão (Rectangle) baseada na posição atual e tamanho do jogador.
    Rectangle recJogador = { jogador->pos.x, jogador->pos.y, jogador->tamanho, jogador->tamanho };
    
    // Tentar pegar um item do chão
    for (int i = 0; i < *qtdItensChao; i++){
        // Verifica se o item está ativo e pertence à fase atual em que o jogador se encontra.
        if (itensChao[i].ativo && itensChao[i].fase == faseAtual){
            Rectangle recItem = { itensChao[i].pos.x, itensChao[i].pos.y, MAPA_TILE_SIZE, MAPA_TILE_SIZE };
            // Se houver sobreposição entre o retângulo do jogador e o item do chão:
            if (CheckCollisionRecs(recJogador, recItem)){
                // Tenta adicionar na mochila, se conseguir espaço, desativa o item no chão.
                if (adicionarItem(jogador, itensChao[i].item)){
                    itensChao[i].ativo = false; 
                }
                break; // Interrompe o loop após interagir com o item.
            }
        }
    }

    // Tentar Destrancar a porta (Bloco 3 na Matriz).
    // Converte a posição central do jogador em pixels para coordenadas da matriz (linha e coluna).
    int colCentro = (int)((jogador->pos.x + jogador->tamanho/2) / MAPA_TILE_SIZE);
    int linCentro = (int)((jogador->pos.y + jogador->tamanho/2) / MAPA_TILE_SIZE);
    
    // Verifica uma área 3x3 ao redor do jogador para checar se ele está de frente para uma porta trancada.
    for(int i = linCentro - 1; i <= linCentro + 1; i++){
        for(int j = colCentro - 1; j <= colCentro + 1; j++){
            // Checagem de limites para garantir que não vai ler fora da matriz do mapa.
            if(i >= 0 && i < linhas && j >= 0 && j < colunas){
                if(mapa[i][j] == 3){ // Se encontrou o bloco de porta trancada
                    // Verifica se o jogador tem a Chave na mochila
                    if(temItem(jogador, ITEM_CHAVE)){ 
                        mapa[i][j] = 2; // Altera o valor da matriz para porta aberta/destrancada
                        removerItemPorTipo(jogador, ITEM_CHAVE); // Consome/remove a chave da mochila
                    }
                }
            }
        }
    }
}

// FUNÇÃO: atualizarMenuInventario
// OBJETIVO: Controlar a navegação por teclado dentro da tela do inventário (setas, fechar, abrir submenus).
void atualizarMenuInventario(Jogador *jogador, EstadoMenuInventario *menu, ItemNoChao *itensChao, int *qtdItensChao, int faseAtual, EstadoJogo *estadoAtual){
    // Se o jogador estiver lendo o manual ou examinando um item, qualquer tecla de saída fecha a janela de texto.
    if (menu->lendoManual || menu->examinandoItemGeral){
        if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_TAB)){
            menu->lendoManual = false;
            menu->examinandoItemGeral = false;
        }
    } 
    // Se o submenu de opções (Usar / Examinar / Descartar) estiver aberto:
    else if (menu->menuOpcoesAberto){
        if (IsKeyPressed(KEY_UP)) menu->opcaoSelecionada--;
        if (IsKeyPressed(KEY_DOWN)) menu->opcaoSelecionada++;
        // Garante loop cíclico nas 3 opções do submenu
        if (menu->opcaoSelecionada < 0) menu->opcaoSelecionada = 2;
        if (menu->opcaoSelecionada > 2) menu->opcaoSelecionada = 0;
        // Fecha o submenu
        if (IsKeyPressed(KEY_ESCAPE)) menu->menuOpcoesAberto = false;
        
        if (IsKeyPressed(KEY_ENTER)){
            Item itemAcao = jogador->inventario[menu->itemSelecionado];
            
            if (menu->opcaoSelecionada == 0 && itemAcao.tipo == ITEM_CURA){ // OPÇÃO: USAR
                usarItem(jogador, menu->itemSelecionado, menu);
                menu->menuOpcoesAberto = false;
                if (menu->itemSelecionado >= jogador->qtdItens) menu->itemSelecionado--;
            } 
            else if (menu->opcaoSelecionada == 1){ // OPÇÃO: EXAMINAR
                if (itemAcao.tipo == ITEM_MANUAL) menu->lendoManual = true;
                else menu->examinandoItemGeral = true;
                menu->menuOpcoesAberto = false;
            } 
            else if (menu->opcaoSelecionada == 2){ // OPÇÃO: DESCARTAR
                // Cria o item no chão na posição atual do jogador
                itensChao[*qtdItensChao].item = itemAcao;
                itensChao[*qtdItensChao].pos = jogador->pos;
                itensChao[*qtdItensChao].fase = faseAtual;
                itensChao[*qtdItensChao].ativo = true;
                (*qtdItensChao)++;
                
                removerItem(jogador, menu->itemSelecionado); // Remove da mochila
                menu->menuOpcoesAberto = false;
                if (menu->itemSelecionado >= jogador->qtdItens) menu->itemSelecionado--;
            }
            if (menu->itemSelecionado < 0) menu->itemSelecionado = 0;
        }
    } 
    // Navegação principal pelos slots da mochila
    else{
        if (IsKeyPressed(KEY_TAB) || IsKeyPressed(KEY_ESCAPE)) *estadoAtual = ESTADO_EXPLORACAO;
        
        if (IsKeyPressed(KEY_LEFT)) menu->itemSelecionado--;
        if (IsKeyPressed(KEY_RIGHT)) menu->itemSelecionado++;

        // Limites da seleção nos slots (0 a qtdItens - 1)
        if (menu->itemSelecionado < 0) menu->itemSelecionado = 0;
        if (menu->itemSelecionado >= jogador->qtdItens && jogador->qtdItens > 0) menu->itemSelecionado = jogador->qtdItens - 1;
        
        // Ao apertar ENTER em cima de um item válido, abre o submenu de opções
        if (IsKeyPressed(KEY_ENTER) && jogador->qtdItens > 0){
            menu->menuOpcoesAberto = true;
            menu->opcaoSelecionada = 0;
        }
    }
}

// FUNÇÃO: desenharMenuInventario
// OBJETIVO: Renderizar graficamente a interface da mochila, status do jogador, slots e caixas de texto.
void desenharMenuInventario(Jogador *jogador, EstadoMenuInventario *menu, int larguraTela, int alturaTela){
    // Desenha um painel escuro semitransparente cobrindo a tela de exploração
    DrawRectangle(40, 40, larguraTela - 80, alturaTela - 80, Fade(BLACK, 0.9f));
    DrawText("MENU DO JOGADOR", 60, 60, 30, WHITE);

    // Caixa de informações do Jogador (Nome, HP e Score)
    DrawRectangle(60, 120, 100, 100, DARKBLUE);
    DrawText(jogador->nome, 180, 120, 25, WHITE);
    DrawText(TextFormat("HP: %d / 100", jogador->hp), 180, 155, 20, GREEN);
    DrawText(TextFormat("Score: %d", jogador->score), 180, 190, 20, GOLD);

    // Laço for para desenhar os 10 slots fixos da mochila na tela
    for (int i = 0; i < 10; i++){
        Rectangle slot = { 60 + (i * 65), 350, 60, 60 };
        DrawRectangleRec(slot, GRAY); // Fundo cinza padrão do slot
        
        // Se houver item cadastrado neste slot, desenha o ícone colorido e o nome embaixo
        if (i < jogador->qtdItens){
            Color corItem = WHITE;
            if (jogador->inventario[i].tipo == ITEM_CHAVE) corItem = GOLD; // Chave = Amarelo Dourado
            else if (jogador->inventario[i].tipo == ITEM_MANUAL) corItem = DARKBLUE; // Manual = Azul Escuro
            else if (jogador->inventario[i].tipo == ITEM_CURA) corItem = GREEN; // Cura = Verde
            
            DrawRectangle(slot.x + 15, slot.y + 15, 30, 30, corItem);
            DrawText(jogador->inventario[i].nome, slot.x, slot.y + 65, 10, WHITE);
        }

        // Desenha uma borda amarela destacando o slot atualmente selecionado pelo jogador
        if (i == menu->itemSelecionado && !menu->menuOpcoesAberto && !menu->lendoManual && !menu->examinandoItemGeral && jogador->qtdItens > 0) {
            DrawRectangleLinesEx(slot, 3, YELLOW);
        }
    }

    // Se o submenu de ações estiver aberto, desenha a caixinha flutuante com as opções
    if (menu->menuOpcoesAberto){
        Rectangle sub = { 60 + (menu->itemSelecionado * 65), 230, 120, 110 };
        DrawRectangleRec(sub, DARKGRAY);
        DrawRectangleLinesEx(sub, 2, WHITE);
        
        Item itemAcao = jogador->inventario[menu->itemSelecionado];
        Color corUsar = (itemAcao.tipo == ITEM_CURA) ? WHITE : GRAY; // Desativa visualmente o "Usar" se não for cura
        
        DrawText("Usar", sub.x + 15, sub.y + 15, 18, (menu->opcaoSelecionada == 0) ? YELLOW : corUsar);
        DrawText("Examinar", sub.x + 15, sub.y + 45, 18, (menu->opcaoSelecionada == 1) ? YELLOW : WHITE);
        DrawText("Descartar", sub.x + 15, sub.y + 75, 18, (menu->opcaoSelecionada == 2) ? YELLOW : WHITE);
    }

    // Se estiver lendo o manual, desenha a janela branca grande com o texto carregado do arquivo de texto
    if (menu->lendoManual){
        DrawRectangle(60, 50, larguraTela - 120, alturaTela - 100, RAYWHITE);
        DrawRectangleLines(60, 50, larguraTela - 120, alturaTela - 100, DARKGRAY);
        // Título e instrução de fechar colocados no topo
        DrawText("MANUAL DE SOBREVIVÊNCIA", 80, 65, 20, BLACK);
        DrawText("(Aperte ENTER para fechar)", 480, 70, 14, RED);
        DrawLine(80, 95, larguraTela - 80, 95, GRAY);
        
        DrawText(menu->textoManual, 80, 105, 11, DARKGRAY); // Exibe o texto formatado do manual.txt
    } 
    // Se estiver examinando um item comum (Chave ou Coxinha), desenha uma caixa pop-up menor com sua descrição
    else if (menu->examinandoItemGeral){
        DrawRectangle(150, 200, 500, 150, DARKBLUE);
        DrawRectangleLines(150, 200, 500, 150, WHITE);
        
        Item itemAcao = jogador->inventario[menu->itemSelecionado];
        if (itemAcao.tipo == ITEM_CHAVE) DrawText("Uma chave dourada.\nParece se encaixar na porta final.", 170, 230, 20, WHITE);
        if (itemAcao.tipo == ITEM_CURA) DrawText("Uma coxinha do RU fresquinha.\nRecupera parte da vida perdida.", 170, 230, 20, WHITE);
        DrawText("Aperte ENTER para fechar.", 170, 310, 15, GRAY);
    }
}