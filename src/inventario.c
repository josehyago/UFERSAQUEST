// Inventario.c
#include <stdio.h>
#include <stdbool.h>
#include "inventario.h"
#include "entidades.h"
#include "ui.h"

// Opções do submenu que abre ao apertar ENTER sobre um item da mochila.
// TOTAL_OPCOES não é uma opção: serve só para saber quantas existem (usado no loop do cursor).
typedef enum { OPCAO_USAR, OPCAO_EXAMINAR, OPCAO_DESCARTAR, TOTAL_OPCOES } OpcaoMenu;

// FUNÇÃO: criarItem
// PARÂMETROS: nome - nome exibido; tipo - categoria (cura, chave, manual); poder - valor numérico (ex: HP que a cura recupera); descricao - texto mostrado em "Examinar".
// POR QUE: montar a struct campo a campo em vários lugares é repetitivo e fácil de errar.
// PARA QUE SERVE: RETORNA um Item pronto, copiando as strings com segurança (snprintf respeita o tamanho do vetor).
Item criarItem(const char *nome, TipoItem tipo, int poder, const char *descricao){
    Item novo;
    snprintf(novo.nome, sizeof(novo.nome), "%s", nome);
    novo.tipo = tipo;
    novo.poder = poder;
    snprintf(novo.descricao, sizeof(novo.descricao), "%s", descricao);
    return novo;
}

// FUNÇÃO: obterRetanguloItem
// PARÂMETROS: it - ponteiro (somente leitura) para o item que está no chão.
// POR QUE: o item (50px) é menor que o tile (100px). Calcular essa área em um só lugar garante que o que o jogador VÊ na tela é exatamente o que ele consegue TOCAR.
// PARA QUE SERVE: RETORNA o retângulo real do item, já centralizado dentro do seu tile. Usado para desenhar (desenharItensNoChao) e para detectar a coleta (pegarItemDoChao).
Rectangle obterRetanguloItem(const ItemNoChao *it){
    // Folga = espaço que sobra de cada lado do item dentro do tile: (100 - 50) / 2 = 25px.
    float folga = (MAPA_TILE_SIZE - TAMANHO_ITEM) / 2.0f;

    // Desloca o item "folga" pixels para a direita e para baixo, centralizando-o no tile.
    return (Rectangle){ it->pos.x + folga, it->pos.y + folga, TAMANHO_ITEM, TAMANHO_ITEM };
}

// FUNÇÃO: corDoItem
// PARÂMETROS: tipo - categoria do item.
// POR QUE: a escolha da cor estava repetida na main e no menu do inventário.
// PARA QUE SERVE: RETORNA a cor que representa cada tipo de item (um lugar só para mudar).
Color corDoItem(TipoItem tipo){
    switch (tipo){
        case ITEM_CHAVE:  return GOLD;     // chave dourada
        case ITEM_MANUAL: return DARKBLUE; // manual azul
        case ITEM_CURA:   return GREEN;    // cura verde
        default:          return WHITE;
    }
}

// FUNÇÃO: adicionarItem
// PARÂMETROS: j - ponteiro para o jogador; novoItem - o item a ser guardado.
// POR QUE: a mochila tem tamanho fixo e não pode estourar.
// PARA QUE SERVE: tenta colocar um item coletado dentro do vetor (mochila) do jogador. RETORNA true se coube, false se a mochila está cheia.
// COMPLEXIDADE: O(1).
bool adicionarItem(Jogador *j, Item novoItem){
    // Mochila cheia: não adiciona.
    if (j->qtdItens >= MAX_MOCHILA) return false;

    // Guarda o item na primeira posição livre (índice igual à quantidade atual) e conta mais um.
    j->inventario[j->qtdItens] = novoItem;
    j->qtdItens++;
    return true;
}

