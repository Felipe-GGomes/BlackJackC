#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>   // Necessário para o delay
#include "baralho.h"
#include "cores.h"

// --- Funções Auxiliares de Interface ---

void limparTela() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// Função para criar o suspense
void delay(int milissegundos) {
    long pause;
    clock_t now,then;

    pause = milissegundos*(CLOCKS_PER_SEC/1000);
    now = then = clock();
    while( (now-then) < pause )
        now = clock();
}

// Limpa o buffer do teclado
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void imprimeCarta(Carta c) {
    if (strcmp(c.naipe, "Copas") == 0 || strcmp(c.naipe, "Ouros") == 0) {
        printf(VERMELHO "[%s de %s]" RESET, c.face, c.naipe);
    } else {
        printf(CIANO "[%s de %s]" RESET, c.face, c.naipe);
    }
    printf(" (Val: %d)\n", c.valor);
}

void ativarCheat(Baralho *b) {
    char face[3], naipe[10];
    int opNaipe;

    printf(CINZA "\n>>> MODO DEBUG/TRAPACA ATIVADO <<<\n");

    printf("-----------------------------------------\n");
    printf("| " BRANCO "TABELA DE VALORES" CINZA " | " BRANCO "CODIGOS DE NAIPES" CINZA " |\n");
    printf("|-------------------|-------------------|\n");
    printf("| A       = 11 (ou 1)| 1 - Copas " VERMELHO "(<3)" CINZA "  |\n");
    printf("| 2 a 9   = Face    | 2 - Ouros " VERMELHO "(<>)" CINZA "  |\n");
    printf("| 10      = 10      | 3 - Paus  " CIANO "(& )" CINZA "  |\n");
    printf("| J, Q, K = 10      | 4 - Espadas " CIANO "(^ )" CINZA " |\n");
    printf("-----------------------------------------\n" RESET);

    printf("Escolha a carta que voce quer puxar agora:\n");
    printf("Digite a Face (Ex: A, 7, 10, K): ");
    scanf("%s", face);

    printf("Digite o numero do Naipe (1-4): ");
    scanf("%d", &opNaipe);
    limparBuffer();

    switch(opNaipe) {
        case 1: strcpy(naipe, "Copas"); break;
        case 2: strcpy(naipe, "Ouros"); break;
        case 3: strcpy(naipe, "Paus"); break;
        case 4: strcpy(naipe, "Espadas"); break;
        default: strcpy(naipe, "Copas");
    }

    face[0] = toupper(face[0]);
    if(strlen(face) > 1) face[1] = toupper(face[1]);

    if (forcarCartaNoTopo(b, face, naipe)) {
        printf(VERDE "\n>> SUCESSO! A carta %s de %s foi movida para o topo!\n" RESET, face, naipe);
        printf(CINZA "   (Ela sera a proxima a ser sacada ao pedir 'H')\n" RESET);
    } else {
        printf(VERMELHO "\n>> ERRO: A carta %s de %s nao foi encontrada.\n" RESET, face, naipe);
    }

    printf("\nPressione ENTER para voltar ao jogo...");
    getchar();
}

void exibirRegras() {
    limparTela();
    printf(BRANCO "=================================================\n");
    printf(AMARELO "           MANUAL DE REGRAS - BLACKJACK          \n" RESET);
    printf(BRANCO "=================================================\n" RESET);

    printf(CIANO "\n[1] O OBJETIVO\n" RESET);
    printf("   Derrotar o Computador obtendo uma contagem de pontos\n");
    printf("   maior que a dele, mas sem ultrapassar " VERMELHO "21" RESET ".\n");

    printf(CIANO "\n[2] VALORES DAS CARTAS\n" RESET);
    printf("   - " VERDE "2 a 9" RESET ": Valem o numero da face.\n");
    printf("   - " VERDE "10, J, Q, K" RESET ": Valem " NEGRITO "10" RESET " pontos cada.\n");
    printf("   - " VERDE "As (A)" RESET ": Vale " NEGRITO "11" RESET " ou " NEGRITO "1" RESET ".\n");
    printf("     (O jogo ajusta o As para 1 automaticamente se voce estourar 21).\n");

    printf(CIANO "\n[3] A MESA (COMPUTADOR)\n" RESET);
    printf("   - O Computador joga depois de voce.\n");
    printf("   - Ele e " VERMELHO "OBRIGADO" RESET " a pedir cartas ate ter " AMARELO "17" RESET " ou mais.\n");
    printf("   - Se ele passar de 21, ele perde (e voce ganha).\n");

    printf(CIANO "\n[4] COMANDOS\n" RESET);
    printf("   - [" VERDE "H" RESET "]it  (Pedir): Pega mais uma carta do topo do baralho.\n");
    printf("   - [" VERMELHO "S" RESET "]tand (Parar): Encerra seu turno com os pontos atuais.\n");

    printf(CINZA "\n[5] MODO AVANCADO (DICA)\n");
    printf("   Existe um modo 'Trapaceiro' no menu inicial que permite\n");
    printf("   manipular a pilha de cartas. Use com moderacao!\n" RESET);

    printf("\n=================================================\n");
    printf(NEGRITO "Pressione ENTER para voltar ao menu..." RESET);
    getchar(); // Pausa
}

