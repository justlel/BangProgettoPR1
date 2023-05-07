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

    Salvataggio partita;
    // da qui, il controllo del flusso di esecuzione è affidato a una delle due funzioni che si occupano di creare/caricare
    // una partita, oppure il programma termina sotto scelta dell'utente.
    switch (choice) {
        case PROMPT_CARICA_PARTITA:
            partita = caricaPartita();
            break;
        case PROMPT_NUOVA_PARTITA:
            partita = creaPartita();
            break;
        case PROMPT_EXIT:
        default:
            chiudiGioco();
            break;
    }

    // avvia la partita scelta
    avviaPartita(partita);
}

/**
 * Funzione che genera una partita a partire dalle informazioni fornite dal giocatore.
 * Le informazioni della partita sono restituite in una struttura "Salvataggio" apposita.
 *
 * @return La partita generata dalle informazioni fornite.
 */
Salvataggio creaPartita() {
    // salvataggio da restituire
    Salvataggio partita;

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
    // TODO: Fare una verifica che impedisca di mettere due nomi uguali
    for(int i = 0; i < nGiocatori; i++) {
        printf("\nGiocatore %d: ", i+1);
        scanf("%49s", giocatori[i].nomeUtente);
    }

    // assegnazione dei ruoli dei giocatori
    printf("Piacere di conoscervi! Adesso genererò randomicamente i ruoli di ognuno di voi.\n");
    assegnaRuoli(giocatori, nGiocatori);

    // caricamento del mazzoPesca dal file "mazzo_bang.txt"
    printf("\nI ruoli sono stati assegnati! Carico il mazzoPesca di carte dal file di testo...");
    Mazzo mazzoPesca = caricaMazzo();

    printf("\nMazzo caricato! Adesso distribuirò le carte dalla cima del mazzoPesca di pesca. Vi ricordo "
           "che ad ogni giocatore spetta un numero di carte pari al numero dei suoi punti vita di partenza");

    // distribuzione delle carte dal mazzoPesca ai giocatori
    distribuisciCartePartenza(&mazzoPesca, giocatori, nGiocatori);

    // inserimento dei giocatori nelle info della partita
    partita.giocatori = giocatori;
    // determinazione del primo giocatore che inizia, che è sempre lo sceriffo
    partita.prossimoGiocatore = 0;
    while(giocatori[partita.prossimoGiocatore].ruoloAssegnato != SCERIFFO && partita.prossimoGiocatore < nGiocatori)
        partita.prossimoGiocatore++;

    // creazione di un mazzo vuoto che contiene le carte scartate
    Mazzo mazzoScarti = {MAZZO_SCARTO, 0, NULL};

    // inserimento dei mazzi nelle info della partita
    partita.mazzoPesca = mazzoPesca;
    partita.mazzoScarti = mazzoScarti;

    // richiesta all'utente di un nome che identifichi il file di salvataggio
    printf("Ci siamo quasi! Come ultima cosa, vorrei che mi dicessi un nome per il file di salvataggio\n"
           "in cui sarà memorizzata questa partita (max. %d caratteri)", SAVEGAME_NAME_LEN);
    printf("?) ");
    scanf("%16s", partita.nomeSalvataggio); // TODO: forse si può fare una funzione per generare un format da un numero

    // scrittura del nuovo salvataggio
    scriviSalvataggio(partita);

    printf("\nBene, è tutto pronto! Che la partita abbia inizio, e buona fortuna ai partecipanti!");

    return partita;
}

