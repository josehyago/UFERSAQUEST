// tipos.h
// Declarações antecipadas de tipos compartilhados entre vários arquivos .h
#ifndef TIPOS_H
#define TIPOS_H

/* POR QUE: mapa.h e inventario.h só precisam saber que "Jogador" existe 
 (eles usam apenas ponteiros), então não precisam incluir entidades.h inteiro. 
 Isso evita includes circulares.
 A definição completa (todos os campos) fica em entidades.h.
*/

typedef struct Jogador Jogador;

#endif