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
    int i = 0, j = 0;
    // salvataggio da restituire
    Salvataggio partita;
    // boolean di appoggio per verificare che un nickname sia valido
    bool verificaNome = true;

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

    // verifica dell'allocazione dinamica
    assertPuntatoreNonNull(giocatori, "\nErrore: allocazione dinamica dell'array dei giocatori fallito, arresto.");

    // richiesta all'utente dei nickname dei giocatori
    printf("Perfetto, dimmi un po' di più sui partecipanti!\n"
           "Cominciamo dai loro nomi: inserisci il nome di ogni giocatore, tenendo a mente che\n"
           "non possono contenere spazi ed hanno una lunghezza massima di %d caratteri.", NOME_UTENTE_LEN);
    for(i = 0; i < nGiocatori; i++) {
        do {
            printf("\nGiocatore %d: ", i+1);
            scanf(" %49s", giocatori[i].nomeUtente);

            // verifico che il nome non sia vuoto
            if(strcmp(giocatori[i].nomeUtente, "") == 0) {
                verificaNome = false;
                printf("\nInserisci un nickname!");
            }

            // verifica che il nome non sia già stato inserito
            for(j = i-1; j <= 0; j--) {
                // il nome è già stato inserito!
                if(strcmp(giocatori[j].nomeUtente, giocatori[i].nomeUtente) == 0) {
                    verificaNome = false;
                    printf("\nQuesto nome è già stato inserito! Riprova.");
                }
            }
        } while (!verificaNome);
    }

    // assegnazione dei ruoli dei giocatori
    printf("Piacere di conoscervi! Adesso genererò randomicamente i ruoli di ognuno di voi.\n");
    assegnaRuoli(giocatori, nGiocatori);

    // caricamento del mazzoPesca dal file "mazzo_bang.txt"
    printf("\nI ruoli sono stati assegnati! Carico il mazzoPesca di carte dal file di testo...");
    Mazzo mazzoPesca = caricaMazzo();

    printf("\nMazzo caricato! Adesso distribuirò le carte dalla cima del mazzoPesca di pesca. Vi ricordo "
           "che ad ogni giocatore spetta un numero di carte pari al numero dei suoi punti vita di partenza");

    // creazione di un mazzo vuoto che contiene le carte scartate
    Mazzo mazzoScarti = {MAZZO_SCARTO, 0, NULL};

    // distribuzione delle carte dal mazzoPesca ai giocatori
    distribuisciCartePartenza(&mazzoPesca, &mazzoScarti, giocatori, nGiocatori);

    // inserimento dei mazzi nelle info della partita
    partita.mazzoPesca = mazzoPesca;
    partita.mazzoScarti = mazzoScarti;

    // inserimento dei giocatori nelle info della partita
    partita.giocatori = giocatori;
    // determinazione del primo giocatore che inizia, che è sempre lo sceriffo
    partita.prossimoGiocatore = 0;
    while(giocatori[partita.prossimoGiocatore].ruoloAssegnato != SCERIFFO && partita.prossimoGiocatore < nGiocatori)
        partita.prossimoGiocatore++;

    // richiesta all'utente di un nome che identifichi il file di salvataggio
    printf("Ci siamo quasi! Come ultima cosa, vorrei che mi dicessi un nome per il file di salvataggio\n"
           "in cui sarà memorizzata questa partita (max. %d caratteri)", SAVEGAME_NAME_LEN);
    printf("?) ");
    scanf("%16s", partita.nomeSalvataggio);

    // scrittura del nuovo salvataggio
    scriviSalvataggio(partita, partita.nomeSalvataggio);

    #ifdef SAVEGAME_LIST_FILE

    #endif

    printf("\nBene, è tutto pronto! Che la partita abbia inizio, e buona fortuna ai partecipanti!");

    return partita;
}