void avviaPartita(Salvataggio partita) {
    bool ripetizioneCiclo, ripetiScelta = true, bangGiocato, cartaGiocata = false;
    int promptTurnoScelta, posizioneCartaSelezionata;
    char ruoloGiocatore[NOME_RUOLO_LEN_MAX + 1];
    char tmpChoice;
    Carta cartaSelezionata;

    while(!partitaTerminata(partita)) {
        printf("------ TURNO n° %d ------", partita.prossimoGiocatore + 1);
        
        prendiNomeRuolo(partita.giocatori[partita.prossimoGiocatore].ruoloAssegnato, ruoloGiocatore);
        printf("\n%s, tocca a te giocare! Il tuo ruolo è '%s'", partita.giocatori[partita.prossimoGiocatore].nomeUtente, ruoloGiocatore);

        // TODO: pesca 2 carte

        printf("\nPer prima cosa, verifico l'effetto delle tue carte in gioco.");
        // se il giocatore non è morto per effetto di qualche carta in gioco, allora inizia il turno
        if(verificaCarteInGioco(partita)) {
            do {
                do {
                    printf("\nScegli una delle seguenti azioni:\n"
                           "%d) Gioca una delle tue carte\n"
                           "%d) Vedi le tue carte in gioco\n"
                           "%d) Controlla la tua distanza dagli altri giocatori\n"
                           "%d) Vedi le carte in gioco degli altri giocatori\n"
                           "%d) Passa il turno\n"
                           "%d) Chiudi il gioco\n"
                           "?) ",
                           PROMPT_TURNO_GIOCA_CARTA, PROMPT_TURNO_VEDI_CARTE_GIOCO, PROMPT_TURNO_VEDI_DISTANZE,
                           PROMPT_TURNO_VEDI_CARTE_GIOCO_ALTRI, PROMPT_TURNO_PASSA_TURNO, PROMPT_TURNO_ESCI);
                    scanf("%d", &promptTurnoScelta);
                    ripetizioneCiclo = promptTurnoScelta != PROMPT_TURNO_GIOCA_CARTA &&
                                       promptTurnoScelta != PROMPT_TURNO_VEDI_CARTE_MANO &&
                                       promptTurnoScelta != PROMPT_TURNO_VEDI_CARTE_GIOCO &&
                                       promptTurnoScelta != PROMPT_TURNO_VEDI_DISTANZE &&
                                       promptTurnoScelta != PROMPT_TURNO_VEDI_CARTE_GIOCO_ALTRI &&
                                       promptTurnoScelta != PROMPT_TURNO_PASSA_TURNO &&
                                       promptTurnoScelta != PROMPT_TURNO_ESCI;
                    if (ripetizioneCiclo)
                        printf("\nInserisci una delle scelte mostrate!\n"
                               "?) ");
                } while (ripetizioneCiclo);

                // a seconda di quello che ha scelto il giocatore, effettuo l'azione corrispondente
                switch (promptTurnoScelta) {
                    // il giocatore ha deciso di giocare una carta
                    case PROMPT_TURNO_GIOCA_CARTA:
                        printf("\nBene, queste sono le carte nella tua mano!");
                        // mostro le carte nella mano del giocatore
                        mostraCarteMazzo(partita.giocatori[partita.prossimoGiocatore].carteMano);
                        printf("\nInserisci il numero della carta da selezionare:");

                        // chiedo al giocatore di scegliere una carta
                        do {
                            ripetizioneCiclo = false;
                            printf("\n?) ");
                            scanf("%d", &posizioneCartaSelezionata);
                            if (posizioneCartaSelezionata <= 0 || posizioneCartaSelezionata >
                                                                  partita.giocatori[partita.prossimoGiocatore].carteMano.numeroCarte) {
                                printf("\nInserisci un valore valido!");
                                ripetizioneCiclo = true;
                            } else {
                                cartaSelezionata = partita.giocatori[partita.prossimoGiocatore].carteMano.carte[
                                        posizioneCartaSelezionata - 1];
                                // 'Bang!' può essere giocata solo una volta (ammenoché non si possegga una 'Volcanic' come arma)
                                if (strcmp(cartaSelezionata.nomeCarta, "Bang!") == 0) {
                                    if (bangGiocato && strcmp(prendiArmaGiocatore(
                                            partita.giocatori[partita.prossimoGiocatore]).nomeCarta, "Volcanic") != 0) {
                                        printf("\nHai già giocato un 'Bang!' in questo turno!");
                                        ripetizioneCiclo = true;
                                        // il giocatore deve selezionare un'altra carta perché non può giocare un altro bang
                                        continue;
                                    }
                                }
                                // salvo in una variabile booleana un valore che indica se la carta è stata effettivamente giocata o meno
                                cartaGiocata = giocaCarta(partita.nGiocatori, partita.giocatori,
                                                          partita.prossimoGiocatore, cartaSelezionata,
                                                          &partita.mazzoPesca);
                                if (!cartaGiocata) {
                                    printf("\nScegli un'altra carta!");
                                    ripetizioneCiclo = true;
                                } else {
                                    // se la carta giocata è un 'Bang!', allora impedisco al giocatore di giocarla ancora
                                    if (strcmp(cartaSelezionata.nomeCarta, "Bang!") == 0)
                                        bangGiocato = true;
                                }
                            }
                        } while (ripetizioneCiclo);
                        break;
                    case PROMPT_TURNO_VEDI_CARTE_MANO:
                        mostraCarteMazzo(partita.giocatori[partita.prossimoGiocatore].carteMano);
                        break;
                    case PROMPT_TURNO_VEDI_CARTE_GIOCO:
                        mostraCarteMazzo(partita.giocatori[partita.prossimoGiocatore].carteGioco);
                        break;
                    case PROMPT_TURNO_VEDI_DISTANZE:
                        mostraDistanze(partita.nGiocatori, partita.giocatori, partita.prossimoGiocatore);
                        break;
                    case PROMPT_TURNO_VEDI_CARTE_GIOCO_ALTRI:
                        mostraCarteInGiocoAltri(partita.nGiocatori, partita.giocatori,
                                                partita.giocatori[partita.prossimoGiocatore]);
                        break;
                    case PROMPT_TURNO_PASSA_TURNO:
                        printf("\nHai deciso di passare il turno! Sei sicuro?\n");
                        printf("\n%c/%c) ", PROMPT_CONFERMA, PROMPT_RIFIUTA);
                        tmpChoice = getchar();
                        ripetiScelta = tmpChoice == PROMPT_RIFIUTA;
                        if (!ripetiScelta) {
                            printf("\nBene, passiamo il turno al prossimo giocatore!");
                        }
                        break;
                    case PROMPT_TURNO_ESCI:
                        printf("\nSei sicuro di voler uscire?\n");
                        printf("\n%c/%c) ", PROMPT_CONFERMA, PROMPT_RIFIUTA);
                        tmpChoice = getchar();
                        if (tmpChoice == PROMPT_CONFERMA)
                            chiudiGioco();
                }
            } while (ripetiScelta); // se in una delle sue azioni il giocatore ha scelto "annulla", allora ripeti il ciclo di scelta
        }
        partita.prossimoGiocatore = (partita.prossimoGiocatore + 1) % partita.nGiocatori;
    }
}