// Retorna: 1 (Vitória Jogador), -1 (Vitória PC), 0 (Empate)
int jogarPartida(int modoCheat, int apostaAtual) {
    Baralho meuBaralho;
    Carta cartaTemp;
    int ptsJog = 0, ptsComp = 0;
    int asesJog = 0, asesComp = 0;
    char opcao;

    limparTela();
    printf(BRANCO "APOSTA DA RODADA: R$ %d\n" RESET, apostaAtual);
    printf(VERDE "Embaralhando e distribuindo...\n" RESET);
    delay(1500); // Atraso maior para parecer que está embaralhando mesmo
    criarEEmbaralhar(&meuBaralho);

    // --- Distribuição Inicial ---
    printf(AMARELO "\n->>> Sua Mao <<<-\n" RESET);

    // Cartas do Jogador (Com delay maior)
    desempilhaCarta(&meuBaralho, &cartaTemp);
    if(cartaTemp.valor == 11) asesJog++;
    ptsJog += cartaTemp.valor;
    printf("Voce recebeu: ");
    delay(600); // Suspense carta 1
    imprimeCarta(cartaTemp);

    desempilhaCarta(&meuBaralho, &cartaTemp);
    if(cartaTemp.valor == 11) asesJog++;
    ptsJog += cartaTemp.valor;
    printf("Voce recebeu: ");
    delay(600); // Suspense carta 2
    imprimeCarta(cartaTemp);

    while (ptsJog > 21 && asesJog > 0) { ptsJog -= 10; asesJog--; }

    // AQUI: Total do Jogador em Laranja/Amarelo
    printf(AMARELO ">> Total Jogador: %d\n\n" RESET, ptsJog);
    delay(800);

    // Cartas do Computador
    desempilhaCarta(&meuBaralho, &cartaTemp);
    if(cartaTemp.valor == 11) asesComp++;
    ptsComp += cartaTemp.valor;

    // AQUI: Seção do Computador toda em ROXO (Magenta)
    printf(MAGENTA "--- Olho no Adversario ---\n" RESET);
    printf("O " MAGENTA "Computador" RESET " Mostra:\n");
    printf("> "); imprimeCarta(cartaTemp);
    printf(MAGENTA ">> Pontos que Vemos: %d\n" RESET, ptsComp);

    // Trapaça
    if (modoCheat) {
        Carta proxima = meuBaralho.itens[meuBaralho.topo];
        int previsao = ptsComp + proxima.valor;
        if (previsao > 21 && (proxima.valor == 11 || asesComp > 0)) previsao -= 10;

        printf(CINZA "\n-- (Trapaca) --\n");
        printf(">> (Trapaca) O Computador tem escondido: ");
        imprimeCarta(proxima);
        printf(CINZA ">> (Trapaca) Total Real do Computador: " RESET MAGENTA "%d\n" RESET, previsao);
    }

    printf(AMARELO "\n----------------------------------\n");
    printf(" SEUS PONTOS ATUAIS: %d\n", ptsJog);
    printf("----------------------------------\n" RESET);

    // --- Turno do Jogador ---
    while (1) {
        if (ptsJog == 21) {
            // Estilo mais "Blackjack" (Negrito e Vermelho/Verde forte)
            printf(NEGRITO VERDE "\n*** BLACKJACK! 21 PONTOS! ***\n" RESET);
            delay(1000);
            break;
        }

        printf("\nOpcoes: [" VERDE "H" RESET "]it (Pedir), [" VERMELHO "S" RESET "]tand (Parar)");
        if (modoCheat) printf(", [" CINZA "T" RESET "]rapaca");
        printf(": ");

        scanf(" %c", &opcao);
        limparBuffer();
        opcao = toupper(opcao);

        if (opcao == 'T' && modoCheat) {
            ativarCheat(&meuBaralho);
            limparTela();
            // Reimpressão resumida para o visual
            printf(AMARELO "\n->>> Sua Mao <<<-\n" RESET);
            printf(AMARELO ">> Seus Pontos: %d\n\n" RESET, ptsJog);

            printf(MAGENTA "--- Olho no Adversario ---\n" RESET);
            printf(MAGENTA ">> Pontos que Vemos: %d\n" RESET, ptsComp);
            continue;
        }

        if (opcao == 'H') {
            printf("Sacando carta...\n");
            delay(800); // Suspense no saque

            desempilhaCarta(&meuBaralho, &cartaTemp);
            printf("Voce sacou: "); imprimeCarta(cartaTemp);
            ptsJog += cartaTemp.valor;
            if (cartaTemp.valor == 11) asesJog++;

            while (ptsJog > 21 && asesJog > 0) {
                ptsJog -= 10; asesJog--;
                printf(AMARELO "(As valendo 1 para salvar voce)\n" RESET);
            }

            printf(AMARELO ">> Total Jogador: %d\n" RESET, ptsJog);

            if (ptsJog > 21) {
                printf(VERMELHO "\nESTOUROU! Voce passou de 21.\n" RESET);
                if (modoCheat) printf(CINZA "Perdeu trapaceando? Tem que treinar muito para ser ruim!\n" RESET);
                delay(1500);
                return -1; // Derrota
            }
        } else if (opcao == 'S') {
            printf("\nVoce parou com %d pontos.\n", ptsJog);
            delay(800);
            break;
        }
    }

    // --- Turno do Computador ---
    printf("\n" MAGENTA "--- Turno do Computador ---" RESET "\n");
    delay(1000); // Suspense antes de virar a carta

    desempilhaCarta(&meuBaralho, &cartaTemp);
    if(cartaTemp.valor == 11) asesComp++;
    ptsComp += cartaTemp.valor;

    while (ptsComp > 21 && asesComp > 0) { ptsComp -= 10; asesComp--; }

    printf("O " MAGENTA "Computador" RESET " revela a carta oculta: "); imprimeCarta(cartaTemp);
    printf(MAGENTA ">> Total do Computador: %d\n" RESET, ptsComp);
    delay(1500);

    while (ptsComp < 17) {
        printf("O " MAGENTA "Computador" RESET " esta pensando...\n");
        delay(1500); // O computador "pensa" mais tempo

        printf("O " MAGENTA "Computador" RESET " compra...\n");
        delay(500); // "Animação" de compra

        desempilhaCarta(&meuBaralho, &cartaTemp);
        imprimeCarta(cartaTemp);

        ptsComp += cartaTemp.valor;
        if (cartaTemp.valor == 11) asesComp++;

        while (ptsComp > 21 && asesComp > 0) {
            ptsComp -= 10; asesComp--;
        }
        printf(MAGENTA ">> Total do Computador: %d\n" RESET, ptsComp);
        delay(1000); // Pausa entre cartas para ler o que aconteceu
    }

    // --- Resultado ---
    printf(BRANCO "\n========================================\n");
    printf("            RESULTADO FINAL             \n");
    printf("========================================\n" RESET);
    printf(AMARELO "Voce: %d" RESET "  vs  " MAGENTA "Computador: %d\n\n" RESET, ptsJog, ptsComp);
    delay(1000); // Suspense final antes de dizer quem ganhou

    int resultado = 0;
    if (ptsComp > 21) {
        printf(VERDE "O Computador estourou! VOCE VENCEU!\n" RESET);
        resultado = 1;
    } else if (ptsJog > ptsComp) {
        printf(VERDE "Voce tem mais pontos. VOCE VENCEU!\n" RESET);
        resultado = 1;
    } else if (ptsJog < ptsComp) {
        printf(VERMELHO "O Computador venceu.\n" RESET);
        resultado = -1;
        if (modoCheat) printf(CINZA "Perdeu trapaceando... vergonhoso!\n" RESET);
    } else {
        printf(AMARELO "EMPATE!\n" RESET);
        resultado = 0;
    }

    if (resultado == 1 && modoCheat) {
        printf(CINZA "Acha bonito ganhar trapaceando?\n" RESET);
    }

    printf("\nPressione ENTER...");
    getchar();
    return resultado;
}

