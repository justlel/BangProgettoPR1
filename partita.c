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

    printf("%c) Carica Partita\n"
           "%c) Crea Nuova Partita\n"
           "%c) Esci dal gioco\n", PROMPT_CARICA_PARTITA, PROMPT_NUOVA_PARTITA, PROMPT_EXIT);
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
            // caricaSalvataggio();
            printf("Opzione non ancora disponibile!");
            chiudiGioco();
            break;
        case PROMPT_NUOVA_PARTITA:
            creaPartita();
            break;
        case PROMPT_EXIT:
            chiudiGioco();
            break;
    }
}

/**
 * Subroutine che genera una partita a partire dalle informazioni fornite dal giocatore.
 */
void creaPartita() {
    printf("\nBene, iniziamo allora! Bang! è un gioco multiplayer, a cui è possibile "
           "giocare in compagnia da un minimo di %d a un massimo di %d persone.\n"
           "Per cominciare, perché non mi dici quanti giocatori sarete?\n", MIN_PLAYERS, MAX_PLAYERS);

    // richiesta all'utente del numero dei giocatori che intendono partecipare
    int nGiocatori;
    do {
        printf("?) ");
        scanf("%d", &nGiocatori);
        if(nGiocatori < MIN_PLAYERS || nGiocatori > MAX_PLAYERS)
            printf("Devi inserire un numero compreso tra %d e %d!\n", MIN_PLAYERS, MAX_PLAYERS);
    } while (nGiocatori < MIN_PLAYERS || nGiocatori > MAX_PLAYERS); // validazione dell'input

    // allocazione dinamica di un array contenente le strutture dei giocatori partecipanti
    Giocatore* giocatori = NULL;
    giocatori = (Giocatore*) calloc(nGiocatori, sizeof(Giocatore));
    // TODO: eventualmente, mettere questo codice di controllo in un'altra funzione
    // verifica dell'allocazione dinamica
    if(giocatori == NULL) {
        printf("\nErrore: allocazione dinamica dell'array dei giocatori fallito, arresto.");
        exit(-1);
    }

    // richiesta all'utente dei nickname dei giocatori
    printf("Perfetto, dimmi un po' di più sui partecipanti!\n"
           "Cominciamo dai loro nomi: inserisci il nome di ogni giocatore, tenendo a mente che\n"
           "non possono contenere spazi ed hanno una lunghezza massima di %d caratteri.", NOME_UTENTE_LEN);
    for(int i = 0; i < nGiocatori; i++) {
        printf("\nGiocatore %d: ", i+1);
        scanf("%49s", giocatori[i].nomeUtente);
    }

    // assegnazione dei ruoli dei giocatori
    printf("Piacere di conoscervi! Adesso genererò randomicamente i ruoli di ognuno di voi.\n");
    assegnaRuoli(giocatori, nGiocatori);

    // caricamento del mazzo dal file "mazzo_bang.txt"
    printf("\nI ruoli sono stati assegnati! Carico il mazzo di carte dal file di testo...");
    Mazzo mazzo = caricaMazzo();

    printf("\nMazzo caricato! Adesso distribuirò le carte dalla cima del mazzo di pesca. Vi ricordo "
           "che ad ogni giocatore spetta un numero di carte pari al numero dei suoi punti vita di partenza");

    // distribuzione delle carte dal mazzo ai giocatori
    distribuisciCartePartenza(&mazzo, giocatori, nGiocatori);

    printf("\nBene, è tutto pronto! Che la partita abbia inizio, e buona fortuna ai partecipanti!");
}

/**
 * Subroutine che assegna randomicamente i ruoli di ogni giocatore.
 * La distribuzione dei ruoli segue il numero dei partecipanti:
 * * 4 giocatori -> 1 sceriffo, 1 rinnegato, 2 fuorilegge, 0 vicesceriffi
 * * 5 giocatori -> 1 sceriffo, 1 rinnegato, 2 fuorilegge, 1 vicesceriffo
 * * 6 giocatori -> 1 sceriffo, 1 rinnegato, 3 fuorilegge, 1 vicesceriffo
 * * 7 giocatori -> 1 sceriffo, 1 rinnegato, 3 fuorilegge, 2 vicesceriffi
 *
 * @param giocatori L'array contenente i giocatori.
 * @param nGiocatori Il numero totale di giocatori.
 */
