#include "log.h"


/**
 * Subroutine che stampa sul file di log un separatore per indicare l'inizio del turno.
 *
 * @param Il turno corrente.
 */
void scriviInizioTurno(int turno) {
    // puntatore al file contenente i log
    FILE* logFile = NULL;

    // apertura del file di log
    logFile = fopen(LOG_FILE_NAME, "a");
    assertPuntatoreNonNull(logFile, "\nErrore: impossibile aprire il file di log.");

    // stampa separatore
    fprintf(logFile, "%s INIZIO TURNO %d %s\n", MEZZO_SEPARATORE, turno, MEZZO_SEPARATORE);

    fclose(logFile);
}

/**
 * Subroutine che scrive su log una carta estratta da un giocatore.
 *
 * @param nomeGiocatore Il nome del giocatore che ha estratto la carta.
 * @param carta La carta estratta dal giocatore.
 */
void scriviCartaEstrattaSuLog(char nomeGiocatore[], Carta carta) {
    // puntatore al file contenente i log
    FILE* logFile = NULL;
    // stringa contenente il seme della carta
    char semeCarta[SEME_CARTA_LEN + 1];

    // apertura del file di log
    logFile = fopen(LOG_FILE_NAME, "a");
    assertPuntatoreNonNull(logFile, "\nErrore: impossibile aprire il file di log.");

    // scrittura carta estratta
    prendiSemeCarta(carta.semeCarta, semeCarta);
    fprintf(logFile, "%s ha estratto una carta: %s, %d di %s.\n", nomeGiocatore, carta.nomeCarta, carta.numeroCarta, semeCarta);

    // chiusura del file
    fclose(logFile);
}

/**
 * Subroutine che stampa sui log la carta estratta in seguito all'effetto della dinamite.
 *
 * @param nomeGiocatore Il nome del giocatore che ha estratto una carta
 * @param cartaEstratta La carta estratta.
 */
void scriviDinamiteSuLog(char nomeGiocatore[], Carta cartaEstratta) {
    // puntatore al file contenente i log
    FILE* logFile = NULL;
    // stringa contenente il seme della carta
    char semeCarta[SEME_CARTA_LEN + 1];

    // apertura del file di log
    logFile = fopen(LOG_FILE_NAME, "a");
    assertPuntatoreNonNull(logFile, "\nErrore: impossibile aprire il file di log.");

    // scrittura carta estratta
    prendiSemeCarta(cartaEstratta.semeCarta, semeCarta);
    fprintf(logFile, "EVENTO DINAMITE! %s ha estratto una carta: %s, %d di %s.", nomeGiocatore, cartaEstratta.nomeCarta, cartaEstratta.numeroCarta, semeCarta);
    // verifico che la carta faccia esplodere la dinamite
    if(cartaEstratta.numeroCarta >= 2 && cartaEstratta.numeroCarta <= 9 && cartaEstratta.semeCarta == PICCHE) {
        fprintf(logFile, " La carta ha fatto esplodere la dinamite!\n");
    } else {
        fprintf(logFile, " La carta non ha fatto esplodere la dinamite.\n");
    }

    // chiusura del file
    fclose(logFile);
}

/**
 * Subroutine che scrive sui log informazioni sull'evento dinamite.
 *
 * @param nomeGiocatore Nome del giocatore che estrae la carta.
 * @param cartaEstratta La carta estratta.
 */
void scriviPrigioneSuLog(char nomeGiocatore[], Carta cartaEstratta) {
    // puntatore al file contenente i log
    FILE* logFile = NULL;
    // stringa contenente il seme della carta
    char semeCarta[SEME_CARTA_LEN + 1];

    // apertura del file di log
    logFile = fopen(LOG_FILE_NAME, "a");
    assertPuntatoreNonNull(logFile, "\nErrore: impossibile aprire il file di log.");

    // scrittura carta estratta
    prendiSemeCarta(cartaEstratta.semeCarta, semeCarta);
    fprintf(logFile, "EVENTO PRIGIONE! %s ha estratto una carta: %s, %d di %s.", nomeGiocatore, cartaEstratta.nomeCarta, cartaEstratta.numeroCarta, semeCarta);
    // verifico che l'utente sia evaso
    if(cartaEstratta.semeCarta == CUORI) {
        fprintf(logFile, " %s evade di prigione!\n", nomeGiocatore);
    } else {
        fprintf(logFile, " %s non evade, e salta il turno!.\n", nomeGiocatore);
    }

    // chiusura del file
    fclose(logFile);
}

