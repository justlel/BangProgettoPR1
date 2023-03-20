#include "savegame.h"

/**
 * Legge dal file "SAVEGAME_DIR/nomeSalvataggio" il salvataggio in cui è contenuta una partita,
 * e restituisce una struttura avente per campi tutti i parametri di quel salvataggio.
 *
 * @param nomeSalvataggio Il nome del file in cui è contenuto il salvataggio.
 * @return Una struttura contenente il salvataggio caricato.
 */
Salvataggio caricaSalvataggio(char nomeSalvataggio[SAVEGAME_NAME_LEN]) {
    // struct salvataggio e file da aprire per leggere i suoi campi
    Salvataggio salvataggio;
    FILE* salvataggioFile = NULL;

    salvataggioFile = fopen(strcat(SAVEGAME_DIR, nomeSalvataggio), "rb");
    if(salvataggioFile == NULL) {
        printf("\nImpossibile aprire il file di salvataggio."); // TODO: nel caso spostare queste funzioni in un file 'utils'
        exit(-1);
    }

    // lettura del file di salvataggio
    int read, readPlayer, readMano, readGioco;
    // numero di giocatori
    read = fread(&salvataggio.nGiocatori, sizeof(int), 1, salvataggioFile);
    if(read != 1) {
        printf("\nErrore durante la lettura del file di salvataggio '%s': numero di giocatori non determinato.", nomeSalvataggio);
        exit(-1);
    }
    // lettura dei blocchi di giocatori
    Giocatore giocatoreLetto;
    for(int i = 0; i < salvataggio.nGiocatori; i++) {
        readPlayer = fread(&giocatoreLetto, sizeof(Giocatore), 1, salvataggioFile);
        readMano = fread(&giocatoreLetto.carteMano, sizeof(Mazzo), 1, salvataggioFile);
        readGioco = fread(&giocatoreLetto.carteGioco, sizeof(Mazzo), 1, salvataggioFile);
        if(readPlayer != 1 || readMano != 1 || readGioco != 1) {
            printf("\nErrore durante la lettura del file di salvataggio '%s': il giocatore %d ha un numero di informazioni insufficienti.",
                   nomeSalvataggio, i);
            exit(-1);
        }
    }
    // lettura del numero del prossimo giocatore che deve giocare
    read = fread(&salvataggio.prossimoGiocatore, sizeof(int), 1, salvataggioFile);
    if(read != 1) {
        printf("\nErrore durante la lettura del file di salvataggio '%s': non è stato fornito il numero del prossimo giocatore.", nomeSalvataggio);
        exit(-1);
    }
    // lettura del mazzo di pesca
    read = fread(&salvataggio.mazzoPesca, sizeof(Mazzo), 1, salvataggioFile);
    if(read != 1) {
        printf("\nErrore durante la lettura del file di salvataggio '%s': non è stato fornito il mazzo di pesca.", nomeSalvataggio);
        exit(-1);
    }
    // lettura del mazzo di scarti
    read = fread(&salvataggio.mazzoScarti, sizeof(Mazzo), 1, salvataggioFile);
    if(read != 1) {
        printf("\nErrore durante la lettura del file di salvataggio '%s': non è stato fornito il mazzo di scarti.", nomeSalvataggio);
        exit(-1);
    }

    // restituisci la struct del salvataggio letto
    return salvataggio;
}

