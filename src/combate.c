#include <stdio.h>
#include <string.h>
// ---------------------------------------------------------
// RESPONSÁVEL PELO COMBATE E PERGUNTAS: [Hyago]
// ---------------------------------------------------------
// O que adicionar aqui:
// 1. Criar a 'struct Pergunta' com texto, um vetor de strings para as 4 opções, e a resposta correta.
// 2. Usar alocação dinâmica (malloc) para criar um banco de perguntas de disciplinas do curso.
// 3. Criar a função void resolverTurno(Jogador *j, Inimigo *i, Pergunta p, int escolha).
// 4. Fazer a lógica: Se a escolha bater com a resposta certa, diminuir HP do Inimigo.
//    Se errar, diminuir HP do Jogador usando os ponteiros (j->hp).
// 5. Fazer a interface de combate usando Raylib (DrawText para mostrar a pergunta e opções).