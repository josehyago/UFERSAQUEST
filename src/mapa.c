// Mapa.h
#include <stdio.h>
#include <stdlib.h>
#include "mapa.h"
#include "entidades.h"

#define MAX_FASES 10 // quantidade máxima de fases (mapa1.txt ... mapa10.txt)
// Tamanho em pixels de cada bloco DENTRO da imagem sokoban_tilesheet.png
#define TILE_ORIGINAL_SIZE 64

// Guarda se a porta trancada de cada fase já foi aberta (o índice é o número da fase).
static bool portaAberta[MAX_FASES + 1] = { false };

// FUNÇÃO: inicializarMapa
// PARÂMETROS: MapaTxt - caminho do arquivo (ex: "./mapa1.txt"); linhas e colunas - ponteiros que recebem o tamanho lido do arquivo (parâmetros de saída).
// POR QUE: facilita criar mapas. Dá para desenhar a fase no Bloco de Notas usando os números do enum TipoTile.
// PARA QUE SERVE: aloca a matriz dinamicamente (malloc) e a preenche com o conteúdo do arquivo. RETORNA a matriz pronta, ou NULL se algo falhar (arquivo ausente, formato inválido ou falta de memória), sem deixar memória vazando.
// COMPLEXIDADE: O(n2), pois lê cada célula uma vez.
int **inicializarMapa(const char *MapaTxt, int *linhas, int *colunas){
    // Abre o arquivo em modo leitura ("r"). Se não abrir, avisa no console e devolve NULL.
    FILE *file_mapa = fopen(MapaTxt, "r");
    if (file_mapa == NULL){
        perror("Erro ao abrir o arquivo do mapa");
        return NULL;
    }

    // A primeira linha do arquivo traz o tamanho: "linhas colunas".
    if (fscanf(file_mapa, "%d %d", linhas, colunas) != 2 || *linhas <= 0 || *colunas <= 0){
        printf("Cabecalho invalido em %s\n", MapaTxt);
        fclose(file_mapa);
        return NULL;
    }

    // Aloca o vetor de ponteiros (uma posição para cada linha da matriz).
    int **vetor_mapa = (int **) malloc(*linhas * sizeof(int *));
    if (vetor_mapa == NULL){
        printf("Erro ao alocar memoria.\n");
        fclose(file_mapa);
        return NULL;
    }

    // Aloca cada linha com o número de colunas. Se alguma falhar, libera as anteriores e desiste.
    for (int i = 0; i < *linhas; i++){
        vetor_mapa[i] = (int *) malloc(*colunas * sizeof(int));
        if (vetor_mapa[i] == NULL){
            printf("Erro ao alocar memoria.\n");
            for (int x = 0; x < i; x++) free(vetor_mapa[x]);
            free(vetor_mapa);
            fclose(file_mapa);
            return NULL;
        }
    }

    // Lê os números restantes do arquivo e preenche a matriz célula por célula.
    for (int i = 0; i < *linhas; i++){
        for (int j = 0; j < *colunas; j++){
            if (fscanf(file_mapa, "%d", &vetor_mapa[i][j]) != 1){
                printf("Mapa %s incompleto (linha %d, coluna %d)\n", MapaTxt, i, j);
                liberarMapa(vetor_mapa, *linhas);
                fclose(file_mapa);
                return NULL;
            }
        }
    }

    fclose(file_mapa);
    return vetor_mapa;
}

// FUNÇÃO: liberarMapa
// PARÂMETROS: vetor_mapa - a matriz a ser apagada; linhas - quantas linhas ela tem.
// POR QUE: toda memória pedida com malloc precisa ser devolvida com free, senão há vazamento.
// PARA QUE SERVE: limpa da memória o mapa antigo antes de carregar o próximo (ou ao fechar o jogo).
// COMPLEXIDADE: O(n).
void liberarMapa(int **vetor_mapa, int linhas){
    // Libera cada linha (vetor_mapa[i]) primeiro
    for (int i = 0; i < linhas; i++) free(vetor_mapa[i]);
    // e só depois o vetor de ponteiros principal.
    free(vetor_mapa);
}

