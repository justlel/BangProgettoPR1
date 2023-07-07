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
    // numero di byte letti dalla funzione fscanf
    int read;
    // carta letta in ogni iterazione del ciclo dalla funzione fscanf
    Carta cartaLetta;
    // puntatore allocato dinamicamente, che conterrà le carte da inserire nel mazzo
    Carta* carteMazzo = NULL;
    // puntatore al file contenente le carte da caricare attraverso fopen()
    FILE* fileMazzo = NULL;

    // apertura del file
    fileMazzo = fopen("mazzo_bang.txt", "r");
    // verifica della corretta apertura
    assertPuntatoreNonNull(fileMazzo, "\nErrore: impossibile aprire il file contenente le carte del mazzo. Arresto.");

    // inizializzazione del mazzo che sarà poi restituito dalla funzione
    Mazzo mazzo = {MAZZO_PESCA, 0, NULL};

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
            assertPuntatoreNonNull(carteMazzo, "\nErrore: impossibile allocare dinamicamente il mazzo di carte. Arresto.");

            // l'ultima carta letta diventa l'ultima carta del mazzo
            carteMazzo[mazzo.numeroCarte - 1] = cartaLetta;
        }
    } while(read == 4);

    // chiusura del file
    fclose(fileMazzo);

    // il puntatore contenente le carte lette viene inserito nella struct "mazzo"
    mazzo.carte = carteMazzo;

    // il mazzo con le carte letto viene ora passato come puntatore a "mischiaMazzo" per randomizzare l'ordine delle carte
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
 * @param mazzoScarti Puntatore al mazzo di scarti.
 * @param numeroCarteDaPescare Il numero di carte da pescare.
 * @return Puntatore all'array contenente le carte pescate, di dimensione 'numeroCarteDaPescare'.
 */