// FUNÇÃO: soltarItemNoChao
// PARÂMETROS: itens - vetor de itens do mapa; qtd - ponteiro para a quantidade em uso (é alterada aqui); item - o que será solto; pos - onde; fase - em qual fase ele aparece.
// POR QUE: sem controle de limite, cada drop estourava o vetor, e os slots de itens já coletados nunca eram reaproveitados.
// PARA QUE SERVE: coloca um item no chão reaproveitando slots inativos. RETORNA false se não houver espaço, para quem chamou decidir o que fazer.
// COMPLEXIDADE: O(n), n = quantidade de slots em uso (busca linear por um slot livre).
bool soltarItemNoChao(ItemNoChao *itens, int *qtd, Item item, Vector2 pos, int fase){
    int slot = -1;

    // Procura um slot livre (item que já foi coletado).
    for (int k = 0; k < *qtd; k++){
        if (!itens[k].ativo){ slot = k; break; }
    }

    // Se não achou, usa um slot novo, desde que não tenha chegado ao limite.
    if (slot == -1){
        if (*qtd >= MAX_ITENS_CHAO) return false;
        slot = (*qtd)++;
    }

    // Preenche o slot escolhido e o marca como ativo (visível e coletável).
    itens[slot].item = item;
    itens[slot].pos = pos;
    itens[slot].fase = fase;
    itens[slot].ativo = true;
    return true;
}

// FUNÇÃO: removerItem
// PARÂMETROS: j - ponteiro para o jogador; posicao - índice do item a apagar.
// POR QUE: ao tirar um item do meio do vetor, sobraria um "buraco" na mochila.
// PARA QUE SERVE: apaga o item da posição e puxa os seguintes uma casa para trás para tapar o buraco.
// COMPLEXIDADE: O(n), n = quantidade de itens na mochila.
void removerItem(Jogador *j, int posicao){
    // Validação para não acessar posições inválidas do vetor.
    if (posicao < 0 || posicao >= j->qtdItens) return;

    // Do item removido até o penúltimo: copia o item da frente [i + 1] para a posição atual [i].
    for (int i = posicao; i < j->qtdItens - 1; i++){
        j->inventario[i] = j->inventario[i + 1];
    }

    // Um item a menos na mochila.
    j->qtdItens--;
}

// FUNÇÃO: usarItem
// PARÂMETROS: j - ponteiro para o jogador; posicao - índice do item na mochila.
// POR QUE: apenas itens de cura são consumidos pelo menu. Manual e chave têm outro uso: o manual é lido em "Examinar" e a chave é gasta ao abrir a porta.
// PARA QUE SERVE: consome um item de cura: soma o poder dele ao HP (até HP_MAXIMO) e o remove da mochila.
void usarItem(Jogador *j, int posicao){
    if (posicao < 0 || posicao >= j->qtdItens) return;

    // Cópia do item selecionado; se não for de cura, não faz nada.
    Item itemUsado = j->inventario[posicao];
    if (itemUsado.tipo != ITEM_CURA) return;

    j->hp += itemUsado.poder;                 // soma o valor de cura ao HP atual
    if (j->hp > HP_MAXIMO) j->hp = HP_MAXIMO; // o HP não pode passar do máximo
    removerItem(j, posicao);                  // o item foi consumido
}

// FUNÇÃO: temItem
// PARÂMETROS: j - ponteiro (somente leitura) para o jogador; tipo - categoria procurada.
// POR QUE: portas e outras mecânicas precisam saber se o jogador carrega determinado item.
// PARA QUE SERVE: RETORNA true se existe ao menos um item do tipo na mochila, false caso contrário.
// COMPLEXIDADE: O(n), busca linear na mochila.
bool temItem(const Jogador *j, TipoItem tipo){
    for (int i = 0; i < j->qtdItens; i++){
        if (j->inventario[i].tipo == tipo) return true;
    }
    return false;
}