bool verificaCarteInGioco(Salvataggio partita) {
    int i, giocatoreSuccessivo;
    Giocatore* giocatore = &partita.giocatori[giocatoreSuccessivo];

    if(giocatore->carteGioco.numeroCarte > 0) {
        int posizioneCartaDinamite = cercaCartaMazzoPerNome(giocatore->carteGioco, "Dinamite");
        int posizioneCartaPrigione = cercaCartaMazzoPerNome(giocatore->carteGioco, "Prigione");
        if(posizioneCartaDinamite != -1) {
            printf("\nHai in gioco una carta 'Dinamite'! Ora sarà estratta una carta che potrebbe farla esplodere o meno.");
            Carta cartaEstratta = pescaCimaMazzo(&partita.mazzoPesca, 1)[0];
            if(cartaEstratta.numeroCarta >= 2 && cartaEstratta.numeroCarta <= 9 && cartaEstratta.semeCarta == PICCHE) {
                printf("\nBOOM! La dinamite è esplosa, quindi perdi 3 punti vita!");
                rimuoviPuntiVita(giocatore, 3);
                rimuoviCartaMazzo(&giocatore->carteGioco, cartaEstratta);
            } else {
                // cerco il giocatore in vita immediatamente successivo al giocatore corrente che non abbia già una carta dinamite
                for(i = giocatoreSuccessivo; i < partita.nGiocatori; i = (i + 1) % partita.nGiocatori) {
                    if(partita.giocatori[giocatoreSuccessivo].puntiVita > 0 && !possiedeCartaInGioco(partita.giocatori[i], "Dinamite"))
                        break;
                }
                printf("\nScampato pericolo! La dinamite viene passata al prossimo giocatore possibile: %s.", partita.giocatori[i].nomeUtente);
                aggiungiCartaMazzo(&partita.giocatori[i].carteGioco, giocatore->carteGioco.carte[posizioneCartaDinamite]);
                rimuoviCartaMazzo(&giocatore->carteGioco, giocatore->carteGioco.carte[posizioneCartaDinamite]);
            }
        }
        if(giocatore->puntiVita == 0)
            return false;
        if(posizioneCartaPrigione != -1) {
            printf("\nSei in prigione! Ora sarà estratta una carta: se il suo seme sarà di Cuori, potrai evadere, altrimenti salterai il turno!");
            Carta cartaEstratta = pescaCimaMazzo(&partita.mazzoPesca, 1)[0];
            if(cartaEstratta.semeCarta == CUORI) {
                printf("\nLa carta estratta è di cuori, puoi evadere di prigione!");
            } else {
                printf("\nOps, la carta estratta non è di cuori, quindi salterai il turno!");
                return false;
            }
            rimuoviCartaMazzo(&giocatore->carteGioco, giocatore->carteGioco.carte[i]);
        }
    } else {
        printf("\nNon hai nessuna carta in gioco!");
    }
    return true;
}

bool partitaTerminata(Salvataggio partita) {

}

/**
 * Funzione che richiede all'utente le informazioni relative a un salvataggio da caricare,
 * restituendo poi una struttura "Salvataggio" con le informazioni lette dal file "savegame.bin" relativo.
 * Se il salvataggio non viene trovato, all'utente viene chiesto nuovamente di indicare un nome.
 *
 * @return Il salvataggio caricato dal file.
 */
