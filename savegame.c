#include "savegame.h"

/**
 * Legge dal file "SAVEGAME_DIR/nomeSalvataggio" il salvataggio in cui è contenuta una partita,
 * e restituisce una struttura avente per campi tutti i parametri di quel salvataggio.
 *
 * @param nomeSalvataggio Il nome del file in cui è contenuto il salvataggio.
 * @return Una struttura contenente il salvataggio caricato.
 */
Salvataggio caricaSalvataggio(char nomeSalvataggio[SAVEGAME_NAME_LEN + 1]) {
    // numero di byte letti per ogni operazione
    size_t read, readPlayer, readMano, readGioco;
    // struct salvataggio e file da aprire per leggere i suoi campi
    Salvataggio salvataggio;
    FILE* salvataggioFile = NULL;

    salvataggioFile = fopen(strcat(SAVEGAME_DIR, nomeSalvataggio), "rb");
    if(salvataggioFile == NULL) {
        printf("\nImpossibile aprire il file di salvataggio."); // TODO: nel caso spostare queste funzioni in un file 'utils'
        exit(-1);
    }

    // lettura del file di salvataggio
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

    // aggiunta del nome del salvataggio
    strcpy(salvataggio.nomeSalvataggio, nomeSalvataggio);

    // chiusura del file
    fclose(salvataggioFile);

    // restituisci la struct del salvataggio letto
    return salvataggio;
}

void scriviSalvataggio(Salvataggio salvataggio, char nomeSalvataggio[SAVEGAME_NAME_LEN]) {
    // numero di byte scritti per ogni operazione
    size_t write, writePlayer, writeMano, writeGioco;
    // file da aprire per la scrittura
    FILE* salvataggioFile = NULL;

    salvataggioFile = fopen(strcpy(SAVEGAME_DIR, nomeSalvataggio), "wb");
    if(salvataggioFile == NULL) {
        printf("\nErrore durante la scrittura del file di salvataggio.");
        exit(-1);
    }
    // scrittura del numero di giocatori
    write = fwrite(&salvataggio.nGiocatori, sizeof(int), 1, salvataggioFile);
    if(write != 1) {
        printf("\nErrore durante la scrittura del file di salvataggio: impossibile scrivere il numero di player.");
        exit(-1);
    }
    // scrittura degli nGiocatori
    for(int i = 0; i < salvataggio.nGiocatori; i++) {
        writePlayer = fwrite(&salvataggio.giocatori[i], sizeof(Giocatore), 1, salvataggioFile);
        writeMano = fwrite(&salvataggio.giocatori[i].carteMano, sizeof(Mazzo), 1, salvataggioFile);
        writeGioco = fwrite(&salvataggio.giocatori[i].carteGioco, sizeof(Mazzo), 1, salvataggioFile);
        if(writePlayer != 1 || writeMano != 1 || writeGioco != 1) {
            printf("\nErrore durante la scrittura del file di salvataggio.");
            exit(-1);
        }
    }
    // scrittura del prossimo giocatore
    write = fwrite(&salvataggio.prossimoGiocatore, sizeof(int), 1, salvataggioFile);
    if(write != 1) {
        printf("\nErrore durante la scrittura del file di salvataggio: impossibile scrivere la posizione del prossimo giocatore.");
        exit(-1);
    }
    // scrittura del mazzo di pesca
    write = fwrite(&salvataggio.mazzoPesca, sizeof(Mazzo), 1, salvataggioFile);
    if(write != 1) {
        printf("\nErrore durante la scrittura del file di salvataggio: impossibile scrivere il mazzo di pesca.");
        exit(-1);
    }
    // scrittura del mazzo di scarti
    write = fwrite(&salvataggio.mazzoScarti, sizeof(Mazzo), 1, salvataggioFile);
    if(write != 1) {
        printf("\nErrore durante la scrittura del file di salvataggio: impossibile scrivere il mazzo di scarti.");
        exit(-1);
    }

    // chiusura del file
    fclose(salvataggioFile);
}