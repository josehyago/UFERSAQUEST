# UFERSAQUEST - Repositório

> Jogo educativo de RPG 2D desenvolvido em C com a biblioteca Raylib.

**AVISO**: Para conseguir rodar o projeto, é necessário ter a extensão **Code Runner** instalada no VS Code, o **Raylib** instalado via **MSYS2** e o ter configurado nas variáveis de ambiente do seu sistema.

--- 

## VISÃO GERAL
Informações gerais sobre o projeto.

**Resumo do projeto**

- Do que se trata o projeto? Tipo de jogo?

Jogo 2D top-down educativo/RPG onde uma caloura é transportada para uma dimensão sombria da UFERSA e precisa enfrentar desafios das disciplinas para conseguir escapar. 

- Qual o gênero?

RPG Top-Down 2D / Quiz Educativo por Turnos. 

- Pretende lançar para quais plataformas?
 
PC (Windows/Linux) — desenvolvido em C com a biblioteca Raylib.

---

## CONTROLES

| Teclado | Mouse | Ação |
| :--- | :---: | ---: |
| **WASD/SETAS** | - | Movimentação do personagem no mapa (4 direções: Cima, Baixo, Esquerda, Direita) |
| **1, 2, 3, 4** | - | Selecionar a resposta da pergunta durante a tela de combate |
| **ESC** | - | Pausar ou voltar ao menu inicial |

---

## ESCOPO DO PROJETO
Detalhes do escopo do projeto como um todo.

**Referências**

- Jogos, filmes, séries, que compõem referências deste projeto.

...

**Gameplay**
Um resumo de como funcionará o jogo a nível de gameplay.

Protótipo - Unidade 1:

1 mapa pequeno (Entrada/Portão Principal da UFERSA).
Movimentação da personagem em 4 direções travada por colisões de parede.
1 tipo de inimigo ("Cálculo").
1 tela de combate funcional acionada ao encostar no inimigo, exibindo uma pergunta e recebendo a resposta.

---

## MECÂNICAS
Informações gerais sobre as mecânicas do jogo.
		
**Mecânicas do Player**

- Movimentação

Movimentação por grade (baseada na matriz do mapa).

- Encontro/Colisão
 
Quando os colisores de área do jogador e do inimigo se cruzarem, o jogo transita para o estado TELA_COMBATE.

- Combate por turnos

O inimigo ataca se o jogador errar a pergunta; o jogador ataca o inimigo se acertar a pergunta.

---

## HISTÓRIA
Informações de toda a narrativa do projeto.

- Sinopse

Resumo da história do jogo.

- História (Detalhada)

História em detalhes.

- Personagens

História de cada personagem.

---

## PERSONAGENS
Ficha técnica de todos os personagens presentes no projeto.

**Protagonista**

| Atributo | Detalhes |
| :--- | :--- |
| **Nome** | Caloura |
| **Tipo** | Player |
| **Mecânicas** | Andar pelo mapa, responder perguntas, receber dano ao errar respostas |
| **Atributos** | Vida (HP) e Disciplinas Concluídas (Score) |

**Inimigos**

| Atributo | Detalhes |
| :--- | :--- |
| **Nome** | Bug de C |
| **Tipo** | Inimigo |
| **Mecânicas** | Fica parado ou rondando uma área do mapa. Bloqueia a passagem para a próxima fase até ser derrotado |