void avviaPartita(Salvataggio partita) {
    // variabili booleane di appoggio per i cicli sottostanti
    bool ripetizioneCiclo, ripetiScelta = true;
    // variabili booleane che mantengono informazioni sulle carte giocate nei turni precedenti
    bool bangGiocato, cartaGiocata = false;

    // variabili di tipo int di appoggio per la selezioni delle carte da giocare e delle opzioni dei prompt durante il turno
    int promptTurnoScelta, posizioneCartaSelezionata;

    // variabili di tipo char di appoggio per la selezione delle opzioni nei prompt durante il turno
    char ruoloGiocatore[NOME_RUOLO_LEN_MAX + 1], tmpChoice;

    // variabile "Carta" contenente la carta da giocare in questo turno
    Carta cartaSelezionata;
    // puntatore all'arma del giocatore corrente
    Carta* armaGiocatore;

    // variabile contenente il ruolo dei vincitori al termine della partita
    Ruoli* ruoloVincitore = NULL;

    // puntatore al giocatore del turno corrente
    Giocatore* giocatore = NULL;

    // allocazione dinamica del puntatore "ruoloVincitore"
    ruoloVincitore = (Ruoli*) malloc(sizeof(Ruoli));
    assertPuntatoreNonNull(ruoloVincitore, "\nErrore: impossibile allocare memoria dinamicamente.");

    // scrittura del file di salvataggio
    scriviSalvataggio(partita, partita.nomeSalvataggio);

    // inizio della logica del turno
    while(!partitaTerminata(partita, ruoloVincitore)) { // verifico che la partita non sia terminata
        // salvo il prossimo giocatore
        giocatore = &partita.giocatori[partita.prossimoGiocatore];

        printf("------ TURNO n° %d ------", partita.prossimoGiocatore + 1);
        
        prendiNomeRuolo(giocatore->ruoloAssegnato, ruoloGiocatore);
        printf("\n%s, tocca a te giocare! Il tuo ruolo è '%s'.", giocatore->nomeUtente, ruoloGiocatore);

        // calcolo della gittata del giocatore
        giocatore->gittata = calcolaGittata(giocatore);

        // all'inizio del turno, il giocatore pesca due carte
        pescaCarte(&partita.mazzoPesca, giocatore, 2);

        printf("\nPer prima cosa, verifico l'effetto delle tue carte in gioco...");
        // se le carte in gioco permettono al giocatore di continuare, allora inizia il turno
        if(verificaCarteInGioco(partita)) {
            printf("\nIniziamo il turno!");
            bangGiocato = false;
            do {
                // il giocatore sceglie cosa fare all'inizio del turno
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

                        ripetizioneCiclo = false;
                        // chiedo al giocatore di scegliere una carta
                        do {
                            // se il ciclo è stato ripetuto, chiedo al giocatore se desidera giocare un'altra carta
                            if(ripetizioneCiclo) {
                                printf("\nDesideri giocare un'altra carta?\n"
                                       "%c/%c) ", PROMPT_CONFERMA, PROMPT_RIFIUTA);
                                tmpChoice = getchar();
                                // torno al menu principale
                                if(tmpChoice == PROMPT_RIFIUTA) {
                                    printf("\nTorno al menu principale!");
                                    break;
                                }
                            }

                            // mostro le carte nella mano del giocatore
                            mostraCarteMazzo(giocatore->carteMano);
                            printf("\nInserisci il numero della carta da selezionare: ");
                            scanf("%d", &posizioneCartaSelezionata);

                            if (posizioneCartaSelezionata <= 0 || posizioneCartaSelezionata > giocatore->carteMano.numeroCarte) {
                                printf("\nInserisci un valore valido!");
                                ripetizioneCiclo = true; // il valore è invalido: ripeto la richiesta
                                continue;
                            } else {
                                cartaSelezionata = giocatore->carteMano.carte[posizioneCartaSelezionata - 1];
                                // 'Bang!' può essere giocata solo una volta (ammenoché non si possegga una 'Volcanic' come arma)
                                if (strcmp(cartaSelezionata.nomeCarta, "Bang!") == 0) {
                                    armaGiocatore = prendiArmaGiocatore(giocatore);
                                    // il giocatore deve selezionare un'altra carta perché non può giocare un altro bang
                                    if (bangGiocato && armaGiocatore != NULL && strcmp(armaGiocatore->nomeCarta, "Volcanic") != 0) {
                                        printf("\nHai già giocato un 'Bang!' in questo turno!");
                                        ripetizioneCiclo = true;
                                        continue;
                                    }
                                }

                                // la funzione 'giocaCarta' restituisce "False" se la carta non è stata giocata per qualsiasi motivo
                                // salvo in una variabile booleana un valore che indica se la carta è stata effettivamente giocata o meno
                                cartaGiocata = giocaCarta(partita.nGiocatori, partita.giocatori,
                                                          partita.prossimoGiocatore, cartaSelezionata,
                                                          &partita.mazzoPesca, &partita.mazzoScarti);

                                if (!cartaGiocata) {
                                    printf("\nScegli un'altra carta!");
                                    ripetizioneCiclo = true;
                                } else {
                                    // se la carta giocata è un 'Bang!', allora impedisco al giocatore di giocarla ancora
                                    if (strcmp(cartaSelezionata.nomeCarta, "Bang!") == 0)
                                        bangGiocato = true;
                                    // rimuovo la carta dalla mano del giocatore
                                    aggiungiCartaMazzo(&partita.mazzoScarti, cartaSelezionata);
                                    rimuoviCartaMazzo(&giocatore->carteMano, cartaSelezionata);
                                }
                            }
                        } while (ripetizioneCiclo);
                        break;
                    // seconda scelta: mostro le carte nella mano del giocatore
                    case PROMPT_TURNO_VEDI_CARTE_MANO:
                        mostraCarteMazzo(partita.giocatori[partita.prossimoGiocatore].carteMano);
                        break;
                    // terza scelta: mostro le carte in gioco del giocatore
                    case PROMPT_TURNO_VEDI_CARTE_GIOCO:
                        mostraCarteMazzo(partita.giocatori[partita.prossimoGiocatore].carteGioco);
                        break;
                    // quarta scelta: mostro la distanza tra il giocatore e gli altri
                    case PROMPT_TURNO_VEDI_DISTANZE:
                        mostraDistanze(partita.nGiocatori, partita.giocatori, partita.prossimoGiocatore);
                        break;
                    // quinta scelta: mostro le carte in gioco degli altri giocatori
                    case PROMPT_TURNO_VEDI_CARTE_GIOCO_ALTRI:
                        mostraCarteInGiocoAltri(partita.nGiocatori, partita.giocatori,partita.prossimoGiocatore);
                        break;
                    // sesta scelta: passo il turno
                    case PROMPT_TURNO_PASSA_TURNO:
                        printf("\nHai deciso di passare il turno! Sei sicuro?\n"
                               "%c/%c) ", PROMPT_CONFERMA, PROMPT_RIFIUTA);
                        tmpChoice = getchar();
                        if (tmpChoice == PROMPT_CONFERMA) {
                            printf("\nBene, passiamo il turno al prossimo giocatore!");
                            ripetiScelta = false;
                        }
                        break;
                    // ultima scelta: chiudo il gioco
                    case PROMPT_TURNO_ESCI:
                        printf("\nSei sicuro di voler uscire?\n"
                               "%c/%c) ", PROMPT_CONFERMA, PROMPT_RIFIUTA);
                        tmpChoice = getchar();
                        if (tmpChoice == PROMPT_CONFERMA) {
                            // scrittura del salvataggio
                            scriviSalvataggio(partita, partita.nomeSalvataggio);
                            chiudiGioco();
                        }
                }
            } while (ripetiScelta); // se in una delle sue azioni il giocatore ha scelto "annulla", allora ripeti il ciclo di scelta
        }

        // seleziono il prossimo giocatore in vita per il turno successivo
        do {
            partita.prossimoGiocatore = (partita.prossimoGiocatore + 1) % partita.nGiocatori;
        } while(partita.giocatori[partita.prossimoGiocatore].puntiVita < 0);
    }
    // fine logica del turno

    // fine partita
    scriviSalvataggio(partita, partita.nomeSalvataggio);
    chiudiPartita(*ruoloVincitore);
}

