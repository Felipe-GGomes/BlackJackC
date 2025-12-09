#ifndef BARALHO_H_INCLUDED
#define BARALHO_H_INCLUDED

#define MAX_CARTAS 52

typedef struct {
    char naipe[10];
    char face[3];
    int valor;
} Carta;

typedef struct {
    Carta itens[MAX_CARTAS];
    int topo;
} Baralho;

// --- Protótipos das Funções ---
void inicializaBaralho(Baralho *b);
int baralhoVazio(Baralho *b);
int baralhoCheio(Baralho *b);
int empilhaCarta(Baralho *b, Carta c);
int desempilhaCarta(Baralho *b, Carta *c);
void criarEEmbaralhar(Baralho *b);

// NOVA FUNÇÃO: Permite buscar uma carta no meio da pilha e colocar no topo
// Isso permite a "trapaça" de escolher a próxima carta
int forcarCartaNoTopo(Baralho *b, char face[], char naipe[]);

#endif // BARALHO_H_INCLUDED