// FUNÇÃO: removerItemPorTipo
// PARÂMETROS: j - ponteiro para o jogador; tipo - categoria do item a gastar.
// POR QUE: ao abrir a porta não importa QUAL chave foi usada, só que uma chave seja gasta.
// PARA QUE SERVE: localiza e apaga o primeiro item do tipo informado (ex: gastar a chave).
// COMPLEXIDADE: O(n), busca linear mais o deslocamento feito por removerItem.
void removerItemPorTipo(Jogador *j, TipoItem tipo){
    for (int i = 0; i < j->qtdItens; i++){
        if (j->inventario[i].tipo == tipo){
            removerItem(j, i); // apaga e reorganiza o vetor
            return;            // para logo após o primeiro, para não apagar vários itens de uma vez
        }
    }
}

// FUNÇÃO: carregarTextoManual
// PARÂMETROS: menu - estado do menu (onde o texto é guardado); caminho - arquivo (ex: "manual.txt").
// POR QUE: o texto do manual fica num arquivo para poder ser editado sem recompilar o jogo.
// PARA QUE SERVE: lê o arquivo para menu->textoManual. Se o arquivo não existir, deixa uma mensagem de erro no lugar, para o problema aparecer na tela do jogo.
void carregarTextoManual(EstadoMenuInventario *menu, const char *caminho){
    // Texto de reserva, usado se a leitura falhar.
    snprintf(menu->textoManual, sizeof(menu->textoManual), "Erro: %s nao encontrado!", caminho);

    FILE *arq = fopen(caminho, "r");
    if (arq == NULL) return;

    // fread devolve quantos bytes leu; usamos esse número para colocar o '\0' que termina a string.
    size_t lidos = fread(menu->textoManual, 1, sizeof(menu->textoManual) - 1, arq);
    menu->textoManual[lidos] = '\0';
    fclose(arq);
}

// FUNÇÃO: pegarItemDoChao (uso interno deste arquivo)
// PARÂMETROS: jogador; itensChao e qtdItensChao - os itens do mapa; faseAtual - fase onde o jogador está.
// POR QUE: separa a coleta da abertura de porta, para cada função fazer uma coisa só.
// PARA QUE SERVE: se a hitbox do jogador toca um item da fase atual, tenta guardá-lo na mochila. Só um item é pego por vez.
// COMPLEXIDADE: O(n), n = quantidade de itens no chão.
static void pegarItemDoChao(Jogador *jogador, ItemNoChao *itensChao, int qtdItensChao, int faseAtual){
    Rectangle recJogador = obterHitbox(jogador);

    for (int i = 0; i < qtdItensChao; i++){
        // Só interessa item que esteja no chão (ativo) e na fase em que o jogador está.
        if (!itensChao[i].ativo || itensChao[i].fase != faseAtual) continue;

        // Usa o mesmo retângulo que é desenhado na tela (obterRetanguloItem).
        if (CheckCollisionRecs(recJogador, obterRetanguloItem(&itensChao[i]))){
            // Se houver espaço na mochila, o item sai do chão.
            if (adicionarItem(jogador, itensChao[i].item)) itensChao[i].ativo = false;
            return; // interage com um único item por ENTER
        }
    }
}

// FUNÇÃO: tentarAbrirPorta (uso interno deste arquivo)
// PARÂMETROS: jogador; mapa, linhas e colunas - o mapa atual; faseAtual - fase onde o jogador está.
// POR QUE: separa a abertura de porta da coleta de itens.
// PARA QUE SERVE: se o jogador tem a chave e há uma porta trancada na área 3x3 ao redor dele, destrava a porta, lembra disso (registrarPortaAberta) e gasta a chave.
// COMPLEXIDADE: O(1), pois sempre olha só 9 blocos.
static void tentarAbrirPorta(Jogador *jogador, int **mapa, int linhas, int colunas, int faseAtual){
    // Sem chave não há o que fazer.
    if (!temItem(jogador, ITEM_CHAVE)) return;

    // Descobre em qual bloco está o centro da hitbox do jogador.
    Rectangle h = obterHitbox(jogador);
    int colCentro = (int)((h.x + h.width  / 2) / MAPA_TILE_SIZE);
    int linCentro = (int)((h.y + h.height / 2) / MAPA_TILE_SIZE);

    // Verifica a área 3x3 ao redor para ver se ele está perto de uma porta trancada.
    for (int i = linCentro - 1; i <= linCentro + 1; i++){
        for (int j = colCentro - 1; j <= colCentro + 1; j++){
            // Não ler fora da matriz.
            if (i < 0 || i >= linhas || j < 0 || j >= colunas) continue;

            if (mapa[i][j] == TILE_PORTA_TRANCADA){
                mapa[i][j] = TILE_PORTA;                  // destranca a porta
                registrarPortaAberta(faseAtual);          // lembra, para não trancar ao voltar para a fase
                removerItemPorTipo(jogador, ITEM_CHAVE);  // gasta a chave
                return;                                   // uma chave abre uma porta
            }
        }
    }
}

