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
 * Funzione che pesca un numero dato di carte dalla cima di un mazzoPesca.
 * Restituisce un puntatore a un array contenente le carte pescate dal mazzo di pesca.
 *
 * @param mazzoPesca Puntatore al mazzo di pesca, da cui pescare.
 * @param numeroCarteDaPescare Il numero di carte da pescare.
 * @return Puntatore all'array contenente le carte pescate, di dimensione 'numeroCarteDaPescare'.
 */
Carta* pescaCimaMazzo(Mazzo* mazzoPesca, int numeroCarteDaPescare) {
    // TODO: cosa fare se finiscono le carte del mazzoPesca?
    int i;
    Carta* cartePescate = NULL;

    // alloco dinamicamente un array che contiene le carte che saranno pescate dal 'mazzoPesca'
    cartePescate = (Carta*) calloc(numeroCarteDaPescare, sizeof(Carta));
    if(cartePescate == NULL) {
        printf("Errore: impossibile allocare dinamicamente il mazzoPesca di carte scartato. Arresto.");
        exit(-1); // TODO: utils
    }

    // inserisco le carte pescate in un array
    for(i = mazzoPesca->numeroCarte - 1; i >= 0; i--) {
        cartePescate[i] = mazzoPesca->carte[i];
    }

    // riduco il 'mazzoPesca' togliendo le carte pescate
    mazzoPesca->carte = (Carta*) realloc(mazzoPesca->carte, (mazzoPesca->numeroCarte - numeroCarteDaPescare) * sizeof(Carta));
    if(mazzoPesca->carte == NULL) {
        printf("Errore: impossibile allocare dinamicamente il mazzoPesca di carte scartato. Arresto.");
        exit(-1); // TODO: utils
    }
    // diminuisco il numero di carte presenti nel 'mazzoPesca'
    mazzoPesca->carte -= numeroCarteDaPescare;

    // restituisco le carte pescate
    return cartePescate;
}

/**
 * Funzione che estrae un numero fornito di carte.
 * Con il termine 'estrarre' si intende l'azione che prevede la pesca dal mazzo di pesca, e poi lo scarto immediato.
 * Questo significa che le carte estratte vengono restituite dalla funzione, ma non sono utilizzabili perché sono state
 * spostate nel mazzo degli scarti.
 *
 * @param mazzoPesca Puntatore al mazzo di pesca, da cui pescare.
 * @param mazzoScarti Puntatore al mazzo degli scarti, in cui inserire la carta scartata.
 * @param numeroCarteDaEstrarre Il numero di carte da estrarre.
 * @return Un puntatore all'array contenente le carte estratte (NON possono essere date a un giocatore)
 */
Carta *estraiCarte(Mazzo *mazzoPesca, Mazzo *mazzoScarti, int numeroCarteDaEstrarre) {
    int i;
    // pesco le carte dalla cima del mazzo
    Carta* cartePescate = pescaCimaMazzo(mazzoPesca, numeroCarteDaEstrarre);

    // aggiungo automaticamente al mazzo di scarti
    for(i = 0; i < numeroCarteDaEstrarre; i++) {
        aggiungiCartaMazzo(mazzoScarti, cartePescate[i]);
    }

    // restituisco le carte estratte
    return cartePescate;
}

/**
 * Restituisce l'indice di una carta all'interno di un mazzo dato un nome,
 * oppure -1 se la carta non viene trovata.
 *
 * @param mazzo Il mazzo in cui cercare la carta
 * @param nomeCarta Il nome della carta da cercare
 * @return L'indice della carta se viene trovata, -1 altrimenti
 */
int cercaCartaMazzoPerNome(Mazzo mazzo, char nomeCarta[NOME_CARTA_LEN + 1]) {
    int i;

    for(i = 0; i < mazzo.numeroCarte; i++) {
        if(strcmp(nomeCarta, mazzo.carte[i].nomeCarta) == 0)
            return i;
    }
    return -1;
}

void rimuoviCartaMazzo(Mazzo* mazzo, Carta carta) {
    int i;
    Carta* nuovoMazzo = NULL;

    mazzo->numeroCarte--;
    nuovoMazzo = (Carta*) calloc(mazzo->numeroCarte, sizeof(Carta));
    if(nuovoMazzo == NULL) {
        printf("\nImpossibile allocare dinamicamente memoria."); // TODO: spostare in utils
        exit(-1);
    }

    for(i = 0; i < mazzo->numeroCarte; i++) {
        if(strcmp(mazzo->carte[i].nomeCarta, carta.nomeCarta) == 0 &&
                mazzo->carte[i].numeroCarta == carta.numeroCarta &&
                mazzo->carte[i].semeCarta == carta.semeCarta)
            continue;
        nuovoMazzo[i] = mazzo->carte[i];
    }

    free(mazzo->carte);
    mazzo->carte = nuovoMazzo; // TODO: Da testare
}

void aggiungiCartaMazzo(Mazzo* mazzo, Carta carta) {
    // allargamento del mazzo di carte per far spazio alla nuova carta
    mazzo->numeroCarte++;
    mazzo->carte = (Carta*) realloc(mazzo->carte, mazzo->numeroCarte);
    if(mazzo->carte == NULL) {
        printf("\nErrore: impossibile allocare dinamicamente."); // TODO: 'utils'
        exit(-1);
    }

    // aggiunta della nuova carta
    mazzo->carte[mazzo->numeroCarte - 1] = carta;
}

/**
 * Subroutine che mostra a schermo le informazioni di una carta.
 *
 * @param carta La carta da mostrare.
 */
void mostraCarta(Carta carta) {
    // variabili di appoggio, contenenti il nome della tipologia e del seme della carta
    char tipologiaCarta[TIPOLOGIA_CARTA_LEN + 1], semeCarta[SEME_CARTA_LEN + 1];

    prendiTipologiaCarta(carta.tipologiaCarta, tipologiaCarta);
    prendiSemeCarta(carta.semeCarta, semeCarta);

    printf("\nNome: %s\n"
           "Tipologia: %s\n"
           "Numero della carta: %d\n"
           "Seme della carta: %s",
           carta.nomeCarta, tipologiaCarta, carta.numeroCarta, semeCarta);
}

/**
 * Subroutine che mostra le carte presenti all'interno di un mazzo.
 *
 * @param mazzo Il mazzo con le carte da stampare a schermo.
 */
void mostraCarteMazzo(Mazzo mazzo) {
    int i;

    if(mazzo.numeroCarte == 0)
        printf("\nNessuna carta presente!");
    for(i = 0; i < mazzo.numeroCarte; i++) {
        mostraCarta(mazzo.carte[i]);
    }
}
