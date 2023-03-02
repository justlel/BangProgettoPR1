#include "partita.h"


void avvioGioco() {
    // ----------------- LOGICA DI AVVIO DEL GIOCO -------------------
    printf(" ____          _   _  _____ _   _____  _____  __   ______ _____ _____ _______ _____ ____  _   _ \n"
           "|  _ \\   /\\   | \\ | |/ ____| | |  __ \\|  __ \\/_ | |  ____|  __ \\_   _|__   __|_   _/ __ \\| \\ | |\n"
           "| |_) | /  \\  |  \\| | |  __| | | |__) | |__) || | | |__  | |  | || |    | |    | || |  | |  \\| |\n"
           "|  _ < / /\\ \\ | . ` | | |_ | | |  ___/|  _  / | | |  __| | |  | || |    | |    | || |  | | . ` |\n"
           "| |_) / ____ \\| |\\  | |__| |_| | |    | | \\ \\ | | | |____| |__| || |_   | |   _| || |__| | |\\  |\n"
           "|____/_/    \\_\\_| \\_|\\_____(_) |_|    |_|  \\_\\|_| |______|_____/_____|  |_|  |_____\\____/|_| \\_|\n\n");
    printf("Benvenuto, sfidante, in Bang! Il gioco da tavolo organizzato per il progetto finale"
           "del corso di Programmazione 1 dell'Università di Cagliari!\n");
    printf("Per iniziare, dimmi, preferisci creare una partita da zero, oppure caricare un file di salvataggio?\n");

    printf("%d) Carica Partita\n"
           "%d) Crea Nuova Partita\n"
           "%c) Esci dal gioco\n", PROMPT_CARICA_PARTITA, PROMPT_EXIT, PROMPT_EXIT);
    char choice;
    do {
        printf("?) ");
        choice = getchar();
        if(choice != PROMPT_CARICA_PARTITA && choice != PROMPT_NUOVA_PARTITA && choice != PROMPT_EXIT)
            printf("\nInserisci una delle opzioni possibili!\n");
    } while (choice != PROMPT_CARICA_PARTITA && choice != PROMPT_NUOVA_PARTITA && choice != PROMPT_EXIT);

    // da qui, il controllo del flusso di esecuzione è affidato a una delle due funzioni che si occupano di creare/caricare
    // una partita, oppure il programma termina sotto scelta dell'utente.
    switch (choice) {
        case PROMPT_CARICA_PARTITA:
            caricaSalvataggio();
            break;
        case PROMPT_NUOVA_PARTITA:
            creaPartita();
            break;
        case PROMPT_EXIT:
            chiudiGioco();
            break;
    }
}

void creaPartita() {
    printf("\nBene, iniziamo allora! Bang! è un gioco multiplayer, a cui è possibile "
           "giocare in compagnia da un minimo di %d a un massimo di %d persone. Per cominciare, "
           "perché non mi dici quanti giocatori sarete?\n", MIN_PLAYERS, MAX_PLAYERS);

    int nGiocatori;
    do {
        printf("?) ");
        scanf("%d", &nGiocatori);
        if(nGiocatori < MIN_PLAYERS || nGiocatori > MAX_PLAYERS)
            printf("Devi inserire un numero compreso tra %d e %d!", MIN_PLAYERS, MAX_PLAYERS);
    } while (nGiocatori < MIN_PLAYERS || nGiocatori > MAX_PLAYERS);

    Giocatore* giocatori = NULL;
    giocatori = (Giocatore*) calloc(nGiocatori, sizeof(Giocatore));
    // TODO: eventualmente, mettere questo codice di controllo in un'altra funzione
    if(giocatori == NULL) {
        printf("\nErrore: allocazione dinamica dell'array dei giocatori fallito, arresto.");
        exit(-1);
    }

    printf("Perfetto, dimmi un po' di più sui partecipanti! Cominciamo dai loro nomi: inserisci il nome di ogni"
           "giocatore, tenendo a mente che non possono contenere spazi ed hanno una lunghezza massima di %d caratteri.", NOME_UTENTE_LEN);
    for(int i = 0; i < nGiocatori; i++) {
        printf("\nGiocatore %d: ", i+1);
        scanf("%49s", giocatori[i].nomeUtente);
    }

    assegnaRuoli(giocatori, nGiocatori);
    printf("Piacere di conoscervi! Adesso genererò randomicamente i ruoli di ognuno di voi.\n");
}

void assegnaRuoli(Giocatore giocatori[], int nGiocatori) {
    int ruoliGiocatori[ROLES_NUMBER];

    ruoliGiocatori[SCERIFFO] = 1;
    ruoliGiocatori[RINNEGATO] = 1;

    switch (nGiocatori) {
        case 4:
            ruoliGiocatori[FUORILEGGE] = 2;
            ruoliGiocatori[VICESCERIFFO] = 0;
            break;
        case 5:
            ruoliGiocatori[FUORILEGGE] = 2;
            ruoliGiocatori[VICESCERIFFO] = 1;
            break;
        case 6:
            ruoliGiocatori[FUORILEGGE] = 3;
            ruoliGiocatori[VICESCERIFFO] = 1;
            break;
        case 7:
            ruoliGiocatori[FUORILEGGE] = 3;
            ruoliGiocatori[VICESCERIFFO] = 2;
            break;
        default:
            printf("Errore: numero di giocatori non riconosciuto, impossibile assegnare i ruoli. Arresto.");
    }

    printf("\nEssendo il numero dei partecipanti %d, i ruoli saranno divisi come segue:\n"
           "%d Sceriffo"
           "%d Rinnegato"
           "%d Fuorilegge"
           "%d Vicesceriff%c", nGiocatori,
           ruoliGiocatori[SCERIFFO], ruoliGiocatori[RINNEGATO], ruoliGiocatori[FUORILEGGE], ruoliGiocatori[VICESCERIFFO],
           ruoliGiocatori[VICESCERIFFO] != 1 ? 'i' : '0'); // TODO: vedere se lasciarlo o rimuoverlo

    srand(time(NULL));
    for(int i = 0; i < nGiocatori; i++) {
        int ruoloGenerato;
        do {
            ruoloGenerato = rand() % 4;
            if(ruoliGiocatori[ruoloGenerato] > 0) {
                ruoliGiocatori[ruoloGenerato] -= 1;
                giocatori[i].ruoloAssegnato = ruoloGenerato;
                giocatori[i].puntiVita = ruoloGenerato == SCERIFFO ? PUNTI_VITA_SCERIFFO : PUNTI_VITA_GENERICO;
            }
        } while (ruoliGiocatori[ruoloGenerato] <= 0);
    }
}

/**
 * Chiude il gioco salutando il giocatore, se questo ha deciso di interrompere la partita in un prompt.
 */
void chiudiGioco() {
    printf("Grazie per aver giocato, a presto!");
    exit(0);
}