// FUNÇÃO: interagirComAmbiente
// PARÂMETROS: jogador; itensChao e qtdItensChao - itens do mapa; faseAtual; mapa, linhas e colunas.
// POR QUE: o ENTER no mapa tem duas utilidades e o jogador não precisa escolher qual.
// PARA QUE SERVE: processa o ENTER na exploração: pega o item que estiver tocando e/ou destrava uma porta próxima (se tiver a chave).
void interagirComAmbiente(Jogador *jogador, ItemNoChao *itensChao, int qtdItensChao, int faseAtual, int **mapa, int linhas, int colunas){
    pegarItemDoChao(jogador, itensChao, qtdItensChao, faseAtual);
    tentarAbrirPorta(jogador, mapa, linhas, colunas, faseAtual);
}

// FUNÇÃO: desenharItensNoChao
// PARÂMETROS: itens e qtd - vetor de itens do mapa e quantos estão em uso; fase - fase que está na tela.
// POR QUE: tira da main o laço de desenho dos itens.
// PARA QUE SERVE: desenha um quadrado (TAMANHO_ITEM) colorido para cada item ativo da fase. Deve ser chamada entre BeginMode2D e EndMode2D (usa coordenadas do mapa).
// COMPLEXIDADE: O(n), n = quantidade de itens no chão.
void desenharItensNoChao(const ItemNoChao *itens, int qtd, int fase){
    for (int i = 0; i < qtd; i++){
        if (itens[i].ativo && itens[i].fase == fase){
            // DrawRectangleRec recebe um Rectangle pronto (posição + tamanho). Equivale a DrawRectangleV(posição, tamanho, cor), mas reaproveita o retângulo já centralizado no tile.
            DrawRectangleRec(obterRetanguloItem(&itens[i]), corDoItem(itens[i].item.tipo));
        }
    }
}

