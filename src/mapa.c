// Mapa.h
#include <stdio.h>
#include <stdlib.h>
#include "mapa.h"
#include "entidades.h"

// FUNÇÃO: inicializarMapa 
// OBJETIVO: Alocar a matriz dinamicamente e preencher a matriz com 0 (chão) e 1 (parede) lendo um arquivo de texto.
// POR QUE: Facilita a criação de mapas. Você pode desenhar o mapa no Bloco de Notas (usando 0 e 1).
int **inicializarMapa(const char *MapaTxt, int *linhas, int *colunas){
    // Abre o arquivo MapaTxt em modo leitura ("r").
    FILE *file_mapa = fopen(MapaTxt, "r");
    // Se for NULL, imprime um erro no console e retorna NULL.
    if (file_mapa == NULL){
        perror("Erro ao ler o arquivo do mapa");
        fclose(file_mapa);
        return NULL;
    }
    // Lê a primeira linha usando fscanf(file_mapa, "%d %d", linhas, colunas) para descobrir o tamanho.
    fscanf(file_mapa, "%d %d", linhas, colunas);
    // Usar malloc() para criar um vetor de ponteiros (linhas) e, dentro de um laço 'for', dar malloc() para cada coluna.
    int **vetor_mapa = (int **) malloc(*linhas * sizeof(int *));
    if (vetor_mapa == NULL){
        printf("Erro ao alocar memoria.\n");
        return NULL;
    }
    for (int i = 0; i < *linhas; i++){
        vetor_mapa[i] = (int *) malloc(*colunas * sizeof(int));
        if (vetor_mapa[i] == NULL){
            printf("Erro ao alocar memoria.\n");
            for (int x = 0; x < i; x++) free(vetor_mapa[x]);
            free(vetor_mapa);
            return NULL;
        }
    }
    // Usar dois laços 'for' (i e j) e usar fscanf(file_mapa, "%d", &vetor_mapa[i][j]) para ler o resto dos números e preencher a matriz.
    for (int i = 0; i < *linhas; i++){
        for (int j = 0; j < *colunas; j++){
            fscanf(file_mapa, "%d", &vetor_mapa[i][j]);
        }
    }
    // Fecha o arquivo (fclose) e retorna o ponteiro da matriz alocada.
    fclose(file_mapa);
    return vetor_mapa;
}

// FUNÇÃO: liberarMapa 
// OBJETIVO: Limpar a memória do mapa antigo antes de carregar o próximo.
void liberarMapa(int **vetor_mapa, int linhas){
    // Laço 'for' para dar free() em cada linha (mapa[i]).
    for (int i = 0; i < linhas; i++) free(vetor_mapa[i]);
    // Free() no ponteiro principal (mapa).
    free(vetor_mapa);
}

// FUNÇÃO: desenharMapa
// OBJETIVO: Ler a matriz e desenhar os quadrados coloridos na tela.
void desenharMapa(int **vetor_mapa, int linhas, int colunas){
    for (int i = 0; i < linhas; i++){
        for(int j = 0; j < colunas; j++){

            // Converte a posição da matriz (índices 0, 1, 2...) para pixels na tela.
            // Ex: coluna 2 * 40px = posição 80px no eixo X.
            int posX = j * MAPA_TILE_SIZE;
            int posY = i * MAPA_TILE_SIZE;
            
            // Desenha o bloco dependendo do número salvo na matriz
            if(vetor_mapa[i][j] == 1)
                DrawRectangle(posX, posY, MAPA_TILE_SIZE, MAPA_TILE_SIZE, DARKGRAY); // Parede escura 
            if(vetor_mapa[i][j] == 0)
                DrawRectangle(posX, posY, MAPA_TILE_SIZE, MAPA_TILE_SIZE, LIGHTGRAY); // Chão claro
            if(vetor_mapa[i][j] == -1 || vetor_mapa[i][j] == 2)
                DrawRectangle(posX, posY, MAPA_TILE_SIZE, MAPA_TILE_SIZE, GREEN); // Porta ou Saída (Verde)
            if(vetor_mapa[i][j] == 3)
                DrawRectangle(posX, posY, MAPA_TILE_SIZE, MAPA_TILE_SIZE, MAROON); // Porta Trancada (Vermelha)
            /* // Desenha as linhas de grade para facilitar a visualização
            DrawRectangleLines(posX, posY, MAPA_TILE_SIZE, MAPA_TILE_SIZE, GRAY); */
        } 
    }
}