void assegnaRuoli(Giocatore giocatori[], int nGiocatori) {
    // definizione dell'array contenente i ruoli dei giocatori
    int ruoliGiocatori[ROLES_NUMBER];

    // il numero di sceriffi e di rinnegati è sempre uguale
    ruoliGiocatori[SCERIFFO] = 1;
    ruoliGiocatori[RINNEGATO] = 1;

    // calcolo del numero di guorilegge e vicesceriffi in base al numero di giocatori
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
            printf("\nErrore: numero di giocatori non riconosciuto, impossibile assegnare i ruoli. Arresto.");
    }

    // stampa a schermo della distribuzione dei ruoli
    printf("Essendo il numero dei partecipanti %d, i ruoli saranno divisi come segue:\n"
           "Sceriffi: %d\n"
           "Rinnegati: %d\n"
           "Fuorilegge: %d\n"
           "Vicesceriffi: %d\n",
           nGiocatori, ruoliGiocatori[SCERIFFO], ruoliGiocatori[RINNEGATO], ruoliGiocatori[FUORILEGGE], ruoliGiocatori[VICESCERIFFO]);

    // inizializzazione del seed per la generazione randomica
    srand(time(NULL));

    // condizione booleana che, se verificata, determina la fine del processo di assegnazione dei ruoli
    bool fineAssegnazione;

    // iterazione sui giocatori
    for(int i = 0; i < nGiocatori; i++) {
        int ruoloGenerato; // intero generato randomicamente, che indica un ruolo
        do {
            // TODO: attenzione: se si cambiano i valori dell'enum "Ruoli", la generazione va a puttane, ricontrolla uwu
            ruoloGenerato = rand() % 4;
            // se ruoliGiocatori[ruoloGenerato] > 0, ci sono ancora dei "posti liberi" per assegnare quel ruolo
            if(ruoliGiocatori[ruoloGenerato] > 0) {
                // un ruolo uguale a "ruoloGenerato" è stato assegnato, quindi si diminuisce di 1 il numero di disponibili
                ruoliGiocatori[ruoloGenerato] -= 1;
                // assegnazione ruolo
                giocatori[i].ruoloAssegnato = ruoloGenerato;
                // calcolo dei punti vita
                if(ruoloGenerato == SCERIFFO) {
                    printf("\n%s sarà lo sceriffo di questa partita!\n", giocatori[i].nomeUtente);
                    giocatori[i].puntiVita = PUNTI_VITA_SCERIFFO;
                } else {
                    giocatori[i].puntiVita = PUNTI_VITA_GENERICO;
                }
            }
            // TODO: rivedere, non mi piace
            // validazione della condizione di stop del loop
            fineAssegnazione = ruoliGiocatori[0] == 0 && ruoliGiocatori[1] == 0 && ruoliGiocatori[2] == 0 && ruoliGiocatori[3] == 0;
        } while (!fineAssegnazione && ruoliGiocatori[ruoloGenerato] <= 0); // continua a generare finché non trovo un ruolo da assegnare o il loop non è finito
    }
}

/**
 * Subroutine che distribuisce le prime carte del mazzo caricato ai giocatori.
 * Ogni giocatore possiede, inizialmente, tante carte quante sono i suoi punti vita.
 * La subroutine modifica il mazzo fornito scartando dalla cima le carte distribuite ai giocatori,
 * e il campo "carteMano" della struttura di ogni giocatore, allocata dinamicamente per ognuno di essi.
 * Se l'allocazione dinamica del campo fallisce, il programma si arresta.
 *
 * @param mazzo Il mazzo da cui prendere le carte da distribuire
 * @param giocatori I giocatori a cui distribuire le carte
 * @param nGiocatori Il numero di giocatori a cui distribuire le carte
 */
void distribuisciCartePartenza(Mazzo* mazzo, Giocatore* giocatori, int nGiocatori) {
    // iterazione sui giocatori
    for(int i = 0; i < nGiocatori; i++) {
        // allocazione dinamica del campo contenente le carte di ogni giocatore
        giocatori[i].carteMano = (Carta*) calloc(giocatori->puntiVita, sizeof(Carta));
        // TODO: nel caso, raccogliere in un altro file
        // verifica dell'allocazione dinamica
        if(giocatori[i].carteMano == NULL) {
            printf("Errore: impossibile allocare dinamicamente il mazzo di carte del giocatore. Arresto.");
            exit(-1);
        }
        // a ogni giocatore vengono assegnate le prime carte del mazzo, per un numero totale pari ai suoi punti vita
        for(int j = 0; j < giocatori[i].puntiVita; j++) {
            giocatori[i].carteMano[j] = mazzo->carte[j];
        }
        // le carte assegnate al giocatore vengono scartate dalla cima del mazzo
        scartaCimaMazzo(mazzo, giocatori[i].puntiVita);
        // e si diminuisce quindi anche il campo del mazzo che contiene il numero di carte
        mazzo->numeroCarte -= giocatori[i].puntiVita;
    }
}

/**
 * Chiude il gioco salutando il giocatore, se questo ha deciso di interrompere la partita in un prompt.
 */
void chiudiGioco() {
    printf("Grazie per aver giocato, a presto!");
    exit(0);
}