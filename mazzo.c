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
    assertPuntatoreNonNull(fileMazzo, "\nErrore: impossibile aprire il file contenente le carte del mazzo.");

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
            // allocazione dinamica del puntatore alle carte del mazzo, per fare spazio alla nuova carta letta
            if(mazzo.numeroCarte == 0) { // se questa è la prima carta, allora alloco dinamicamente
                carteMazzo = (Carta*) calloc(mazzo.numeroCarte, sizeof(Carta));
            } else { // altrimenti, rialloco
                carteMazzo = (Carta*) realloc(carteMazzo, mazzo.numeroCarte * sizeof(Carta));
            }
            // verifica della corretta allocazione dinamica
            assertPuntatoreNonNull(carteMazzo, "\nErrore: impossibile allocare dinamicamente il mazzo di carte.");

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
    spostaCartaMazzo(mazzoPesca, mazzoScarti, mazzoPesca->numeroCarte - 1);

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
int cercaCartaMazzoPerNome(Mazzo mazzo, char nomeCarta[]) {
    int i = 0;

    // cerco per tutto il mazzo finché non trovo la carta con il nome dato
    while(i < mazzo.numeroCarte) {
        if(strcmp(nomeCarta, mazzo.carte[i].nomeCarta) == 0)
            return i;
        i++;
    }

    // restituisco -1 se non trovo nulla
    return -1;
}

void rimuoviCartaMazzo(Mazzo* mazzo, int posizioneCarta) {
    int i;

    // diminuisco di uno il numero di carte
    mazzo->numeroCarte--;

    // se il mazzo è vuoto, libero la memoria
    if(mazzo->numeroCarte == 0 && mazzo->carte != NULL) {
        free(mazzo->carte);
        mazzo->carte = NULL;
    } else if(mazzo->numeroCarte > 0) {
        // a partire dalla posizione della carta da rimuovere, sposto tutte le carte indietro di uno
        for(i = posizioneCarta; i <= mazzo->numeroCarte - 1; i++) {
            mazzo->carte[i] = mazzo->carte[i+1];
        }

        // rialloco la memoria dell'array di carte diminuendo lo spazio di uno
        mazzo->carte = (Carta*) realloc(mazzo->carte, sizeof(Carta) * mazzo->numeroCarte);
        assertPuntatoreNonNull(mazzo->carte, "\nErrore: la riallocazione dinamica del mazzo di carte dopo la rimozione non è riuscita.");
    } else {
        printf("\nErrore: il numero di carte in un mazzo non può essere negativo.");
        exit(-1);
    }
}

/**
 * Subroutine che aggiunge una carta sulla cima di un mazzo fornito.
 * Se la carta è la prima ad essere aggiunta, la funzione inizializza il campo "carte" della struttura "mazzo"
 * allocandone dinamicamente la memoria.
 * Se la carta non è la prima ad essere aggiunta, allora la funzione allarga la dimensione dell'array di un'unità.
 *
 * @param mazzo Puntatore al mazzo di carte a cui si vuole aggiungere una carta.
 * @param carta La carta che si intende aggiungere al mazzo
 */
void aggiungiCartaMazzo(Mazzo* mazzo, Carta carta) {
    // allargamento del mazzo di carte per far spazio alla nuova carta
    mazzo->numeroCarte++;

    // allocazione dinamica e verifica
    if(mazzo->numeroCarte == 1 && mazzo->carte == NULL) { // primo caso, il mazzo è nullo, quindi lo alloco dinamicamente
        mazzo->carte = (Carta*) calloc(mazzo->numeroCarte, sizeof(Carta));
    } else if(mazzo->numeroCarte > 1 && mazzo->carte != NULL) { // secondo caso, il mazzo ha già delle carte, quindi rialloco soltanto
        mazzo->carte = (Carta*) realloc(mazzo->carte, mazzo->numeroCarte * sizeof(Carta));
    } else { // terzo caso, un ibrido tra i due, quindi non posso allocare correttamente (il mazzo dovrebbe avere già una carta, ma il puntatore è NULL)
        printf("\nErrore: il numero di carte del mazzo è %d, ma il puntatore contenente le carte è nullo.", mazzo->numeroCarte);
        exit(-1);
    }
    assertPuntatoreNonNull(mazzo->carte, "\nErrore: impossibile effettuare l'allocazione dinamica del mazzo dopo l'aggiunta di una carta.");

    // aggiunta della nuova carta alla cima del mazzo
    mazzo->carte[mazzo->numeroCarte - 1] = carta;
}

/**
 * Subroutine che sposta una carta da un mazzo all'altro.
 * Equivalente ad effettuare aggiungiCarta(partenza, Carta) && rimuoviCarta(destinazione, Carta).
 *
 * @param partenza Puntatore al mazzo da cui spostare la carta.
 * @param destinazione Puntatore al mazzo in cui spostare la carta.
 * @param posizioneCarta Posizione della carta da spostare da un mazzo all'altro.
 */
void spostaCartaMazzo(Mazzo* partenza, Mazzo* destinazione, int posizioneCarta) {
    aggiungiCartaMazzo(destinazione, partenza->carte[posizioneCarta]);
    rimuoviCartaMazzo(partenza, posizioneCarta);
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

    if(mazzo.numeroCarte > 0) {
        for(i = 0; i < mazzo.numeroCarte; i++) {
            printf("\n%d) ", i+1);
            mostraCarta(mazzo.carte[i]);
            printf("\n");
        }
    } else {
        printf("\nNessuna carta presente!\n");
    }

    printf("%s CARTE %s\n", MEZZO_SEPARATORE, MEZZO_SEPARATORE);
}

/**
 * Subroutine che scambia le carte del mazzo di pesca con quelle del mazzo di scarti.
 * Deve essere chiamata solo se il mazzo di pesca è vuoto, altrimenti genera un errore.
 *
 * @param mazzoPesca Puntatore al mazzo di pesca.
 * @param mazzoScarti Puntatore al mazzo di scarti.
 */
void scambiaPescaScarti(Mazzo* mazzoPesca, Mazzo* mazzoScarti) {
    printf("\nIl mazzo di pesca è vuoto! Rigenerazione dal mazzo di scarti in corso...\n");

    if(mazzoPesca->numeroCarte != 0) {
        printf("\nErrore: non è possibile scambiare il mazzo di pesca con quello di scarti se il primo non è vuoto.");
        exit(-1);
    }

    // scambio dei mazzi
    mazzoPesca->carte = mazzoScarti->carte;
    mazzoPesca->numeroCarte = mazzoScarti->numeroCarte;
    mazzoScarti->carte = NULL; // se mazzoPesca è vuoto, mazzoPesca.carte è NULL, quindi non è necessario liberare la memoria.
    mazzoScarti->numeroCarte = 0;
    // mischio il mazzo di pesca nuovo
    mischiaMazzo(mazzoPesca);
}