#include "ufersaquest.h"

// Adicionado ESTADO_MENU e ESTADO_PAUSA para expandir o jogo.
typedef enum{ 
    ESTADO_MENU, 
    ESTADO_EXPLORACAO, 
    ESTADO_PAUSA,
    ESTADO_COMBATE, 
    ESTADO_GAMEOVER 
} EstadoJogo;

// FUNÇÃO: salvarJogo
// OBJETIVO: Pegar a struct do Jogador atual e gravar diretamente em um arquivo binário.
// PARÂMETRO: Ponteiro para o jogador (para ler os dados atuais).
// POR QUE: Permite que o jogador feche o jogo e volte de onde parou (HP, posição e score mantidos).
void salvarJogo(Jogador *j){
    // 1. Abrir o arquivo "save.dat" em modo de escrita binária ("wb").
    // 2. Checar se o ponteiro do arquivo é diferente de NULL.
    // 3. Usar a função fwrite() passando o ponteiro do jogador, o sizeof(Jogador), 1 (quantidade) e o arquivo.
    // 4. Fechar o arquivo (fclose).
}

// FUNÇÃO: carregarJogo
// OBJETIVO: Ler o arquivo binário e reescrever os dados na struct do jogador na memória.
// PARÂMETRO: Ponteiro para o jogador (para sobrescrever os dados).
// RETORNO: bool - Retorna true se encontrou o save, false se for a primeira vez jogando.
bool carregarJogo(Jogador *j){
    // 1. Abrir o arquivo "save.dat" em modo de leitura binária ("rb").
    // 2. Se o arquivo for NULL, significa que não tem save. Retorna false.
    // 3. Se existir, usa fread() para ler o sizeof(Jogador) e jogar direto no ponteiro *j.
    // 4. Fechar o arquivo e retornar true.
}