// FUNÇÃO: checarColisaoComMapa
// OBJETIVO: Impedir que o jogador atravesse os blocos de valor "1".
// RETORNA: 'true' se bateu em uma parede, 'false' se o caminho estiver livre.
bool checarColisaoComMapa(int **vetor_mapa, int linhas, int colunas, Rectangle playerRec){
    
    // Em vez de checar todos os blocos do mapa, checamos apenas os blocos que estão imediatamente ao redor e debaixo do jogador.
    // Convertendo a posição em pixels do jogador de volta para índices da matriz:
    int minX = playerRec.x / MAPA_TILE_SIZE;
    int minY = playerRec.y / MAPA_TILE_SIZE;
    int maxX = (playerRec.x + playerRec.width) / MAPA_TILE_SIZE;
    int maxY = (playerRec.y + playerRec.height) / MAPA_TILE_SIZE;
    
    // Vasculha apenas a área próxima ao jogador.
    for (int i = minY; i <= maxY; i++){
        for (int j = minX; j <= maxX; j++){
            if (vetor_mapa[i][j] == 1 || vetor_mapa[i][j] == 3){ // Se o bloco verificado for uma parede.
                
                // Cria um Rectangle para esse bloco de parede.
                Rectangle bloco = { j * MAPA_TILE_SIZE, i * MAPA_TILE_SIZE, MAPA_TILE_SIZE, MAPA_TILE_SIZE };
                
                // A função CheckCollisionRecs da Raylib cruza as duas caixas (jogador e bloco).
                // Se elas se sobrepuserem, houve colisão.
                if (CheckCollisionRecs(playerRec, bloco)){
                    return true; // Colisão confirmada. Interrompe a função.
                }
            }
        }
    }
    return false; // Se o loop terminar sem achar parede, o caminho está livre.
}

// FUNÇÃO: checarTransicaoDeFase
// OBJETIVO: Mudar de sala se o jogador pisar em um bloco de "Porta" (número 2 na matriz).
int **checarTransicaoDeFase(int **vetor_mapa, int *linhas, int *colunas, Jogador *j, int *faseAtual){
    // Recebe a posição X/Y do jogador e vê em qual bloco da matriz ele está pisando.    
    // Converte a posição X/Y do jogador (em pixels) para a posição na Matriz (linha e coluna).
    Rectangle h = obterHitbox(j);
    int col = (int)((h.x + h.width  / 2) / MAPA_TILE_SIZE);
    int lin = (int)((h.y + h.height / 2) / MAPA_TILE_SIZE);

    // Verificação de segurança para não tentar ler fora da matriz
    if (lin < 0 || lin >= *linhas || col < 0 || col >= *colunas) return vetor_mapa;

    int tipoPorta = vetor_mapa[lin][col];
    if (tipoPorta != 2 && tipoPorta != -1) return vetor_mapa;

    int novaFase = *faseAtual + (tipoPorta == 2 ? 1 : -1);
    char nomeArq[40];
    snprintf(nomeArq, sizeof(nomeArq), "./mapa%d.txt", novaFase);

    int novasLinhas, novasColunas; // temporários: não sobrescrevem os valores do mapa atual se falhar
    int **novoMapa = inicializarMapa(nomeArq, &novasLinhas, &novasColunas);

    if (novoMapa == NULL){
        printf("Mapa %d ainda nao existe. Porta desativada.\n", novaFase);
        vetor_mapa[lin][col] = 0; // evita tentar recarregar a cada frame
        return vetor_mapa;
    }

    liberarMapa(vetor_mapa, *linhas);
    *linhas = novasLinhas;
    *colunas = novasColunas;
    *faseAtual = novaFase;
    aplicarPortasAbertas(novoMapa, novasLinhas, novasColunas, novaFase);

    if (lin > novasLinhas - 2) lin = novasLinhas - 2;
    if (lin < 1) lin = 1;

    j->pos.y = lin * MAPA_TILE_SIZE;
    j->pos.x = (tipoPorta == 2) ? 1 * MAPA_TILE_SIZE : (novasColunas - 2) * MAPA_TILE_SIZE;

    return novoMapa;
}

#define MAX_FASES 10
static bool portaAberta[MAX_FASES + 1] = { false };

void registrarPortaAberta(int fase){
    if (fase >= 0 && fase <= MAX_FASES) portaAberta[fase] = true;
}

// Ao recarregar uma fase, troca as portas trancadas (3) por abertas (2) se já foram destrancadas
static void aplicarPortasAbertas(int **mapa, int linhas, int colunas, int fase){
    if (fase < 0 || fase > MAX_FASES || !portaAberta[fase]) return;
    for (int i = 0; i < linhas; i++)
        for (int j = 0; j < colunas; j++)
            if (mapa[i][j] == 3) mapa[i][j] = 2;
}