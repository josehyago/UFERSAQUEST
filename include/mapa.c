#include "ufersaquest.h"

/*
#define MAPA_LINHAS 25
#define MAPA_COLUNAS 35
#define MAPA_TILE_SIZE 40 
*/

// Para ter mapas de tamanhos diferentes (a entrada da UFERSA  ser pequena e o interior ser gigante), não podemos mais usar o #define fixo.
// Vamos usar um ponteiro duplo (int **) e malloc() para criar a matriz baseada no arquivo
// O arquivo de texto (ex: mapa1.txt) deve ser assim:
// 20 30   <-- (Primeira linha dita a quantidade de linhas e colunas)
// 1 1 1...<-- (O resto é o mapa)
// No mapa, o número 0 é chão, 1 é parede e 2 pode ser uma PORTA de transição.

// A função inicializarMapa() mudou.
// FUNÇÃO: inicializarMapa 
// OBJETIVO: Alocar a matriz dinamicamente e preencher lendo um arquivo de texto.
// POR QUE: Facilita a criação de mapas. Você pode desenhar o mapa no Bloco de Notas (usando 0 e 1).
void inicializarMapa(int mapa[MAPA_LINHAS][MAPA_COLUNAS], const char *nomeArquivoTxt){
    // 1. Abrir o arquivo nomeArquivoTxt em modo leitura ("r").
    // 2. Se for NULL, imprime um erro no console e retorna NULL.
    // 3. Ler a primeira linha usando fscanf(arquivo, "%d %d", linhas, colunas) para descobrir o tamanho.
    // 4. Usar malloc() para criar um vetor de ponteiros (linhas) e, dentro de um laço 'for', dar malloc() para cada coluna.
    // 5. Usar dois laços 'for' (i e j) e usar fscanf(arquivo, "%d", &mapa[i][j]) para ler o resto dos números e preencher a matriz.
    // 6. Fechar o arquivo (fclose) e retornar o ponteiro da matriz alocada.
}
// FUNÇÃO: liberarMapa 
// OBJETIVO: Limpar a memória do mapa antigo antes de carregar o próximo.
void liberarMapa(int **mapa, int linhas){
    // 1. Fazer um laço 'for' para dar free() em cada linha (mapa[i]).
    // 2. No final, dar free() no ponteiro principal (mapa).
}

// FUNÇÃO: checarTransicaoDeFase
// OBJETIVO: Mudar de sala se o jogador pisar em um bloco de "Porta" (ex: número 2 na matriz).
void checarTransicaoDeFase(int **mapa, Jogador *j, int *faseAtual){
    // 1. Receber a posição X/Y do jogador e ver em qual bloco da matriz ele está pisando.
    // 2. Se a matriz nessa posição for igual a 2 (porta):
    //    - Alterar a variável faseAtual (ex: faseAtual++).
    //    - Chamar liberarMapa() para apagar o mapa atual da memória.
    //    - Chamar inicializarMapa() montando o nome do novo arquivo (ex: sprintf(nomeArq, "mapa%d.txt", faseAtual)).
    //    - Resetar a posição do jogador para o início da nova sala.
}

// As outras funções precisam ser adaptadas para receber o ponteiro (int **) e as dimensões dinâmicas.
void desenharMapa(int **mapa, int linhas, int colunas);
bool checarColisaoComMapa(int **mapa, int linhas, int colunas, Rectangle playerRec);