/**
 * Subroutine che scrive su log una carta giocata contro un altro giocatore.
 *
 * @param nomeGiocatore Il giocatore che gioca la carta.
 * @param nomeAvversario Il giocatore contro cui la carta è giocata.
 * @param carta La carta giocata.
 */
void scriviCartaGiocataAvversarioSuLog(char nomeGiocatore[], char nomeAvversario[], Carta carta) {
    // puntatore al file contenente i log
    FILE* logFile = NULL;

    // apertura del file di log
    logFile = fopen(LOG_FILE_NAME, "a");
    assertPuntatoreNonNull(logFile, "\nErrore: impossibile aprire il file di log.");

    // scrittura carta
    fprintf(logFile, "%s ha giocato una carta '%s' contro %s.\n", nomeGiocatore, carta.nomeCarta, nomeAvversario);

    // chiusura del file
    fclose(logFile);
}

/**
 * Subroutine che scrive su log una carta giocata.
 *
 * @param nomeGiocatore Nome del giocatore che ha giocato la carta.
 * @param carta La carta giocata.
 */
void scriviCartaGiocataSuLog(char nomeGiocatore[], Carta carta) {
    // puntatore al file contenente i log
    FILE* logFile = NULL;

    // apertura del file di log
    logFile = fopen(LOG_FILE_NAME, "a");
    assertPuntatoreNonNull(logFile, "\nErrore: impossibile aprire il file di log.");

    // scrittura carta
    fprintf(logFile, "%s ha giocato una carta '%s'.\n", nomeGiocatore, carta.nomeCarta);

    // chiusura del file
    fclose(logFile);
}

/**
 * Subroutine che scrive su log le informazioni su una carta scartata.
 *
 * @param nomeGiocatore Il nome del giocatore che scarta la carta.
 * @param carta La carta scartata.
 */
void scriviCartaScartataSuLog(char nomeGiocatore[], Carta carta) {
    // puntatore al file contenente i log
    FILE* logFile = NULL;

    // apertura del file di log
    logFile = fopen(LOG_FILE_NAME, "a");
    assertPuntatoreNonNull(logFile, "\nErrore: impossibile aprire il file di log.");

    // scrittura carta
    fprintf(logFile, "%s ha scartato una carta '%s'.\n", nomeGiocatore, carta.nomeCarta);

    // chiusura del file
    fclose(logFile);
}

/**
 * Subroutine che scrive sui log una carta rubata da un giocatore a un altro tramite carta 'Panico!'.
 *
 * @param nomeGiocatore Il nome del giocatore che ruba la carta.
 * @param nomeAttaccato Il nome del giocatore a cui è rubata la carta.
 * @param cartaRubata La carta rubata.
 */
void scriviCartaRubataPanico(char nomeGiocatore[], char nomeAttaccato[], Carta cartaRubata) {
    // puntatore al file contenente i log
    FILE* logFile = NULL;

    // apertura del file di log
    logFile = fopen(LOG_FILE_NAME, "a");
    assertPuntatoreNonNull(logFile, "\nErrore: impossibile aprire il file di log.");

    // scrittura carta
    fprintf(logFile, "%s ha usato '%s!' e ruba una carta '%s' a '%s'!\n", nomeGiocatore, CARTA_PANICO, cartaRubata.nomeCarta, nomeAttaccato);

    // chiusura del file
    fclose(logFile);
}

/**
 *
 * @param nomeGiocatore
 * @param cartaEstratta
 */
void scriviBarileSuLog(char nomeGiocatore[], Carta cartaEstratta) {
    // puntatore al file contenente i log
    FILE* logFile = NULL;

    // apertura del file di log
    logFile = fopen(LOG_FILE_NAME, "a");
    assertPuntatoreNonNull(logFile, "\nErrore: impossibile aprire il file di log.");

    // scrittura carta
    fprintf(logFile, "BARILE! %s ha estratto una carta '%s'.", nomeGiocatore, cartaEstratta.nomeCarta);
    // verifico che l'attacco sia mancato
    if(cartaEstratta.semeCarta == CUORI) {
        fprintf(logFile, " Attacco mancato!\n");
    } else {
        fprintf(logFile, " Attacco non mancato!\n");
    }

    // chiusura del file
    fclose(logFile);
}

/**
 * Subroutine che scrive se un giocatore ha giocato una carta 'Mancato!' per evitare un attacco
 *
 * @param nomeGiocatore
 */
