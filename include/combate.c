#include "ufersaquest.h"

// A struct Pergunta continua igual.

// A função criarBancoPerguntas mudou.
// PARÂMETRO ADICIONAL: const char* nomeArquivoTxt (Ex: "perguntas.txt").
// OBJETIVO: Descobrir quantas perguntas existem no arquivo, fazer o malloc, e ler linha por linha.
Pergunta* criarBancoPerguntas(int *qtdPerguntas, const char *nomeArquivoTxt){
    // 1. Abre o arquivo em modo leitura ("r").
    // 2. Lê a primeira linha do arquivo, que deve conter um número inteiro (a quantidade de perguntas). 
    //    Guarda esse número no ponteiro *qtdPerguntas.
    // 3. Agora que você sabe a quantidade, usa o malloc() igual já estava no seu código.
    // 4. Faz um laço 'for' para ler as próximas linhas do arquivo:
    //    - Usa fgets() para ler o enunciado.
    //    - Usa fgets() 4 vezes para ler as 4 opções.
    //    - Usa fscanf() para ler o índice da resposta correta (0, 1, 2 ou 3).
    //    - Lembrar de remover o '\n' que o fgets pega usando strcspn().
    // 5. Fecha o arquivo e retorna o ponteiro do banco.
}

// O restante continua igual.
void liberarBancoPerguntas(Pergunta *banco);
bool resolverTurno(Jogador *j, Inimigo *i, Pergunta *p, int escolha, char *mensagem, Color *corMensagem, bool *mudarPergunta);
void desenharInterfaceCombate(Pergunta p, Jogador j, Inimigo i);