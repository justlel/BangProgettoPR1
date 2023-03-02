#include "mazzo.h"


/**
 * Carica dal file di testo "mazzo_bang.txt" un mazzo di carte. In particolare, il file è letto in modo che
 * ogni riga contenga le informazioni su una carta del mazzo, in ordine: tipologia di carta, numero della carta,
 * seme della carta e nome della carta.
 * Se il file non è trovato, il programma si arresta.
 * Se vi sono dei problemi nell'allocazione dinamica dell'array di carte, il programma si arresta.
 *
 * @return Un puntatore a un mazzo di carte caricato dal file.
 */
Mazzo* caricaMazzo() {
    // apertura del file contenente le carte da caricare attraverso fopen()
    FILE* fileMazzo = NULL;
    fileMazzo = fopen("mazzo_bang.txt", "r");
    if(fileMazzo == NULL) {
        printf("Errore: impossibile aprire il file contenente le carte del mazzo. Arresto.");
        exit(-1);
    }

    Carta* carteMazzo = NULL;
    Mazzo mazzo = {MAZZO_PESCA, 0, carteMazzo};

    // lettura del file, linea per linea
    int read = 0;
    do {
        Carta cartaLetta;
        read = fscanf(fileMazzo,
                      "%d, %d, %d, %s",
                      &cartaLetta.tipologiaCarta, &cartaLetta.numeroCarta, &cartaLetta.semeCarta, cartaLetta.nomeCarta);
        if(read == 4) {
            mazzo.numeroCarte += 1;
            carteMazzo = (Carta*) realloc(carteMazzo, mazzo.numeroCarte * sizeof(Carta));
            if(carteMazzo == NULL) {
                printf("Errore: impossibile allocare dinamicamente il mazzo di carte. Arresto.");
                exit(-1);
            } else {
                carteMazzo[mazzo.numeroCarte - 1] = cartaLetta;
            }
        }
    } while(read == 4);

    mischiaMazzo(&mazzo);
}

void mischiaMazzo(Mazzo* mazzo) {
    // TODO: nel caso, raccogliere in un altro file
    srand(time(NULL));

    int shuffled;
    Carta tmpShuffled;
    for(int i = 0; i < mazzo->numeroCarte; i++) {
        do {
            shuffled = rand() % mazzo->numeroCarte;
        } while(shuffled == i);
        tmpShuffled = mazzo->carte[i];
        mazzo->carte[i] = mazzo->carte[shuffled];
        mazzo->carte[shuffled] = tmpShuffled;
    }
}

void assegnaCarte(Mazzo* mazzo, Giocatore* giocatori, int nGiocatori) {
    Mazzo* mazzoModificato = mazzo;
    for(int i = 0; i < nGiocatori; i++) {
        giocatori[i].carteMano = (Carta*) calloc(giocatori->puntiVita, sizeof(Carta));
        // TODO: nel caso, raccogliere in un altro file
        if(giocatori[i].carteMano == NULL) {
            printf("Errore: impossibile allocare dinamicamente il mazzo di carte del giocatore. Arresto.");
            exit(-1);
        }
        for(int j = 0; j < giocatori[i].puntiVita; j++) {
            giocatori[i].carteMano[j] = mazzoModificato->carte[j];
        }
        scartaCima(mazzo, giocatori[i].puntiVita);
        mazzo->numeroCarte -= giocatori[i].puntiVita;
    }
}

void scartaCima(Mazzo* mazzo, int daScartare) {
    mazzo->carte = (Carta*) realloc(mazzo->carte, (mazzo->numeroCarte - daScartare) * sizeof(Carta));
    // TODO: nel caso, raccogliere in un altro file
    if(mazzo->carte == NULL) {
        printf("Errore: impossibile allocare dinamicamente il mazzo di carte scartato. Arresto.");
        exit(-1);
    }
}