void scriviMancatoSuLog(char nomeGiocatore[]) {
    // puntatore al file contenente i log
    FILE* logFile = NULL;

    // apertura del file di log
    logFile = fopen(LOG_FILE_NAME, "a");
    assertPuntatoreNonNull(logFile, "\nErrore: impossibile aprire il file di log.");

    // scrittura carta giocata
    fprintf(logFile, "%s evita l'attacco giocando un '%s'!\n", nomeGiocatore, CARTA_MANCATO);

    // chiusura del file
    fclose(logFile);
}

/**
 * Subroutine che scrive sul file di log i punti vita persi di un giocatore.
 * Se il giocatore è stato ferito dal gioco e non da un altro player, allora nomeGiocatoreAttaccante == ""
 *
 * @param nomeGiocatoreAttaccato Nome del giocatore che perde punti vita.
 * @param nomeGiocatoreAttaccante Nome del giocatore che rimuove i punti vita.
 * @param puntiVitaPersi I punti vita persi.
 */
void scriviPuntiVitaPersiSuLog(char nomeGiocatoreAttaccato[], char nomeGiocatoreAttaccante[], int puntiVitaPersi) {
    // puntatore al file contenente i log
    FILE* logFile = NULL;

    // apertura del file di log
    logFile = fopen(LOG_FILE_NAME, "a");
    assertPuntatoreNonNull(logFile, "\nErrore: impossibile aprire il file di log.");

    // scrittura punti vita persi
    if(strcmp(nomeGiocatoreAttaccante, "") == 0) { // il giocatore non è stato attaccato
        fprintf(logFile, "%s perde %d punti vita.\n", nomeGiocatoreAttaccato, puntiVitaPersi);
    } else { // il giocatore è stato ferito da qualcuno
        fprintf(logFile, "%s toglie %d punti vita a %s.\n", nomeGiocatoreAttaccante, puntiVitaPersi, nomeGiocatoreAttaccato);
    }

    // chiusura del file
    fclose(logFile);
}

/**
 * Subroutine che scrive su log la morte di un giocatore.
 *
 * @param nomeGiocatoreMorto Il nome del giocatore morto.
 */
void scriviMorteGiocatoreSuLog(char nomeGiocatoreMorto[]) {
    // puntatore al file contenente i log
    FILE* logFile = NULL;

    // apertura del file di log
    logFile = fopen(LOG_FILE_NAME, "a");
    assertPuntatoreNonNull(logFile, "\nErrore: impossibile aprire il file di log.");

    // scrittura morte
    fprintf(logFile, "MORTE! %s ha perso tutti i suoi punti vita, e viene eliminato!\n", nomeGiocatoreMorto);

    // chiusura del file
    fclose(logFile);
}

/**
 * Subroutine che scrive sul file di log le carte pescate da un giocatore.
 *
 * @param nomeGiocatore Nome del giocatore che ha pescato.
 * @param mazzoMano Mazzo di pesca del giocatore.
 * @param nCarte Il numero di carte pescate.
 */
void scriviCartePescateSuLog(char nomeGiocatore[], Mazzo mazzoMano, int nCarte) {
    int i;
    // puntatore al file contenente i log
    FILE* logFile = NULL;

    // apertura del file di log
    logFile = fopen(LOG_FILE_NAME, "a");
    assertPuntatoreNonNull(logFile, "\nErrore: impossibile aprire il file di log.");

    fprintf(logFile, "%s ha pescato! Carte pescate:", nomeGiocatore);
    // stampo i nomi delle carte
    for(i = mazzoMano.numeroCarte - 1; i >= mazzoMano.numeroCarte - nCarte; i--) {
        fprintf(logFile, " '%s'", mazzoMano.carte[i].nomeCarta);
        if(i != mazzoMano.numeroCarte - nCarte) // aggiungo la virgola tra i nomi delle carte
            fprintf(logFile, ",");
    }
    fprintf(logFile, "\n");

    fclose(logFile);
}

void pulisciLog() {
    // puntatore al file contenente i log
    FILE* logFile = NULL;

    // apertura del file in scrittura per troncare il contenuto
    logFile = fopen(LOG_FILE_NAME, "w");
    assertPuntatoreNonNull(logFile, "\nErrore: impossibile aprire il file di log.");

    // chiusura del file senza scrivere alcun contenuto
    fclose(logFile);
}