#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "baralho.h"

void inicializaBaralho(Baralho *b) { b->topo = -1; }

int baralhoVazio(Baralho *b) { return (b->topo == -1); }

int baralhoCheio(Baralho *b) { return (b->topo == MAX_CARTAS - 1); }

int empilhaCarta(Baralho *b, Carta c) {
    if (baralhoCheio(b)) return 0;
    b->topo++;
    b->itens[b->topo] = c;
    return 1;
}

int desempilhaCarta(Baralho *b, Carta *c) {
    if (baralhoVazio(b)) return 0;
    *c = b->itens[b->topo];
    b->topo--;
    return 1;
}

void criarEEmbaralhar(Baralho *b) {
    char naipes[4][10] = {"Copas", "Ouros", "Paus", "Espadas"};
    char faces[13][3] = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};
    int valores[13] = {11, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10};

    Carta tempDeck[MAX_CARTAS];
    int k = 0;

    for (int n = 0; n < 4; n++) {
        for (int f = 0; f < 13; f++) {
            strcpy(tempDeck[k].naipe, naipes[n]);
            strcpy(tempDeck[k].face, faces[f]);
            tempDeck[k].valor = valores[f];
            k++;
        }
    }

    srand(time(NULL));
    for (int i = 0; i < MAX_CARTAS; i++) {
        int r = rand() % MAX_CARTAS;
        Carta temp = tempDeck[i];
        tempDeck[i] = tempDeck[r];
        tempDeck[r] = temp;
    }

    inicializaBaralho(b);
    for (int i = 0; i < MAX_CARTAS; i++) {
        empilhaCarta(b, tempDeck[i]);
    }
}

// === IMPLEMENTAÇÃO DO COMANDO SECRETO ===
// Procura uma carta específica na pilha e a move para o topo
int forcarCartaNoTopo(Baralho *b, char face[], char naipe[]) {
    if (baralhoVazio(b)) return 0;

    for (int i = 0; i <= b->topo; i++) {
        // Compara se a carta na posição 'i' é a que queremos
        if (strcmp(b->itens[i].face, face) == 0 && strcmp(b->itens[i].naipe, naipe) == 0) {

            // TROCA (Swap): Pega a carta atual do topo
            Carta temp = b->itens[b->topo];
            // Coloca a carta desejada no topo
            b->itens[b->topo] = b->itens[i];
            // Coloca a carta que estava no topo na posição antiga da desejada
            b->itens[i] = temp;

            return 1; // Sucesso, achou e trocou
        }
    }
    return 0; // Carta não encontrada (talvez já tenha sido jogada)
}