// FUNÇÃO: desenharMapa
// PARÂMETROS: vetor_mapa - a matriz do mapa; linhas e colunas - o tamanho dela.
// POR QUE: o mapa é só números; alguém precisa transformá-los em quadrados coloridos na tela.
// PARA QUE SERVE: percorre a matriz e desenha um quadrado de MAPA_TILE_SIZE para cada célula.
// COMPLEXIDADE: O(n2).
void desenharMapa(int **vetor_mapa, int linhas, int colunas, Texture2D tileset) {
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {

            // 1. Calcula onde o bloco vai ser desenhado na TELA (em pixels)
            float posX = j * MAPA_TILE_SIZE;
            float posY = i * MAPA_TILE_SIZE;

            // Retângulo de Destino (onde e qual tamanho terá na tela: 100x100)
            Rectangle recDestino = { posX, posY, (float)MAPA_TILE_SIZE, (float)MAPA_TILE_SIZE };

            // 2. Identifica qual o tipo de bloco na posição atual da matriz
            int tipoBloco = vetor_mapa[i][j];

            // Variáveis para guardar em qual Coluna e Linha da IMAGEM o bloco está
            int colImagem = 0;
            int linImagem = 0;

            // 3. Mapeia o número da matriz para a coordenada (Coluna x Linha) do arquivo PNG
            switch (tipoBloco) {
                case TILE_GRAMA: // Valor 0 no .txt
                    colImagem = 0; 
                    linImagem = 0;
                    break;

                case TILE_TERRA: // Valor 4 no .txt
                    colImagem = 1; 
                    linImagem = 0;
                    break;

                case TILE_PAREDE: // Valor 1 no .txt
                    colImagem = 2; 
                    linImagem = 0;
                    break;

                case TILE_PORTA:
                case TILE_VOLTA: // Valores 2 e -1 no .txt (Porta Aberta)
                    colImagem = 3; 
                    linImagem = 0;
                    break;

                case TILE_PORTA_TRANCADA: // Valor 3 no .txt
                    colImagem = 4; 
                    linImagem = 0;
                    break;

                default: // Caso haja um valor estranho, desenha grama por padrão
                    colImagem = 0; 
                    linImagem = 0;
                    break;
            }

            // 4. Recorta o quadrado exato de 64x64 pixels da imagem do tileset
            Rectangle recOrigem = { 
                colImagem * TILE_ORIGINAL_SIZE, 
                linImagem * TILE_ORIGINAL_SIZE, 
                TILE_ORIGINAL_SIZE, 
                TILE_ORIGINAL_SIZE 
            };

            // 5. Desenha na tela ajustando o tamanho de 64px para 100px automaticamente
            DrawTexturePro(tileset, recOrigem, recDestino, (Vector2){ 0.0f, 0.0f }, 0.0f, WHITE);
        }
    }
}



// FUNÇÃO: checarColisaoComMapa
// PARÂMETROS: vetor_mapa, linhas, colunas - o mapa; playerRec - retângulo de colisão (hitbox) a testar.
// POR QUE: impede o jogador de atravessar paredes e portas trancadas.
// PARA QUE SERVE: RETORNA true se o retângulo bate em parede/porta trancada, false se o caminho está livre.
// COMPLEXIDADE: O(1). Só olha os poucos blocos sob o retângulo, nunca o mapa inteiro.
bool checarColisaoComMapa(int **vetor_mapa, int linhas, int colunas, Rectangle playerRec){

    // Converte os cantos do retângulo (pixels) em índices da matriz: só esses blocos precisam ser testados.
    int minX = (int)(playerRec.x / MAPA_TILE_SIZE);
    int minY = (int)(playerRec.y / MAPA_TILE_SIZE);
    int maxX = (int)((playerRec.x + playerRec.width)  / MAPA_TILE_SIZE);
    int maxY = (int)((playerRec.y + playerRec.height) / MAPA_TILE_SIZE);

    // Trava os índices dentro da matriz para nunca ler fora dela.
    if (minX < 0) minX = 0;
    if (minY < 0) minY = 0;
    if (maxX >= colunas) maxX = colunas - 1;
    if (maxY >= linhas)  maxY = linhas - 1;

    // Vasculha apenas a área próxima ao jogador.
    for (int i = minY; i <= maxY; i++){
        for (int j = minX; j <= maxX; j++){
            // Só parede e porta trancada bloqueiam a passagem.
            if (vetor_mapa[i][j] == TILE_PAREDE || vetor_mapa[i][j] == TILE_PORTA_TRANCADA){

                // Cria um Rectangle para esse bloco.
                Rectangle bloco = { j * MAPA_TILE_SIZE, i * MAPA_TILE_SIZE, MAPA_TILE_SIZE, MAPA_TILE_SIZE };

                // CheckCollisionRecs (Raylib) retorna true se os dois retângulos se sobrepõem.
                if (CheckCollisionRecs(playerRec, bloco)) return true;
            }
        }
    }
    return false; // Nenhum bloco sólido encontrado: caminho livre.
}

