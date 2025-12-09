# ♠️ Blackjack em C (Estrutura de Dados)

Projeto desenvolvido como trabalho final da disciplina de **Estrutura de Dados 1** na **UTFPR**. O projeto implementa um jogo de Blackjack funcional via console, utilizando uma **Pilha Estática** para gerenciar o baralho de cartas.

## 📋 Funcionalidades

* **Sistema de Apostas:** Gerenciamento de saldo do jogador ("Bankroll").
* **Mecânica Completa:** Lógica de Hit, Stand, estouro (Bust) e valor dinâmico do Ás.
* **IA do Dealer:** O computador segue a regra estrita de comprar até atingir 17 pontos.
* **Interface Visual:** Uso de cores para distinguir naipes, mensagens de erro e vitória.
* **Modo Trapaça (Cheat Mode):** Implementação de um algoritmo de busca para manipular a próxima carta do baralho (Feature de Debug).

## 🛠️ Tecnologias e Conceitos

O projeto foi construído em **C** utilizando a IDE **Code::Blocks**, aplicando os seguintes conceitos acadêmicos:

* **TAD (Tipo Abstrato de Dado):** Separação entre Interface (`.h`) e Implementação (`.c`).
* **Pilha Estática:** O baralho é um vetor fixo onde cartas são inseridas e removidas apenas pelo topo (LIFO).
* **Ponteiros:** Passagem de parâmetros por referência para manipulação da estrutura.
* **Busca Sequencial:** Utilizada no modo trapaça para encontrar cartas no meio da pilha.
* **Alocação Estática:** Uso de memória pré-definida (`#define MAX_CARTAS 52`) conforme requisito da disciplina.

## 📂 Estrutura de Arquivos

* `main.c`: Lógica principal do jogo, menu e interação com o usuário.
* `baralho.h`: Definição das Structs (Carta, Baralho) e protótipos das funções.
* `baralho.c`: Implementação das funções da pilha (Empilhar, Desempilhar, Embaralhar).
* `cores.h`: Definições de macros ANSI para colorir o terminal.

## 🚀 Como Rodar (Code::Blocks)

1.  Abra o **Code::Blocks**.
2.  Crie um novo projeto: `File` -> `New` -> `Project` -> `Console Application` (C).
3.  Adicione os arquivos ao projeto:
    * Crie `baralho.h` e `cores.h` (Verifique se estão na pasta do projeto).
    * Crie `baralho.c` e cole o código de implementação.
    * Substitua o conteúdo de `main.c` pelo código do jogo.
4.  **Importante:** Certifique-se de que os arquivos `.c` estão marcados para compilar ("Debug" e "Release").
5.  Pressione `F9` para Compilar e Rodar.

## 🎮 Como Jogar

1.  Escolha o valor da aposta.
2.  Receba duas cartas.
3.  Escolha:
    * **[H]it:** Pedir mais uma carta.
    * **[S]tand:** Parar com a pontuação atual.
4.  Torça para o Computador estourar (passar de 21) ou fazer menos pontos que você!

---
*Desenvolvido por Felipe Gabriel Gomes - UTFPR 2025*