// FUNÇÃO: atualizarMenuInventario
// PARÂMETROS: jogador; menu - estado do menu; itensChao e qtdItensChao - para o "Descartar" soltar o item no mapa; faseAtual; estadoAtual - ponteiro para o estado do jogo (permite fechar o menu).
// POR QUE: o menu tem camadas (mochila, submenu, texto) e cada uma reage de um jeito às teclas.
// PARA QUE SERVE: controla a navegação por teclado no inventário (setas, ENTER, ESC, TAB).
void atualizarMenuInventario(Jogador *jogador, EstadoMenuInventario *menu, ItemNoChao *itensChao, int *qtdItensChao, int faseAtual, EstadoJogo *estadoAtual){

    // Lendo o manual ou examinando um item. ENTER, ESC ou TAB fecham a janela de texto.
    if (menu->lendoManual || menu->examinandoItemGeral){
        if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_TAB)){
            menu->lendoManual = false;
            menu->examinandoItemGeral = false;
        }
    }
    // Submenu de opções (Usar / Examinar / Descartar) aberto.
    else if (menu->menuOpcoesAberto){
        if (IsKeyPressed(KEY_UP))   menu->opcaoSelecionada--;
        if (IsKeyPressed(KEY_DOWN)) menu->opcaoSelecionada++;

        // Cursor cíclico: depois da última opção volta à primeira (e vice-versa).
        // Somar TOTAL_OPCOES antes do % evita resultado negativo.
        menu->opcaoSelecionada = (menu->opcaoSelecionada + TOTAL_OPCOES) % TOTAL_OPCOES;

        // ESC fecha só o submenu.
        if (IsKeyPressed(KEY_ESCAPE)) menu->menuOpcoesAberto = false;

        // ENTER executa a opção escolhida sobre o item selecionado.
        if (IsKeyPressed(KEY_ENTER)){
            Item itemAcao = jogador->inventario[menu->itemSelecionado];

            switch (menu->opcaoSelecionada){
                case OPCAO_USAR:
                    // Só itens de cura podem ser usados; nos outros, o ENTER não faz nada.
                    if (itemAcao.tipo == ITEM_CURA){
                        usarItem(jogador, menu->itemSelecionado);
                        menu->menuOpcoesAberto = false;
                    }
                    break;

                case OPCAO_EXAMINAR:
                    // Manual abre o texto grande; os demais itens mostram a descrição.
                    if (itemAcao.tipo == ITEM_MANUAL) menu->lendoManual = true;
                    else menu->examinandoItemGeral = true;
                    menu->menuOpcoesAberto = false;
                    break;

                case OPCAO_DESCARTAR:
                    // Só tira da mochila se conseguiu largar o item no chão (na posição do jogador).
                    if (soltarItemNoChao(itensChao, qtdItensChao, itemAcao, jogador->pos, faseAtual)){
                        removerItem(jogador, menu->itemSelecionado);
                    }
                    menu->menuOpcoesAberto = false;
                    break;

                default:
                    break;
            }

            // Se o último item saiu da mochila, o cursor volta para o último slot válido.
            if (menu->itemSelecionado >= jogador->qtdItens) menu->itemSelecionado = jogador->qtdItens - 1;
            if (menu->itemSelecionado < 0) menu->itemSelecionado = 0;
        }
    }
    // Navegação principal pelos slots da mochila.
    else{
        // TAB ou ESC fecham o inventário e voltam ao jogo.
        if (IsKeyPressed(KEY_TAB) || IsKeyPressed(KEY_ESCAPE)) *estadoAtual = ESTADO_EXPLORACAO;

        if (IsKeyPressed(KEY_LEFT))  menu->itemSelecionado--;
        if (IsKeyPressed(KEY_RIGHT)) menu->itemSelecionado++;

        // Mantém a seleção dentro dos slots ocupados (0 a qtdItens - 1).
        if (menu->itemSelecionado < 0) menu->itemSelecionado = 0;
        if (menu->itemSelecionado >= jogador->qtdItens && jogador->qtdItens > 0) menu->itemSelecionado = jogador->qtdItens - 1;

        // ENTER sobre um item válido abre o submenu de opções.
        if (IsKeyPressed(KEY_ENTER) && jogador->qtdItens > 0){
            menu->menuOpcoesAberto = true;
            menu->opcaoSelecionada = OPCAO_USAR;
        }
    }
}

