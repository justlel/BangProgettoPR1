#include "mazzo.h"


/**
 * Funzione che carica dal file di testo "mazzo_bang.txt" un mazzo di carte. In particolare, il file è letto in modo che
 * ogni riga contenga le informazioni su una carta del mazzo, in ordine: tipologia di carta, numero della carta,
 * seme della carta e nome della carta.
 * Dopo essere stato caricato dal file, il mazzo viene mischiato dalla funzione mischiaMazzo()
 * Se il file non è trovato, il programma si arresta.
 * Se vi sono dei problemi nell'allocazione dinamica dell'array di carte, il programma si arresta.
 *
 * @return Un mazzo di carte caricato dal file e mischiato.
 */
Mazzo caricaMazzo() {
    // apertura del file contenente le carte da caricare attraverso fopen()
    FILE* fileMazzo = NULL;
    fileMazzo = fopen("mazzo_bang.txt", "r");
    // verifica della corretta apertura del file
    if(fileMazzo == NULL) {
        printf("Errore: impossibile aprire il file contenente le carte del mazzo. Arresto.");
        exit(-1);
    }

    // inizializzazione di un puntatore allocato dinamicamente, che conterrà le carte da inserire nel mazzo
    Carta* carteMazzo = NULL;
    // inizializzazione del mazzo che sarà poi restituito dalla funzione
    Mazzo mazzo = {MAZZO_PESCA, 0, NULL};

    int read; // numero di byte letti dalla funzione fscanf
    Carta cartaLetta; // carta letta in ogni iterazione del ciclo dalla funzione fscanf
    // lettura del file, linea per linea
    do {
        read = fscanf(fileMazzo,
                      "%d %d %d %s\n",
                      &cartaLetta.tipologiaCarta, &cartaLetta.numeroCarta, &cartaLetta.semeCarta, cartaLetta.nomeCarta);
        // se sono stati letti 4 byte, significa che c'è una nuova carta da aggiungere al mazzo
        if(read == 4) {
            mazzo.numeroCarte += 1;
            // riallocazione dinamica del puntatore alle carte del mazzo, per fare spazio alla nuova carta letta
            carteMazzo = (Carta*) realloc(carteMazzo, mazzo.numeroCarte * sizeof(Carta));
            // verifica della corretta allocazione dinamica
            if(carteMazzo == NULL) {
                printf("Errore: impossibile allocare dinamicamente il mazzo di carte. Arresto.");
                exit(-1);
            } else {
                // l'ultima carta letta diventa l'ultima carta del mazzo
                carteMazzo[mazzo.numeroCarte - 1] = cartaLetta;
            }
        }
    } while(read == 4);
    fclose(fileMazzo); // chiusura del file

    // il puntatore contenente le carte lette viene inserito nella struct "mazzo"
    mazzo.carte = carteMazzo;

    // chiusura del file, ora che è stato letto
    fclose(fileMazzo);

    // il mazzo con le carte letto viene ora passato come puntatore a "mischiaMazzo" per rondomizzare l'ordine delle carte
    mischiaMazzo(&mazzo);

    // restituisci il mazzo letto e mischiato
    return mazzo;
}

/**
 * Subroutine che si occupa di mischiare il mazzo di carte appena caricato dal file di testo.
 * La randomizzazione dell'ordine segue questa logica: si itera sulle carte del mazzo, e ogni carta
 * viene scambiata con una carta in una posizione randomica all'interno del mazzo, fino ad arrivare
 * all'ultima carta.
 *
 * @param mazzo Il mazzo di carte da mischiare.
 */
void mischiaMazzo(Mazzo* mazzo) {
    int shuffled; // indice della carta estratta per essere scambiata con quella iterata nel ciclo
    Carta tmpShuffled; // carta che sarà scambiata con quella estratta randomicamente
    for(int i = 0; i < mazzo->numeroCarte; i++) {
        do {
            shuffled = rand() % mazzo->numeroCarte; // estrazione di un indice tra 0 e numeroCarte - 1
        } while(shuffled == i); // evito che la carta estratta per essere scambiata sia la stessa di quella iterata
        // scambio delle due carte
        tmpShuffled = mazzo->carte[i];
        mazzo->carte[i] = mazzo->carte[shuffled];
        mazzo->carte[shuffled] = tmpShuffled;
    }
}

/**
 * Funzione che scarta un numero dato di carte dalla cima di un mazzo.
 * Restituisce le carte che sono state scartate
 *
 * @param mazzo Il mazzo da cui scartare la carta
 * @param daScartare Il numero di carte da scartare
 * @return Le carte scartate dalla cima del mazzo
 */
Carta* scartaCimaMazzo(Mazzo* mazzo, int daScartare) {
    // TODO: cosa fare se finiscono le carte del mazzo?
    int i;
    Carta* carteScartate = NULL;

    // creo un array che contiene le carte che saranno scartate dal mazzo
    carteScartate = (Carta*) calloc(daScartare, sizeof(Carta));
    // TODO: nel caso, raccogliere in un altro file
    if(carteScartate == NULL) {
        printf("Errore: impossibile allocare dinamicamente il mazzo di carte scartato. Arresto.");
        exit(-1);
    }
    for(i = 0; i < daScartare; i++) {
        carteScartate[i] = mazzo->carte[mazzo->numeroCarte - 1 - i];
    }

    // scarto dal mazzo le carte
    mazzo->carte = (Carta*) realloc(mazzo->carte, (mazzo->numeroCarte - daScartare) * sizeof(Carta));
    // TODO: nel caso, raccogliere in un altro file
    if(mazzo->carte == NULL) {
        printf("Errore: impossibile allocare dinamicamente il mazzo di carte scartato. Arresto.");
        exit(-1);
    }
    // diminuisco il numero di carte presenti nel mazzo
    mazzo->carte -= daScartare;

    // restituisco le carte scartate
    return carteScartate;
}