Carta *pescaCimaMazzo(Mazzo *mazzoPesca, Mazzo *mazzoScarti, int numeroCarteDaPescare) {
    // TODO: cosa fare se finiscono le carte del mazzoPesca?
    int i, j, carteRimanenti;
    Carta *cartePescate = NULL, *ultimeCarte = NULL, *primeCarte = NULL;

    // alloco dinamicamente un array che contiene le carte che saranno pescate dal 'mazzoPesca'
    cartePescate = (Carta*) calloc(numeroCarteDaPescare, sizeof(Carta));
    if(cartePescate == NULL) {
        printf("Errore: impossibile allocare dinamicamente il mazzoPesca di carte scartato. Arresto.");
        exit(-1); // TODO: utils
    }

    // se le carte da pescare sono maggiori del numero di carte del mazzo, allora si pescano le rimanenti,
    // si riempie il mazzo di pesca con il mazzo degli scarti (rimischiato) e si pescano le carte mancanti
    if(numeroCarteDaPescare > mazzoPesca->numeroCarte) {
        // allocazione dinamica per gli array supplementari delle carte da pescare
        ultimeCarte = (Carta*) calloc(mazzoPesca->numeroCarte, sizeof(Carta));
        primeCarte  = (Carta*) calloc(numeroCarteDaPescare - mazzoPesca->numeroCarte, sizeof(Carta));
        if(ultimeCarte == NULL || primeCarte == NULL) {
            printf("Errore: impossibile allocare dinamicamente gli array delle carte da pescare.");
            exit(-1);
        }

        printf("\nIl mazzo di pesca è vuoto! Rigenerazione dal mazzo di scarti in corso...");
        // chiamata ricorsiva per pescare le ultime carte del mazzo
        ultimeCarte = pescaCimaMazzo(mazzoPesca, mazzoScarti, mazzoPesca->numeroCarte);
        // segno da parte il numero delle carte che devono ancora essere pescate
        carteRimanenti = numeroCarteDaPescare - mazzoPesca->numeroCarte;

        // le carte del mazzo di scarti diventano quelle del mazzo di pesca, e vengono mischiate
        mazzoPesca->carte = mazzoScarti->carte;
        mischiaMazzo(mazzoPesca);

        // svuoto il mazzo degli scarti
        mazzoScarti->numeroCarte = 0;
        mazzoScarti->carte = (Carta*) calloc(sizeof(Carta), 0);

        // pesco le carte rimanenti da prima
        primeCarte = pescaCimaMazzo(mazzoPesca, mazzoScarti, carteRimanenti);
        // infine, unisco le carte pescate prima e dopo la rigenerazione del mazzo
        for(i = 0; i < numeroCarteDaPescare - carteRimanenti; i++) {
            cartePescate[i] = ultimeCarte[i];
        }
        for(i = numeroCarteDaPescare - carteRimanenti, j = 0; i < numeroCarteDaPescare && j < (numeroCarteDaPescare - carteRimanenti); i++, j++) {
            cartePescate[i] = primeCarte[j];
        }

        // free
        free(ultimeCarte);
        free(primeCarte);
    // pesca standard
    } else {
        // inserisco le carte pescate in un array
        for(i = mazzoPesca->numeroCarte - 1; i >= 0; i--) {
            cartePescate[i] = mazzoPesca->carte[i];
        }
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
 * Funzione che estrae una carta da un mazzo.
 * Con il termine 'estrarre' si intende l'azione che prevede la pesca dal mazzo di pesca, e poi lo scarto immediato.
 * Questo significa che le carte estratte vengono restituite dalla funzione, ma non sono utilizzabili perché sono state
 * spostate nel mazzo degli scarti.
 *
 * @param mazzoPesca Puntatore al mazzo di pesca, da cui pescare.
 * @param mazzoScarti Puntatore al mazzo degli scarti, in cui inserire la carta scartata.
 * @return Un puntatore all'array contenente le carte estratte (NON possono essere date a un giocatore)
 */
Carta estraiCarta(Mazzo *mazzoPesca, Mazzo *mazzoScarti) {
    // carta estratta
    Carta cartaEstratta;

    // prendo l'ultima carta dal mazzo di pesca
    cartaEstratta = mazzoPesca->carte[mazzoPesca->numeroCarte - 1];

    // sposto la carta dal mazzo di pesca al mazzo degli scarti
    aggiungiCartaMazzo(mazzoScarti, cartaEstratta);
    rimuoviCartaMazzo(mazzoPesca, mazzoPesca->numeroCarte - 1);

    // restituisco la carta estratta
    return cartaEstratta;
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

void rimuoviCartaMazzo(Mazzo* mazzo, int posizioneCarta) {
    int i;
    //Carta* nuovoMazzo = NULL;

    // dimunisco di uno il numero di carte
    mazzo->numeroCarte--;

    //nuovoMazzo = (Carta*) calloc(mazzo->numeroCarte, sizeof(Carta));
    //assertPuntatoreNonNull(nuovoMazzo, "\nImpossibile allocare dinamicamente memoria.");

    // se il mazzo è vuoto, libero la memoria
    if(mazzo->numeroCarte == 0) {
        free(mazzo->carte);
        mazzo->carte = NULL;
    } else if(mazzo->numeroCarte > 0) {
        // altrimenti, rialloco la memoria dell'array di carte diminuendo lo spazio di uno
        // a partire dalla posizione della carta da rimuovere, sposto tutte le carte indietro di uno
        for(i = posizioneCarta; i <= mazzo->numeroCarte - 1; i++) {
            mazzo->carte[i] = mazzo->carte[i+1];
        }

        mazzo->carte = (Carta*) realloc(mazzo->carte, sizeof(Carta) * mazzo->numeroCarte);
        assertPuntatoreNonNull(mazzo->carte, "\nErrore: la riallocazione dinamica del mazzo di carte dopo la rimozione non è riuscita.");
    } else {
        printf("\nErrore: il numero di carte in un mazzo non può essere negativo.");
        exit(-1);
    }
}

void aggiungiCartaMazzo(Mazzo* mazzo, Carta carta) {
    // allargamento del mazzo di carte per far spazio alla nuova carta
    mazzo->numeroCarte++;

    // allocazione dinamica e verifica
    if(mazzo->carte == NULL) {
        mazzo->carte = (Carta*) malloc(sizeof(Carta));
    } else {
        mazzo->carte = (Carta*) realloc(mazzo->carte, mazzo->numeroCarte * sizeof(Carta));
    }
    assertPuntatoreNonNull(mazzo->carte, "\nErrore: impossibile allocare dinamicamente.");

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

    printf("Nome: %s\n"
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

    printf("\n%s CARTE %s", MEZZO_SEPARATORE, MEZZO_SEPARATORE);

    if(mazzo.numeroCarte == 0)
        printf("\nNessuna carta presente!\n");
    for(i = 0; i < mazzo.numeroCarte; i++) {
        printf("\n%d) ", i+1);
        mostraCarta(mazzo.carte[i]);
        printf("\n");
    }

    printf("%s-------%s", MEZZO_SEPARATORE, MEZZO_SEPARATORE);
}