Salvataggio caricaPartita() {
    // nome del salvataggio da caricare
    char nomeSalvataggio[SAVEGAME_NAME_LEN + 1];

    // verifico l'esistenza del file con la lista dei salvataggi
    #ifdef SAVEGAME_LIST_FILE
        FILE* savegamesFile = NULL;
        savegamesFile = fopen(strcat(SAVEGAME_DIR, SAVEGAME_LIST_FILE), "r");
        // TODO: nel caso spostare queste funzioni in un file 'utils'
        if(savegamesFile == NULL) {
            printf("\nImpossibile aprire il file desiderato.");
            exit(-1);
        }

        char** salvataggi = NULL;
        salvataggi = (char**) calloc(1, sizeof(char*));
        if(salvataggi == NULL) {
            printf("\nImpossibile allocare dinamicamente memoria."); // TODO: nel caso spostare queste funzioni in un file 'utils'
            exit(-1);
        }
        // lettura del file contenente i salvataggi
        int read, nSalvataggi = 0;
        do {
            salvataggi[nSalvataggi] = (char*) calloc(SAVEGAME_NAME_LEN, sizeof(char));
            if(salvataggi[nSalvataggi] == NULL) {
                printf("\nImpossibile allocare dinamicamente memoria."); // TODO: nel caso spostare queste funzioni in un file 'utils'
                exit(-1);
            }
            read = fscanf(savegamesFile, "%s\n", salvataggi[nSalvataggi]);
            nSalvataggi++;
        } while(read == 1);

        printf("\nLista dei salvataggi rinvenuta dal file '%s':", SAVEGAME_LIST_FILE);
        for(int i = 0; i < nSalvataggi; i++) {
            printf("\n%d) %s", i+1, salvataggi[i]);
        }

        // prompt per permettere all'utente di scegliere il salvataggio da caricare
        int toLoad;
        printf("\nInserisci il numero corrispondente al salvataggio da caricare:\n"
               "?) ");
        do {
            scanf("%d", &toLoad);
            if(toLoad < 1 || toLoad > nSalvataggi)
                printf("\nInserisci un numero tra quelli nella lista:\n"
                       "?)");
        } while(toLoad < 1 || toLoad > nSalvataggi);
        strcpy(nomeSalvataggio, salvataggi[toLoad]);
        free(salvataggi); // libero la memoria non necessaria
    #else // il file con la lista non è stato definito: il nome deve essere inserito a mano
        printf("\nSe vuoi caricare un salvataggio, inserisci il nome del file in cui è stato scritto\n"
               "?) ");
        do {
            scanf("%16s", nomeSalvataggio);
            if(strcmp(nomeSalvataggio, "") == 0)
                printf("\nInserisci un nome valido\n"
                       "?) ");
        } while(strcmp(nomeSalvataggio, "") == 0);
    #endif
    // conferma del salvataggio scelto
    printf("\nHai selezionato il salvataggio '%s'. Desideri caricarlo?\n"
           "%c/%c", nomeSalvataggio, PROMPT_CONFERMA, PROMPT_RIFIUTA);
    char conferma;

    do {
        conferma = getchar();
        if(conferma != PROMPT_CONFERMA && conferma != PROMPT_RIFIUTA)
            printf("\nInserisci una delle due opzioni\n"
                   "%c/%c) ", PROMPT_CONFERMA, PROMPT_RIFIUTA);
    } while(conferma != PROMPT_CONFERMA && conferma != PROMPT_RIFIUTA);

    // se l'utente ha confermato, carico il salvataggio, altrimenti chiudo il gioco
    if(conferma == PROMPT_CONFERMA) {
        printf("\nCaricamento del salvataggio '%s'...", nomeSalvataggio);
        caricaSalvataggio(nomeSalvataggio);
    } else {
        printf("\nUscita dal gioco.");
        chiudiGioco();
    }
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
        giocatori[i].carteMano.carte = (Carta*) calloc(giocatori->puntiVita, sizeof(Carta));
        // TODO: nel caso, raccogliere in un altro file
        // verifica dell'allocazione dinamica
        if(giocatori[i].carteMano.carte == NULL) {
            printf("Errore: impossibile allocare dinamicamente il mazzo di carte del giocatore. Arresto.");
            exit(-1);
        }
        // a ogni giocatore vengono assegnate le carte dalla cima del mazzo, per un numero totale pari ai suoi punti vita
        giocatori[i].carteMano.carte = pescaCimaMazzo(mazzo, giocatori[i].puntiVita); // TODO: da testare
    }
}

/**
 * Chiude il gioco salutando il giocatore, se questo ha deciso di interrompere la partita in un prompt.
 */
void chiudiGioco() {
    printf("Grazie per aver giocato, a presto!");
    exit(0);
}