// FUNÇÃO: registrarPortaAberta
// PARÂMETROS: fase - número da fase cuja porta foi destrancada.
// POR QUE: o mapa é recarregado do arquivo ao trocar de sala, e o arquivo sempre tem a porta trancada (3).
// PARA QUE SERVE: lembra que a porta dessa fase já foi aberta, para ela não "trancar de novo" ao voltar.
void registrarPortaAberta(int fase){
    if (fase >= 0 && fase <= MAX_FASES) portaAberta[fase] = true;
}

// FUNÇÃO: aplicarPortasAbertas (uso interno deste arquivo)
// PARÂMETROS: mapa, linhas, colunas - o mapa recém-carregado; fase - número dessa fase.
// POR QUE: complementa registrarPortaAberta, restaurando o que o jogador já destrancou.
// PARA QUE SERVE: se a porta dessa fase já foi aberta, troca todas as portas trancadas por portas abertas.
// COMPLEXIDADE: O(n2).
static void aplicarPortasAbertas(int **mapa, int linhas, int colunas, int fase){
    if (fase < 0 || fase > MAX_FASES || !portaAberta[fase]) return;

    for (int i = 0; i < linhas; i++){
        for (int j = 0; j < colunas; j++){
            if (mapa[i][j] == TILE_PORTA_TRANCADA) mapa[i][j] = TILE_PORTA;
        }
    }
}

// FUNÇÃO: checarTransicaoDeFase
// PARÂMETROS: vetor_mapa - mapa atual; linhas, colunas - ponteiros com o tamanho do mapa atual (são atualizados se a fase mudar); j - jogador (sua posição é ajustada); faseAtual - ponteiro para o número da fase (é atualizado se a fase mudar).
// POR QUE: o jogo tem várias salas; pisar numa porta precisa levar o jogador para outra.
// PARA QUE SERVE: se o centro da hitbox do jogador está sobre uma porta (2 = avançar, -1 = voltar), carrega o mapa da outra fase e reposiciona o jogador. RETORNA o ponteiro do mapa que deve ser usado daqui para frente (o novo, ou o mesmo se nada mudou).
// COMPLEXIDADE: O(1) quando não há porta; O(linhas x colunas) no quadro em que a fase é carregada.
int **checarTransicaoDeFase(int **vetor_mapa, int *linhas, int *colunas, Jogador *j, int *faseAtual){
    // Descobre em qual bloco da matriz está o CENTRO da hitbox do jogador.
    Rectangle h = obterHitbox(j);
    int col = (int)((h.x + h.width  / 2) / MAPA_TILE_SIZE);
    int lin = (int)((h.y + h.height / 2) / MAPA_TILE_SIZE);

    // Segurança: não ler fora da matriz.
    if (lin < 0 || lin >= *linhas || col < 0 || col >= *colunas) return vetor_mapa;

    // Se o bloco não for porta, não há o que fazer.
    int tipoPorta = vetor_mapa[lin][col];
    if (tipoPorta != TILE_PORTA && tipoPorta != TILE_VOLTA) return vetor_mapa;

    // Monta o nome do arquivo da fase de destino (ex: faseAtual 1 + porta 2 -> "./mapa2.txt").
    int novaFase = *faseAtual + (tipoPorta == TILE_PORTA ? 1 : -1);
    char nomeArq[40];
    snprintf(nomeArq, sizeof(nomeArq), "./mapa%d.txt", novaFase);

    // Carrega o novo mapa ANTES de apagar o antigo. Os tamanhos vão para variáveis temporárias, assim, se o carregamento falhar, os valores do mapa atual continuam intactos.
    int novasLinhas, novasColunas;
    int **novoMapa = inicializarMapa(nomeArq, &novasLinhas, &novasColunas);

    if (novoMapa == NULL){
        printf("Mapa %d ainda nao existe. Porta desativada.\n", novaFase);
        vetor_mapa[lin][col] = TILE_GRAMA; // transforma em grama para não tentar recarregar a cada quadro
        return vetor_mapa;
    }

    // Deu certo: troca o mapa antigo pelo novo e atualiza os valores do chamador.
    liberarMapa(vetor_mapa, *linhas);
    *linhas = novasLinhas;
    *colunas = novasColunas;
    *faseAtual = novaFase;
    aplicarPortasAbertas(novoMapa, novasLinhas, novasColunas, novaFase);

    // Mantém a linha de entrada dentro do novo mapa (entre a 1ª e a penúltima linha).
    if (lin > novasLinhas - 2) lin = novasLinhas - 2;
    if (lin < 1) lin = 1;

    // Reposiciona o jogador: avançou -> lado esquerdo do novo mapa; voltou -> lado direito.
    j->pos.y = lin * MAPA_TILE_SIZE;
    j->pos.x = (tipoPorta == TILE_PORTA) ? 1 * MAPA_TILE_SIZE : (novasColunas - 2) * MAPA_TILE_SIZE;

    return novoMapa;
}