// FUNÇÃO: desenharMenuInventario
// PARÂMETROS: jogador; menu - estado do menu; larguraTela e alturaTela - tamanho da janela.
// POR QUE: o jogador precisa ver a mochila, o status e os textos de ajuda.
// PARA QUE SERVE: desenha o painel do inventário: status, slots, submenu de opções e as janelas de leitura do manual e de exame de item.
void desenharMenuInventario(Jogador *jogador, EstadoMenuInventario *menu, int larguraTela, int alturaTela){
    // Painel escuro de fundo e título.
    DrawRectangle(40, 40, larguraTela - 80, alturaTela - 80, Fade(BLACK, 0.9f));
    desenharTexto("MENU DO JOGADOR", 60, 60, 30, WHITE);

    // Caixa com as informações do jogador (nome, HP e score).
    DrawRectangle(60, 120, 100, 100, DARKBLUE);
    desenharTexto(jogador->nome, 180, 120, 25, WHITE);
    desenharTexto(TextFormat("HP: %d / %d", jogador->hp, HP_MAXIMO), 180, 155, 20, GREEN);
    desenharTexto(TextFormat("Score: %d", jogador->score), 180, 190, 20, GOLD);

    // Desenha os slots fixos da mochila.
    for (int i = 0; i < MAX_MOCHILA; i++){
        Rectangle slot = { 60 + (i * 65), 350, 60, 60 };
        DrawRectangleRec(slot, GRAY); // fundo cinza padrão do slot

        // Slot ocupado: ícone colorido (cor do tipo) e nome embaixo.
        if (i < jogador->qtdItens){
            DrawRectangle(slot.x + 15, slot.y + 15, 30, 30, corDoItem(jogador->inventario[i].tipo));
            desenharTexto(jogador->inventario[i].nome, slot.x, slot.y + 65, 10, WHITE);
        }

        // Borda amarela no slot selecionado (só quando nenhuma janela está aberta por cima).
        if (i == menu->itemSelecionado && !menu->menuOpcoesAberto && !menu->lendoManual && !menu->examinandoItemGeral && jogador->qtdItens > 0){
            DrawRectangleLinesEx(slot, 3, YELLOW);
        }
    }

    // Submenu flutuante com as ações, acima do slot selecionado.
    if (menu->menuOpcoesAberto){
        Rectangle sub = { 60 + (menu->itemSelecionado * 65), 230, 120, 110 };
        DrawRectangleRec(sub, DARKGRAY);
        DrawRectangleLinesEx(sub, 2, WHITE);

        // "Usar" fica cinza quando o item não é de cura (não pode ser usado).
        Item itemAcao = jogador->inventario[menu->itemSelecionado];
        Color corUsar = (itemAcao.tipo == ITEM_CURA) ? WHITE : GRAY;

        // A opção selecionada fica amarela.
        desenharTexto("Usar",      sub.x + 15, sub.y + 15, 18, (menu->opcaoSelecionada == OPCAO_USAR)      ? YELLOW : corUsar);
        desenharTexto("Examinar",  sub.x + 15, sub.y + 45, 18, (menu->opcaoSelecionada == OPCAO_EXAMINAR)  ? YELLOW : WHITE);
        desenharTexto("Descartar", sub.x + 15, sub.y + 75, 18, (menu->opcaoSelecionada == OPCAO_DESCARTAR) ? YELLOW : WHITE);
    }

    // Janela grande com o texto do manual (lido do manual.txt).
    if (menu->lendoManual){
        DrawRectangle(60, 50, larguraTela - 120, alturaTela - 100, RAYWHITE);
        DrawRectangleLines(60, 50, larguraTela - 120, alturaTela - 100, DARKGRAY);

        // Título à esquerda e aviso de como fechar alinhado à direita.
        desenharTexto("MANUAL DE SOBREVIVÊNCIA", 80, 65, 20, BLACK);
        const char *aviso = "(Aperte ENTER para fechar)";
        desenharTexto(aviso, larguraTela - 80 - medirTexto(aviso, 14), 70, 14, RED);
        DrawLine(80, 95, larguraTela - 80, 95, GRAY);

        desenharTexto(menu->textoManual, 80, 105, 16, DARKGRAY);
    }
    // Janela menor com a descrição do item examinado (chave, coxinha...).
    else if (menu->examinandoItemGeral){
        DrawRectangle(150, 200, 500, 150, DARKBLUE);
        DrawRectangleLines(150, 200, 500, 150, WHITE);

        Item itemAcao = jogador->inventario[menu->itemSelecionado];
        desenharTexto(itemAcao.descricao, 170, 230, 20, WHITE);
        desenharTexto("Aperte ENTER para fechar.", 170, 310, 15, GRAY);
    }
}