int main() {
    int opcaoMenu, modoJogo;
    char jogarNovamente;
    int saldo = 1000;
    int aposta = 0;
    int resultadoPartida;

    do {
        limparTela();
        // ESTILO MAIS "BLACKJACK": Vermelho e Negrito
        printf(VERMELHO "########################################\n");
        printf("#" NEGRITO BRANCO "      TRABALHO FINAL - ESTRUTURA      " RESET VERMELHO "#\n");
        printf("#" NEGRITO BRANCO "           BLACKJACK UTFPR            " RESET VERMELHO "#\n");
        printf("########################################\n" RESET);
        printf(AMARELO "\n $$$ SEU SALDO: R$ %d $$$\n" RESET, saldo);
        printf("\n1. " VERDE "Jogar" RESET);
        printf("\n2. " AMARELO "Regras" RESET);
        printf("\n3. " VERMELHO "Sair" RESET);
        printf("\n\nEscolha: ");
        scanf("%d", &opcaoMenu);
        limparBuffer();

        switch (opcaoMenu) {
            case 1:
                if (saldo <= 0) {
                    printf(VERMELHO "\nVoce faliu! O jogo acabou.\n" RESET);
                    printf("Pressione ENTER para resetar o jogo...\n");
                    getchar();
                    saldo = 1000;
                    break;
                }

                limparTela();
                printf("Escolha a Dificuldade:\n");
                printf("1. " VERDE "Normal" RESET " (Justo)\n");
                printf("2. " CINZA "Trapaceiro" RESET " (Modo Deus)\n");
                printf("Opcao: ");
                scanf("%d", &modoJogo);
                limparBuffer();

                int flagCheat = (modoJogo == 2) ? 1 : 0;

                do {
                    // Sistema de Apostas
                    do {
                        limparTela();
                        printf(AMARELO "SEU SALDO: R$ %d\n" RESET, saldo);
                        printf("Quanto deseja apostar? (0 para sair): ");
                        if (scanf("%d", &aposta) != 1) {
                            aposta = -1;
                            limparBuffer();
                        } else {
                            limparBuffer();
                        }

                        if (aposta == 0) break;
                        if (aposta < 0 || aposta > saldo) {
                            printf(VERMELHO "Aposta invalida! Voce tem apenas R$ %d.\n" RESET, saldo);
                            printf("Pressione ENTER...");
                            getchar();
                        }
                    } while (aposta < 0 || aposta > saldo);

                    if (aposta == 0) break;

                    resultadoPartida = jogarPartida(flagCheat, aposta);

                    if (resultadoPartida == 1) {
                        saldo += aposta;
                        printf(VERDE "\n$$$ VOCE GANHOU R$ %d $$$\n" RESET, aposta);
                    } else if (resultadoPartida == -1) {
                        saldo -= aposta;
                        printf(VERMELHO "\n$$$ VOCE PERDEU R$ %d $$$\n" RESET, aposta);
                    } else {
                        printf(AMARELO "\n$$$ APOSTA DEVOLVIDA $$$\n" RESET);
                    }

                    if (saldo <= 0) {
                        printf(VERMELHO "\nVoce perdeu todo seu dinheiro!\n" RESET);
                        printf("Pressione ENTER para voltar ao menu...\n");
                        getchar();
                        break;
                    }

                    printf("\n" NEGRITO "Jogar novamente? (S/N): " RESET);
                    scanf(" %c", &jogarNovamente);
                    limparBuffer();

                } while (toupper(jogarNovamente) == 'S');
                break;

            case 2:
                exibirRegras();
                break;

            case 3:
                printf("\nSaindo...\n");
                break;

            default:
                printf("Opcao invalida.\n");
                getchar();
        }
    } while (opcaoMenu != 3);

    return 0;
}