bool verificaCarteInGioco(Salvataggio partita) {
    int i, giocatoreSuccessivo;
    Giocatore* giocatore = &partita.giocatori[giocatoreSuccessivo];

    if(giocatore->carteGioco.numeroCarte > 0) {
        int posizioneCartaDinamite = cercaCartaMazzoPerNome(giocatore->carteGioco, "Dinamite");
        int posizioneCartaPrigione = cercaCartaMazzoPerNome(giocatore->carteGioco, "Prigione");
        if(posizioneCartaDinamite != -1) {
            printf("\nHai in gioco una carta 'Dinamite'! Ora sarà estratta una carta che potrebbe farla esplodere o meno.");
            Carta cartaEstratta = pescaCimaMazzo(&partita.mazzoPesca, NULL, 1)[0];
            if(cartaEstratta.numeroCarta >= 2 && cartaEstratta.numeroCarta <= 9 && cartaEstratta.semeCarta == PICCHE) {
                printf("\nBOOM! La dinamite è esplosa, quindi perdi 3 punti vita!");
                rimuoviPuntiVita(giocatore, NULL, 3, NULL, NULL);
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
        if(posizioneCartaPrigione != -1) {
            printf("\nSei in prigione! Ora sarà estratta una carta: se il suo seme sarà di Cuori, potrai evadere, altrimenti salterai il turno!");
            Carta cartaEstratta = pescaCimaMazzo(&partita.mazzoPesca, NULL, 1)[0];
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
}

/**
 * Funzione che verifica che la partita sia terminata (con la vittoria dei giocatori/fuorilegge/sceriffo)
 * verificandone lo stato, restituendo true o false di conseguenza.
 * Il secondo parametro rappresenta un puntatore a una variabile di tipo "Ruoli", che, nel caso di vittoria,
 * conterrà il ruolo che ha vinto la partita.
 * Se la funzione restituisce "False", allora ruoloVincitore assumerà il valore "-1".
 *
 * @param partita Il file contenente il salvataggio della partita da verificare.
 * @param ruoloVincitore Puntatore alla variabile contenente il ruolo dei vincitori, o "-1" se la partita non è finita.
 * @return True se la partita è terminata, False altrimenti.
 */
bool partitaTerminata(Salvataggio partita, Ruoli* ruoloVincitore) {
    // variabile contatore
    int i;
    // variabili contatori per tutte le tipologie di giocatori ancora in vita
    int nSceriffi = 0, nFuorilegge = 0, nVice = 0, nPlayer = 0, nRinnegati = 0;

    // iterazione sull'array di giocatori
    for(i = 0; i < partita.nGiocatori; i++) {
        if(partita.giocatori[i].puntiVita > 0) {
            // comincio la conta dei giocatori per categoria
            switch (partita.giocatori[i].ruoloAssegnato) {
                case SCERIFFO:
                    nSceriffi++;
                    break;
                case FUORILEGGE:
                    nFuorilegge++;
                    break;
                case VICESCERIFFO:
                    nVice++;
                    break;
                case RINNEGATO:
                    nRinnegati++;
                    break;
                default:
                    nPlayer++;
                    break;
            }
        }
    }

    // se una delle condizioni seguenti è vera, allora la partita è terminata, altrimenti è possibile continuare
    // prima verifica: vittoria degli sceriffi
    if(nSceriffi > 0 && nFuorilegge == 0) {
        *ruoloVincitore = SCERIFFO;
    // seconda verifica: vittoria dei fuorilegge
    } else if(nSceriffi == 0 && nFuorilegge > 0) {
        *ruoloVincitore = FUORILEGGE;
    // terza verifica: vittoria dei rinnegati
    } else if(nSceriffi == 0 && nFuorilegge == 0 && nVice == 0 && nPlayer == 0 && nRinnegati > 0) {
        *ruoloVincitore = RINNEGATO;
    } else {
        *ruoloVincitore = -1; // dato che la partita non è terminata, il ruolo vincitore viene settato all'intero "-1"
    }

    return *ruoloVincitore != -1; // restituisco vero o falso a seconda che sia stato determinato o meno un vincitore
}

// TODO: spostare nel file dei salvataggi
/**
 * Funzione che richiede all'utente le informazioni relative a un salvataggio da caricare,
 * restituendo poi una struttura "Salvataggio" con le informazioni lette dal file "savegame.bin" relativo.
 * Se il salvataggio non viene trovato, all'utente viene chiesto nuovamente di indicare un nome.
 *
 * @return Il salvataggio caricato dal file.
 */
Salvataggio caricaPartita() {
    // variabili di supporto utilizzate nella lettura del file contenente i nomi dei salvataggi
    int read, nSalvataggi = 0, toLoad;
    // nome del salvataggio da caricare
    char nomeSalvataggio[SAVEGAME_NAME_LEN + 1];
    // array di puntatori allocato dinamicamente. se esiste un file con i nomi dei salvataggi, questo li contiene tutti
    char** salvataggi = NULL;
    // puntatore al file che, se esiste, contiene i nomi di tutti i salvataggi disponibili
    FILE* savegamesFile = NULL;

    // verifico l'esistenza del file con la lista dei salvataggi
    #ifdef SAVEGAME_LIST_FILE
        // apertura del file e verifica
        savegamesFile = fopen(strcat(SAVEGAME_DIR, SAVEGAME_LIST_FILE), "r");
        assertPuntatoreNonNull(savegamesFile, "\nImpossibile aprire il file contenente la lista dei salvataggi.");

        // dichiarazione dinamica dell'array di stringhe con i nomi dei salvataggi e verifica
        salvataggi = (char**) calloc(1, sizeof(char*));
        assertPuntatoreNonNull(salvataggi, "\nImpossibile allocare dinamicamente memoria.");

        // lettura del file contenente i salvataggi
        do {
            // allocazione dinamica della stringa con il nome del salvataggio corrente e verifica
            salvataggi[nSalvataggi] = (char*) calloc(SAVEGAME_NAME_LEN, sizeof(char));
            assertPuntatoreNonNull(salvataggi[nSalvataggi], "\nImpossibile allocare dinamicamente memoria.");

            // lettura della riga, che contiene il nome del file
            read = fscanf(savegamesFile, "%s\n", salvataggi[nSalvataggi]);

            // se è stato letto qualcosa, incremento il numero di salvataggi disponibili
            if(read == 1)
                nSalvataggi++;
        } while(read == 1);

        // stampo a schermo i salvataggi disponibili
        printf("\nLista dei salvataggi rinvenuta dal file '%s':", SAVEGAME_LIST_FILE);
        for(int i = 0; i < nSalvataggi; i++) {
            printf("\n%d) %s", i+1, salvataggi[i]);
        }

        // prompt per permettere all'utente di scegliere il salvataggio da caricare
        printf("\nInserisci il numero corrispondente al salvataggio da caricare:\n"
               "?) ");
        do {
            scanf("%d", &toLoad);
            if(toLoad < 1 || toLoad > nSalvataggi)
                printf("\nInserisci un numero tra quelli nella lista:\n"
                       "?)");
        } while(toLoad < 1 || toLoad > nSalvataggi);
        strcpy(nomeSalvataggio, salvataggi[toLoad - 1]);
        free(salvataggi); // libero la memoria non necessaria
    // il file con la lista non è stato definito: il nome deve essere inserito a mano
    #else
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
    int i;
    // intero generato randomicamente, che indica un ruolo (da 0 a 3)
    int ruoloGenerato;
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
            chiudiGioco();
    }

    // stampa a schermo della distribuzione dei ruoli
    printf("Essendo il numero dei partecipanti %d, i ruoli saranno divisi come segue:\n"
           "Sceriffi: %d\n"
           "Rinnegati: %d\n"
           "Fuorilegge: %d\n"
           "Vicesceriffi: %d\n",
           nGiocatori, ruoliGiocatori[SCERIFFO], ruoliGiocatori[RINNEGATO], ruoliGiocatori[FUORILEGGE], ruoliGiocatori[VICESCERIFFO]);

    // iterazione sui giocatori
    for(i = 0; i < nGiocatori; i++) {
        do {
            // dato che i valori dei ruoli vanno sempre da 0 a 3, la generazione deve essere solo per questi tre valori
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
            // TODO: forse può essere più efficiente?
        } while (ruoliGiocatori[ruoloGenerato] <= 0); // continua a generare finché non trovo un ruolo da assegnare
    }
}

/**
 * Subroutine che distribuisce le prime carte del mazzo caricato ai giocatori.
 * Ogni giocatore possiede, inizialmente, tante carte quante sono i suoi punti vita.
 * La subroutine modifica il mazzo fornito scartando dalla cima le carte distribuite ai giocatori,
 * e il campo "carteMano" della struttura di ogni giocatore, allocata dinamicamente per ognuno di essi.
 * Se l'allocazione dinamica del campo fallisce, il programma si arresta.
 *
 * @param mazzoPesca Puntatore al mazzo di pesca da cui prendere le carte da distribuire
 * @param mazzoScarti Puntatore al mazzo delle carte degli scarti.
 * @param giocatori I giocatori a cui distribuire le carte
 * @param nGiocatori Il numero di giocatori a cui distribuire le carte
 */
void distribuisciCartePartenza(Mazzo *mazzoPesca, Mazzo *mazzoScarti, Giocatore *giocatori, int nGiocatori) {
    // iterazione sui giocatori
    for(int i = 0; i < nGiocatori; i++) {
        // allocazione dinamica del campo contenente le carte di ogni giocatore
        giocatori[i].carteMano.carte = (Carta*) calloc(giocatori->puntiVita, sizeof(Carta));

        // verifica dell'allocazione dinamica
        assertPuntatoreNonNull(giocatori[i].carteMano.carte, "\nErrore: impossibile allocare dinamicamente il mazzo di carte del giocatore. Arresto.");

        // a ogni giocatore vengono assegnate le carte dalla cima del mazzo, per un numero totale pari ai suoi punti vita
        giocatori[i].carteMano.carte = pescaCimaMazzo(mazzoPesca, mazzoScarti, giocatori[i].puntiVita);
    }
}

void chiudiPartita(Ruoli ruoloVincitore) {
    // stringa contenente il nome del ruolo vincitore
    char nomeRuolo[NOME_RUOLO_LEN_MAX + 1];

    switch (ruoloVincitore) {
        case SCERIFFO:
            strcpy(nomeRuolo, "Sceriffi");
            break;
        case VICESCERIFFO:
            strcpy(nomeRuolo, "Vicesceriffi");
            break;
        case FUORILEGGE:
            strcpy(nomeRuolo, "Fuorilegge");
            break;
        case RINNEGATO:
            strcpy(nomeRuolo, "Rinnegati");
            break;
    }

    printf("\nCongratulazioni ai vincitori di questa partita: '%s'!", nomeRuolo); // TODO: rivedere, per ora stampa solo le congratulazioni
    chiudiGioco();
}

/**
 * Chiude il gioco salutando il giocatore, se questo ha deciso di interrompere la partita in un prompt.
 */
void chiudiGioco() {
    printf("Grazie per aver giocato, a presto!");
    exit(0);
}