//
// Created by just on 24/03/23.
//
#include "giocatore.h"

/**
 * Funzione che associa ad ogni elemento dell'enumerazione 'Ruoli' un nome,
 * che viene inserito nel puntatore alla stringa fornito.
 *
 * @param ruoli Il ruolo di cui si vuole ottenere il nome
 * @param nomeRuolo La stringa in cui inserire il nome del ruolo
 */
void prendiNomeRuolo(Ruoli ruoli, char nomeRuolo[NOME_RUOLO_LEN_MAX + 1]) {
    switch (ruoli) {
        case SCERIFFO:
            strcpy(nomeRuolo, "Sceriffo");
            break;
        case VICESCERIFFO:
            strcpy(nomeRuolo, "Vicesceriffo");
            break;
        case FUORILEGGE:
            strcpy(nomeRuolo, "Fuorilegge");
            break;
        case RINNEGATO:
            strcpy(nomeRuolo, "Rinnegato");
            break;
    }
}

/**
 * Funzione che calcola le distanze degli altri utenti da un giocatore scelto.
 * Il valore di ritorno è un puntatore ad un array, dove ogni elemento corrisponde
 * alla distanza dal giocatore dello stesso indice, compreso il giocatore stesso (che ha distanza 0).
 * Per esempio, l'elemento '1' dell'array conterrà la distanza del giocatore scelto dal giocatore '1'.
 *
 * @param nGiocatori Il numero di utenti in gioco.
 * @param giocatori Un array contenente gli utenti in gioco.
 * @param posizioneGiocatore La posizione del giocatore da cui calcolare le altre distanze.
 * @return Un puntatore ad un array di dimensione 'nGiocatori', contente le distanze dagli altri giocatori.
 */
int* calcolaDistanzeGiocatori(int nGiocatori, Giocatore giocatori[nGiocatori], int posizioneGiocatore) {
    int i;
    // array delle distanze
    int* distanze = NULL;

    distanze = (int*) calloc(nGiocatori, sizeof(int));
    assertPuntatoreNonNull(distanze, "\nImpossibile allocare dinamicamente memoria.");

    // la distanza dal giocatore a sé stesso è convenzionalmente 0
    distanze[posizioneGiocatore] = 0;

    // calcolo la distanza da ogni giocatore
    for(i = 0; i < nGiocatori; i++)
        distanze[i] = calcolaDistanzaGiocatore(giocatori, posizioneGiocatore, i);

    // restituisco l'array con le distanze
    return distanze;
}

/**
 * Funzione che calcola la distanza tra due giocatori.
 * Si considera come "giocatore1" quello di cui si verifica l'effetto della carta "mirino",
 * mentre come "giocatore2" quello di cui si verifica la carta "mustang".
 * Il calcolo della distanza è effettuato rimuovendo dalla conta i giocatori che sono morti durante la partita.
 *
 * @param giocatori Array di giocatori.
 * @param posizioneGiocatore1 Posizione del primo giocatore.
 * @param posizioneGiocatore2 Posizione del secondo giocatore.
 * @return La distanza tra giocatore1 e giocatore2.
 */
int calcolaDistanzaGiocatore(Giocatore giocatori[], int posizioneGiocatore1, int posizioneGiocatore2) {
    int distanza;

    // calcolo la distanza senza considerare altri fattori
    if(posizioneGiocatore2 > posizioneGiocatore1) {
        // primo caso, scorro l'array di giocatori da sinistra a destra, partendo dal primo
        distanza = posizioneGiocatore2 - posizioneGiocatore1;
        // conto il numero di giocatori morti, e per ogni giocatore trovato tra i due diminuisco di uno la posizione
        for(int i = posizioneGiocatore1 + 1; i < posizioneGiocatore2; i++) {
            if(giocatori[i].puntiVita < 1)
                distanza--;
        }
    } else {
        // secondo caso, scorro l'array di giocatori da sinistra a destra, partendo dal secondo
        distanza = posizioneGiocatore1 - posizioneGiocatore2;
        // conto il numero di giocatori morti, e per ogni giocatore trovato tra i due diminuisco di uno la posizione
        for(int i = posizioneGiocatore2 + 1; i < posizioneGiocatore1; i++) {
            if(giocatori[i].puntiVita < 1)
                distanza--;
        }
    }

    // verifico che il giocatore1 possegga una carta mirino
    if(possiedeCartaInGioco(giocatori[posizioneGiocatore1], "Mirino") && distanza > 1)
        distanza--;
    // verifico che il giocatore2 non abbia un mustang
    if(possiedeCartaInGioco(giocatori[posizioneGiocatore2], "Mustang"))
        distanza++;

    // restituisco la distanza calcolata
    return distanza;
}

/**
 * Subroutine mostra all'utente la sua distanza dagli altri giocatori.
 *
 * @param nGiocatori Il numero di giocatori in partita.
 * @param posizioneGiocatore L'indice del giocatore da cui calcolare le distanze.
 * @param giocatori I giocatori in partita.
 */
void mostraDistanze(int nGiocatori, Giocatore giocatori[], int posizioneGiocatore) {
    // array contenente le distanze per da ogni giocatore
    int *distanze;

    distanze = calcolaDistanzeGiocatori(nGiocatori, giocatori, posizioneGiocatore);

    printf("\n-------------- DISTANZE --------------");

    // itero sulle distanze calcolate
    for(int i = 0; i < nGiocatori; i++) {
        if(i != posizioneGiocatore && giocatori[i].puntiVita > 0) { // non è necessario mostrare la posizione del giocatore stesso e dei morti
            // è necessario evidenziare lo sceriffo tra gli altri giocatori
            if(giocatori[i].ruoloAssegnato == SCERIFFO) {
                printf("\n*) Lo SCERIFFO, %s, dista %d posizioni.", giocatori[i].nomeUtente, distanze[i]);
            } else {
                printf("\n*) Il giocatore %s dista %d posizioni.", giocatori[i].nomeUtente, distanze[i]);
            }
        }
    }

    printf("\n--------------------------------------\n");
}

/**
 * Subroutine che mostra le carte in gioco di tutti i giocatori, al di fuori di quello fornito.
 *
 * @param nGiocatori Il numero totale di giocatori.
 * @param giocatori Gli utenti in gioco.
 * @param posizioneGiocatore L'indice del posizioneGiocatore di cui NON si vogliono mostrare le carte.
 */
void mostraCarteInGiocoAltri(int nGiocatori, Giocatore giocatori[nGiocatori], int posizioneGiocatore) {
    int i;

    for(i = 0; i < nGiocatori; i++) {
        if(strcmp(giocatori[i].nomeUtente, giocatori[posizioneGiocatore].nomeUtente) != 0 && giocatori[i].puntiVita > 0) {
            printf("\n*) Carte in gioco di %s", giocatori[i].nomeUtente);
            mostraCarteMazzo(giocatori[i].carteGioco);
            printf("\n");
        }
    }
}

// TODO: Aggiungere documentazione
bool giocaCarta(Mazzo *mazzoPesca, Mazzo *mazzoScarti, int nGiocatori, Giocatore giocatori[nGiocatori], int posizioneGiocatore, int posizioneCarta) {
    // puntatore al giocatore che sta giocando la carta al momento
    Giocatore* giocatore = &giocatori[posizioneGiocatore];
    // carta da giocare
    Carta carta = giocatori[posizioneGiocatore].carteMano.carte[posizioneCarta];
    // variabili di appoggio utilizzate nei cicli do-while interni per chiedere conferma delle scelte
    bool confermaCarta;
    char confermaAzione;

    // PRIMA PARTE: carte istantanee
    if (carta.tipologiaCarta == ISTANTANEA) {
        if (strcmp(carta.nomeCarta, "Bang!") == 0) {
            int i;
            // e il puntatore al giocatore bersagliato
            Giocatore* giocatoreScelto = NULL;
            // arma in uso dal giocatore, ottenuta dalla funzione prendiArmaGiocatore

            printf("\nLa carta 'Bang!' ti permette di infliggere danni a un giocatore.");

            printf("\nLa tua gittata attuale è: %d", giocatore->gittata);

            // chiedo all'utente a quale giocatore vorrebbe sparare, finché non conferma la sua scelta
            do {
                giocatoreScelto = selezionaGiocatore(nGiocatori, giocatori, posizioneGiocatore, true, true);

                // chiedo all'utente di confermare la sua scelta
                printf("\nStai per sparare a %s! Confermi la tua scelta?\n"
                       "%c/%c) ", giocatoreScelto->nomeUtente, PROMPT_CONFERMA, PROMPT_RIFIUTA);
                scanf(" %c", &confermaAzione);
                confermaCarta = confermaAzione != PROMPT_RIFIUTA;
                if (!confermaCarta) {
                    printf("\nDesideri cambiare giocatore? Inserendo no, tornerai al menu principale.\n"
                           "%c/%c) ", PROMPT_CONFERMA, PROMPT_RIFIUTA);
                    scanf(" %c", &confermaAzione);
                    if (confermaAzione == PROMPT_RIFIUTA) {
                        printf("\nTorno al menu principale!");
                        return false; // la carta 'bang!' non è stata giocata
                    }
                }
            } while (!confermaCarta);

            // verifica di eventuali azioni per evitare lo sparo
            // primo elemento: carta barile
            if (possiedeCartaInGioco(*giocatoreScelto, "Barile")) {
                printf("\nIl giocatore %s possiede una carta 'Barile'. Sarà estratta una carta che, se sarà"
                       "del seme di Cuori, annullerà completamente l'attacco.", giocatoreScelto->nomeUtente);
                // estraggo una carta (che sarà spostata nel mazzo scarti) attraverso la funzione 'estraiCarta'.
                if (estraiCarta(mazzoPesca, mazzoScarti).tipologiaCarta == CUORI) {
                    printf("\nLa carta estratta è di Cuori! L'attacco è mancato!");
                    return true;
                } else {
                    printf("\nLa carta estratta non è una carta di Cuori!");
                }
            }

            // secondo elemento: carta mancato
            int cartaMancato = cercaCartaMazzoPerNome(giocatoreScelto->carteMano, "Mancato");
            // il giocatore possiede un 'Mancato!'
            if (cartaMancato != -1) {
                printf("\nIl giocatore %s possiede una carta 'Mancato'! Se deciderà di giocarla, potrà evitare l'attacco.", giocatoreScelto->nomeUtente);
                svuotaSchermo();
                printf("\nPassa lo schermo a %s per permettergli di scegliere se giocare la carta.\n"
                       "Premi 'Invio' per continuare.", giocatoreScelto->nomeUtente);
                while (getchar() != '\n') // aspetto che il giocatore sia sullo schermo
                    continue;
                getchar();
                printf("\n%s ha giocato una carta 'Bang!' contro di te: desideri scartare una carta 'Mancato!' ed evitare l'attacco?\n"
                       "%c/%c) ", giocatore->nomeUtente, PROMPT_CONFERMA, PROMPT_RIFIUTA);
                scanf(" %c", &confermaAzione);

                // passo di nuovo lo schermo
                svuotaSchermo();
                printf("\nBene! Ora passa lo schermo di nuovo all'altro giocatore.");
                printf("\nPremi invio per continuare.");
                while (getchar() != '\n')
                    continue;
                getchar();

                // verifico la scelta del giocatore attaccato

                if (confermaAzione != PROMPT_RIFIUTA) {
                    printf("\n%s ha giocato un 'Mancato' ed evita l'attacco!", giocatoreScelto->nomeUtente);
                    // scarto la carta 'Mancato!' appena giocata
                    aggiungiCartaMazzo(mazzoScarti, giocatore->carteMano.carte[cartaMancato]);
                    rimuoviCartaMazzo(&giocatoreScelto->carteMano, cartaMancato);
                    return true; // la carta 'Bang!' è stata comunque giocata, quindi termino la funzione
                } else {
                    printf("\n%s ha deciso di non giocare un 'Mancato'!", giocatoreScelto->nomeUtente);
                }
            }
            // rimozione del punto vita e fine della funzione
            printf("\n%s ha sparato a %s, che perde quindi un punto vita!", giocatore->nomeUtente, giocatoreScelto->nomeUtente);
            rimuoviPuntiVita(giocatoreScelto, giocatore, 1, mazzoPesca, mazzoScarti);
        } else if (strcmp(carta.nomeCarta, "Birra") == 0) {
            // verifico che il giocatore non abbia già il massimo della vita
            if (giocatore->puntiVita == prendiVitaMassima(*giocatore)) {
                printf("\nHai già il massimo della vita!");
                return false; // la carta non è stata giocata
            }
            printf("\n%s ha utilizzato una birra e guadagna un punto vita!", giocatore->nomeUtente);
            giocatori[posizioneGiocatore].puntiVita++;
        } else if (strcmp(carta.nomeCarta, "Diligenza") == 0) {
            printf("\n%s pesca 2 carte!", giocatore->nomeUtente);
            pescaCarte(mazzoPesca, mazzoScarti, &giocatori[posizioneGiocatore], 2);
        } else if (strcmp(carta.nomeCarta, "Panico!") == 0) {
            int i;
            // posizione della carta del mazzo che si vuole rubare
            int posizioneCartaDaRubare = 0;
            // posizione del giocatore scelto
            int posizioneGiocatoreScelto = 0;
            // puntatore al giocatore scelto
            Giocatore* giocatoreScelto = NULL;
            // puntatore alla carta che si intende rubare
            Carta* cartaDaRubare = NULL;


            printf("\nLa carta 'Panico!' ti permette di rubare una carta a un giocatore di distanza 1.");
            // chiedo all'utente il nome del giocatore che intende bersagliare
            do {
                do {
                    // chiedo al giocatore di sceglierne un altro, mostrando le distanze
                    giocatoreScelto = selezionaGiocatore(nGiocatori, giocatori, posizioneGiocatore, true, false);

                    // valido la distanza
                    // calcolo della posizione del giocatore
                    for(i = 0; i < nGiocatori; i++) {
                        if (strcmp(giocatoreScelto->nomeUtente, giocatori[i].nomeUtente) == 0) {
                            posizioneGiocatoreScelto = i;
                            break;
                        }
                    }
                    // verifico che la distanza sia corretta
                    if(calcolaDistanzaGiocatore(giocatori, posizioneGiocatore, posizioneGiocatoreScelto) > 1) {
                        printf("\nQuesto giocatore è troppo lontano!");
                        giocatoreScelto = NULL;
                    }

                    // verifico che l'utente scelto abbia effettivamente delle carte in mano
                    if(giocatore->carteMano.numeroCarte < 1) {
                        printf("\nQuesto giocatore non ha nessuna carta in mano!");
                        giocatoreScelto = NULL;
                    }

                    // potrebbe essere che l'unico giocatore disponibile non abbia carte in mano, quindi chiedo all'utente
                    // se preferisce sceglierne un altro o tornare al menu prinicpale
                    if(giocatoreScelto == NULL) {
                        printf("\nIl giocatore scelto non è valido. Vuoi sceglierne un altro?\n"
                               "%c/%c) ", PROMPT_CONFERMA, PROMPT_RIFIUTA);
                        scanf(" %c", &confermaAzione);
                        if(confermaAzione == PROMPT_RIFIUTA) {
                            printf("\nTorno al menu principale!");
                            return false;
                        }
                    }
                } while(giocatoreScelto == NULL);

                // e chiedo all'utente di confermare la scelta
                printf("\nHai scelto di rubare una carta a %s. Confermi?\n"
                       "%c/%c) ", giocatoreScelto->nomeUtente, PROMPT_CONFERMA, PROMPT_RIFIUTA);
                scanf(" %c", &confermaAzione);
                if(confermaAzione == PROMPT_RIFIUTA) {
                    giocatoreScelto = NULL; // annullo la scelta del giocatore
                    printf("\nScelta annullata! Desideri tornare al menu principale?\n"
                           "%c/%c) ", PROMPT_CONFERMA, PROMPT_RIFIUTA);
                    scanf(" %c", &confermaAzione);
                    if(confermaAzione == PROMPT_CONFERMA) {
                        printf("\nTorno al menu principale!");
                        return false;
                    } else {
                        giocatoreScelto = NULL;
                        continue;
                    }
                }

                do {
                    // chiedo all'utente la posizione della carta da rubare
                    do {
                        // mostro le carte disponibili
                        printf("\nQueste sono le carte di %s:", giocatoreScelto->nomeUtente);
                        mostraCarteMazzo(giocatoreScelto->carteMano);

                        printf("\nInserisci la posizione della carta da rubare!\n"
                               "?) ");
                        scanf("%d", &posizioneCartaDaRubare);

                        // verifico che la posizione sia valida
                        if(posizioneCartaDaRubare <= 0 || posizioneCartaDaRubare > giocatoreScelto->carteMano.numeroCarte)
                            printf("\nInserisci una posizione valida!");
                    } while (posizioneCartaDaRubare <= 0 || posizioneCartaDaRubare > giocatoreScelto->carteMano.numeroCarte);
                    // salvo da parte la carta indicata dal giocatore
                    cartaDaRubare = &giocatoreScelto->carteMano.carte[posizioneCartaDaRubare - 1];


                    // mostro al giocatore la carta da rubare
                    printf("\nHai deciso di rubare questa carta!");
                    printf("\n%s CARTA %s\n", MEZZO_SEPARATORE, MEZZO_SEPARATORE);
                    mostraCarta(*cartaDaRubare);
                    printf("\n%s CARTA %s", MEZZO_SEPARATORE, MEZZO_SEPARATORE);

                    // chiedo al giocatore di confermare la scelta della carta
                    printf("\nConfermi la tua scelta?\n"
                           "%c/%c) ", PROMPT_CONFERMA, PROMPT_RIFIUTA);
                    scanf(" %c", &confermaAzione);
                    if(confermaAzione == PROMPT_RIFIUTA) {
                        // controllo che il giocatore voglia cambiare carta, cambiare giocatore, o altrimenti torno al menu principale
                        printf("\nDesideri cambiare carta?\n"
                               "%c/%c) ", PROMPT_CONFERMA, PROMPT_RIFIUTA);
                        scanf(" %c", &confermaAzione);
                        if(confermaAzione == PROMPT_RIFIUTA) {
                            printf("\nDesideri cambiare giocatore?\n"
                                   "%c/%c) ", PROMPT_CONFERMA, PROMPT_RIFIUTA);
                            scanf(" %c", &confermaAzione);
                            if(confermaAzione == PROMPT_RIFIUTA) {
                                printf("\nTorno al menu principale!");
                                return false;
                            } else {
                                printf("\nBene, scegli un altro giocatore!");
                                giocatoreScelto = NULL; // resetto il giocatore scelto, così il ciclo si ripete
                            }
                        } else {
                            cartaDaRubare = NULL; // resetto la carta scelta, così il ciclo si ripete
                        }
                    }
                } while (cartaDaRubare == NULL);
            } while(giocatoreScelto == NULL);

            // la scelta è stata confermata, quindi rimuovo la carta dal mazzo da rubare e la aggiungo a quello del giocatore
            aggiungiCartaMazzo(&giocatore->carteMano, *cartaDaRubare);
            rimuoviCartaMazzo(&giocatoreScelto->carteMano, posizioneCartaDaRubare);
            printf("\n%s ruba una carta '%s' a %s!", giocatore->nomeUtente, cartaDaRubare->nomeCarta, giocatoreScelto->nomeUtente);
        } else if (strcmp(carta.nomeCarta, "CatBalou") == 0) {
            printf("\n%s DESCRIZIONE %s", MEZZO_SEPARATORE, MEZZO_SEPARATORE);
            printf("\nLa carta 'CatBalou' ti permette di far scartare una carta\n"
                   "a un giocatore a scelta, a prescindere dalla distanza.");
            printf("\n%s DESCRIZIONE %s\n", MEZZO_SEPARATORE, MEZZO_SEPARATORE);

            // puntatore al giocatore a cui far scartare una carta
            Giocatore* giocatoreScelto;

            do {
                // mostro all'utente la lista dei giocatori in vita, escluso sé stesso
                giocatoreScelto = selezionaGiocatore(nGiocatori, giocatori, posizioneGiocatore, false, false);

                printf("\nConfermi la tua di far scartare una carta a '%s'?\n"
                       "%c/%c) ", giocatoreScelto->nomeUtente, PROMPT_CONFERMA, PROMPT_RIFIUTA);
                scanf(" %c", &confermaAzione);
                if (confermaAzione == PROMPT_RIFIUTA) {
                    printf("\nDesideri scegliere un altro giocatore? Inserendo no, tornerai al menu principale\n"
                           "%c/%c) ", PROMPT_CONFERMA, PROMPT_RIFIUTA);
                    scanf(" %c", &confermaAzione);
                    if (confermaAzione == PROMPT_RIFIUTA) {
                        printf("\nTorno al menu principale!");
                        return false;
                    } else {
                        printf("\nScegli un altro giocatore!");
                        giocatoreScelto = NULL; // annullo il giocatore scelto, così il ciclo si ripete
                    }
                }
            } while (giocatoreScelto == NULL);

            // lo schermo deve passare al giocatore che deve scartare
            svuotaSchermo();
            printf("\n%s ha giocato un CatBalou contro %s, che deve scartare una carta!", giocatore->nomeUtente,
                   giocatoreScelto->nomeUtente);
            printf("\nPassa lo schermo a %s per permettergli di scegliere quale carta scartare.\n"
                   "Premi 'Invio' appena sei pronto.", giocatoreScelto->nomeUtente);
            while (getchar() != '\n')
                continue;
            getchar();

            scartaCarta(&giocatoreScelto->carteMano, mazzoScarti);
            svuotaSchermo();
            printf("\n%s ha scartato una carta per effetto di un Catbalou!", giocatoreScelto->nomeUtente);
        } else if (strcmp(carta.nomeCarta, "Mancato") == 0) {
            printf("\nNon puoi giocare questa carta direttamente, ma solo quando stai subendo uno sparo!");
            return false;
        } else if (strcmp(carta.nomeCarta, "Gatling") == 0) {
            printf("\n%s ha giocato un Gatling: tutti i giocatori perdono un punto vita!", giocatore->nomeUtente);
            for (int i = 0; i < nGiocatori; i++) {
                if (i != posizioneGiocatore && giocatori[i].puntiVita > 0) {
                    rimuoviPuntiVita(&giocatori[i], NULL, 1, mazzoPesca, mazzoScarti);
                }
            }
        } else if (strcmp(carta.nomeCarta, "Saloon") == 0) {
            printf("\n%s ha giocato un Saloon: tutti i giocatori guadagnano un punto vita!", giocatore->nomeUtente);
            for (int i = 0; i < nGiocatori; i++) {
                if (i != posizioneGiocatore && giocatori[i].puntiVita > 0) {
                    giocatori[i].puntiVita++;
                }
            }
        } else if (strcmp(carta.nomeCarta, "WellsFargo") == 0) {
            printf("\n%s pesca 3 carte!", giocatore->nomeUtente);
            pescaCarte(mazzoPesca, NULL, &giocatori[posizioneGiocatore], 3);
        }
        // SECONDA PARTE: carte istantanee speciali
    } else if (carta.tipologiaCarta == ISTANTANEA_SPECIAL) {
        if (strcmp(carta.nomeCarta, "Duello") == 0) {
            // variabile di appoggio che contiene la posizione della carta bang da scartare a ogni turno del duello
            // se la carta non esiste, il valore assunto è '-1'
            int posizioneCartaBang;
            // e il puntatore al giocatore
            Giocatore* giocatoreScelto = NULL;
            // puntatori al, rispettivamente, vincitore e perdente del duello
            Giocatore* vincitoreDuello = NULL, *perdenteDuello = NULL;
            // variabile di appoggio che contiene il giocatore che deve giocare il turno corrente del duello
            Giocatore* duellanteTurnoCorrente;

            do {
                // chiedo all'utente di selezionare un giocatore
                giocatoreScelto = selezionaGiocatore(nGiocatori, giocatori, posizioneGiocatore, false, false);

                printf("\nConfermi la tua scelta di sfidare '%s'?\n"
                       "%c/%c) ", giocatoreScelto->nomeUtente, PROMPT_CONFERMA, PROMPT_RIFIUTA);
                scanf(" %c", &confermaAzione);
                if (confermaAzione == PROMPT_RIFIUTA) {
                    giocatoreScelto = NULL; // annullo la scelta del giocatore
                    printf("\nDesideri scegliere un altro giocatore? Inserendo no, tornerai al menu principale\n"
                           "%c/%c) ", PROMPT_CONFERMA, PROMPT_RIFIUTA);
                    scanf(" %c", &confermaAzione);
                    if (confermaAzione == PROMPT_RIFIUTA) {
                        printf("\nTorno al menu principale!");
                        return false;
                    }
                }
            } while (giocatoreScelto == NULL);

            // il giocatore è confermato, e il duello può avere inizio
            printf("\nSi dia inizio al duello tra %s e %s!", giocatore->nomeUtente, giocatoreScelto->nomeUtente);
            printf("\nEstrarrò una carta 'Bang!' una volta a testa per ciascun giocatore. Il primo a finirle perderà un punto vita!");
            printf("\nIl primo ad iniziare sarà il giocatore sfidato!");
            duellanteTurnoCorrente = giocatoreScelto;

            do {
                printf("\nE' il turno di %s!", duellanteTurnoCorrente->nomeUtente);
                // cerco la carta 'Bang!' nel mazzo del duellante
                posizioneCartaBang = cercaCartaMazzoPerNome(duellanteTurnoCorrente->carteMano, "Bang!");

                // in questo turno, il giocatore non possiede una carta 'Bang!', e quindi ha perso.
                if(posizioneCartaBang == -1) {
                    // inserisco il puntatore al giocatore che ha vinto nella variabile relativa
                    if(duellanteTurnoCorrente == giocatoreScelto) {
                        vincitoreDuello = giocatore;
                        perdenteDuello = giocatoreScelto;
                    } else {
                        vincitoreDuello = giocatoreScelto;
                        perdenteDuello = giocatore;
                    }
                // il giocatore possiede una carta 'Bang!', quindi la scarta e si va al turno successivo
                } else {
                    // rimuovo la carta dalla mano e la inserisco nel mazzo scarti
                    aggiungiCartaMazzo(mazzoScarti, duellanteTurnoCorrente->carteMano.carte[posizioneCartaBang]);
                    rimuoviCartaMazzo(&duellanteTurnoCorrente->carteMano, posizioneCartaBang);
                    printf("\n%s scarta una carta 'Bang!' e supera il turno!", duellanteTurnoCorrente->nomeUtente);

                    // cambio duellante per il prossimo turno
                    if(duellanteTurnoCorrente == giocatoreScelto) {
                        duellanteTurnoCorrente = giocatore;
                    } else {
                        duellanteTurnoCorrente = giocatoreScelto;
                    }
                }
            // ripeti il turno finché non è stato determinato un vincitore
            } while (vincitoreDuello == NULL && perdenteDuello == NULL);

            printf("\n%s FINE DUELLO %s", MEZZO_SEPARATORE, MEZZO_SEPARATORE);
            printf("\n%s ha esaurito le carte 'Bang!', quindi %s vince il duello!", perdenteDuello->nomeUtente, vincitoreDuello->nomeUtente);
            printf("\n%s FINE DUELLO %s", MEZZO_SEPARATORE, MEZZO_SEPARATORE);

            // rimuovo i punti vita dello sconfitto
            rimuoviPuntiVita(perdenteDuello, vincitoreDuello, 3, mazzoPesca, mazzoScarti);
        } else if (strcmp(carta.nomeCarta, "Emporio") == 0) {
            printf("\nLa carta 'Emporio' estrae dal mazzo di pesca un numero di carte pari a quello dei giocatori in vita,\n"
                   "per poi permettere ad ogni utente (a partire da chi ha giocato la carta) di pescare una carta da questo mazzo.");

            // variabili di appoggio per i cicli interni
            int i = 0, j = 0;
            // variabili contenenti il numero di carte da rivelare
            int nCarteRivelate = 0;
            // variabile d'appoggio contenente la posizione della carta scelta dall'utente
            int cartaScelta;
            // array contenente le carte rivelate dall'array
            Carta* carteRivelate = NULL;
            // intero contenente il numero rimanente di carte da swappare
            int swapRimanenti = 0;
            // array temporaneo che contiene le carte del mazzo di pesca da swappare con quello di scarti
            Carta* swapTmp = NULL;

            // calcolo dei giocatori in vita
            for (i = 0; i < nGiocatori; i++) {
                if (giocatori[i].puntiVita > 0)
                    nCarteRivelate++;
            }

            // allocazione dinamica dell'array di carte rivelate
            carteRivelate = (Carta*) calloc(nCarteRivelate, sizeof(Carta));
            assertPuntatoreNonNull(carteRivelate, "\nErrore: impossibile allocare dinamicamente l'array di carte da rivelare.");

            printf("\nEssendoci %d giocatori in vita, saranno rivelate %d carte.", nCarteRivelate, nCarteRivelate);

            if(nCarteRivelate > mazzoPesca->numeroCarte) {
                swapRimanenti = nCarteRivelate - mazzoPesca->numeroCarte;
                // estraggo prima le carte rimanenti dal mazzo, poi lo scambio con quello di pesca e prendo le ultime
                for(i = 0; i < nCarteRivelate - mazzoPesca->numeroCarte; i++) {
                    // sposto la carta nell'array di quelle rivelate
                    carteRivelate[i] = mazzoPesca->carte[mazzoPesca->numeroCarte - i - 1];
                    rimuoviCartaMazzo(mazzoPesca, mazzoPesca->numeroCarte - i - 1);
                }
                // swappo i mazzi
                swapTmp = mazzoPesca->carte;
                mazzoPesca->carte = mazzoScarti->carte;
                mazzoScarti->carte = swapTmp;
                mazzoPesca->numeroCarte = mazzoScarti->numeroCarte;
                mazzoScarti->numeroCarte = 0;
                // pesco le carte rimanenti
                for(i = 0; i < swapRimanenti; i++) {
                    // sposto la carta nell'array di quelle rivelate
                    carteRivelate[i] = mazzoPesca->carte[mazzoPesca->numeroCarte - i - 1];
                    rimuoviCartaMazzo(mazzoPesca, mazzoPesca->numeroCarte - i - 1);
                }
            } else {
                // sposto le carte normalmente
                for(i = 0; i < nCarteRivelate; i++) {
                    // sposto la carta nell'array di quelle rivelate
                    carteRivelate[i] = mazzoPesca->carte[mazzoPesca->numeroCarte - i - 1];
                    rimuoviCartaMazzo(mazzoPesca, mazzoPesca->numeroCarte - i - 1);
                }
            }

            // a partire dal giocatore che ha estratto la carta, chiedo di pescarne una da quelle appena ottenute
            j = posizioneGiocatore;
            // il ciclo si ripete finché tutti i giocatori non hanno estratto almeno una carta
            while (nCarteRivelate > 0) {
                printf("\nTocca a %s pescare! Passagli lo schermo, e premi 'Invio' appena sei pronto.", giocatori[j].nomeUtente);
                while (getchar() != '\n')
                    continue;
                getchar(); // aspetto che venga premuto invio
                svuotaSchermo();

                do {
                    // mostro all'utente le carte rivelate
                    for(i = 0; i < nCarteRivelate; i++) {
                        printf("\n%d)\n", i+1);
                        mostraCarta(carteRivelate[i]);
                        printf("\n");
                    }

                    // e chiedo di sceglierne una
                    printf("\nInserisci il numero della carta da pescare: \n"
                           "?) ");
                    scanf("%d", &cartaScelta);

                    // verifico che il valore inserito sia valido
                    if (cartaScelta <= 0 || cartaScelta > nCarteRivelate)
                        printf("\nIl valore inserito non è valido!");
                } while (cartaScelta <= 0 || cartaScelta > nCarteRivelate);

                printf("\nBene! Questa è la carta che hai pescato:\n");
                mostraCarta(carteRivelate[cartaScelta - 1]);
                printf("\n");

                // aggiunta della carta alla mano del giocatore
                aggiungiCartaMazzo(&giocatori[j].carteMano, carteRivelate[cartaScelta - 1]);

                // rimozione della carta dal mazzetto pescato
                // questa azione viene effettuata "shiftando" di una posizione a sinistra tutte le carte a destra di quella scelta
                for (i = cartaScelta; i < nCarteRivelate; i++) {
                    carteRivelate[i - 1] = carteRivelate[i];
                }

                nCarteRivelate--;
                // rialloco l'array al termine dell'operazione
                if(nCarteRivelate > 0) { // primo caso: ci sono altre carte che devono essere pescate
                    carteRivelate = (Carta*) realloc(carteRivelate, nCarteRivelate * sizeof(Carta));
                    assertPuntatoreNonNull(carteRivelate, "\nErrore: impossibile riallocare l'array di carte rivelate.");
                } else if(nCarteRivelate == 0) { // secondo caso: le carte sono terminate, posso liberare l'array
                    free(carteRivelate);
                } else { // terzo caso: numero negativo, errore
                    printf("\nErrore: il numero di carte rivelate è negativo. Arresto.");
                    exit(-1);
                }

                // uso il modulo per tornare all'inizio nel caso in cui dovessi aver raggiunto la fine dell'array giocatori
                do {
                    j = (j + 1) % nGiocatori;
                } while (giocatori[j].puntiVita < 1);
            }

            printf("\nEstrazione terminata!");
        } else if (strcmp(carta.nomeCarta, "Indiani") == 0) {
            int i;
            // variabile d'appoggio che contiene la posizione della carta 'Bang!' di ogni giocatore.
            // vale '-1' se il giocatore non possiede la carta
            int cartaBang;

            printf("\n%s DESCRIZIONE %s", MEZZO_SEPARATORE, MEZZO_SEPARATORE);
            printf("\nAttacco di indiani in arrivo! Ogni giocatore (eccetto chi ha giocato la carta) deve giocare\n"
                   "una carta 'Bang!', oppure perdere un punto vita!");
            printf("\n%s DESCRIZIONE %s\n", MEZZO_SEPARATORE, MEZZO_SEPARATORE);
            for (i = 0; i < nGiocatori; i++) {
                // il giocatore deve essere in vita, e chi ha giocato la carta non viene attaccato
                if (i != posizioneGiocatore && giocatori[i].puntiVita > 0) {
                    cartaBang = cercaCartaMazzoPerNome(giocatori[i].carteMano, "Bang!");
                    if (cartaBang == -1) {
                        printf("\n%s non possiede una carta 'Bang!', e perde quindi un punto vita!", giocatori[i].nomeUtente);
                        rimuoviPuntiVita(&giocatori[i], NULL, 1, mazzoPesca, mazzoScarti);
                    } else {
                        printf("\n%s scarta una carta 'Bang!' ed evita l'attacco degli indiani!", giocatori[i].nomeUtente);

                        // scarto la carta 'Bang!' giocata
                        aggiungiCartaMazzo(mazzoScarti, giocatori[i].carteMano.carte[cartaBang]);
                        rimuoviCartaMazzo(&giocatori[i].carteMano, cartaBang);
                    }
                }
            }
        }
        // TERZA PARTE: armi
    } else if (carta.tipologiaCarta == ARMA) {
        // intero contenente, se esiste, l'arma già in gioco
        int posizioneArmaInUso;
        // puntatore all'arma attualmente in uso da un giocatore
        Carta* armaInUso = prendiArmaGiocatore(giocatore);

        // il giocatore ha già un'arma in gioco, quindi chiedo di conferma
        // TODO: verificare che la carta di default conti effettivamente come carta (e quindi chiedere conferma prima di cambiarla) o no
        if(armaInUso != NULL) {
            printf("Hai già un'arma equipaggiata: %s. Sei sicuro di voler cambiare arma?\n"
                   "%c/%c) ", armaInUso->nomeCarta, PROMPT_CONFERMA, PROMPT_RIFIUTA);
            scanf(" %c", &confermaAzione);
            if(confermaAzione != PROMPT_CONFERMA)
                return false;
            // calcolo la posizione dell'arma in uso
            posizioneArmaInUso = cercaCartaMazzoPerNome(giocatore->carteGioco, armaInUso->nomeCarta);
        }

        if (strcmp(carta.nomeCarta, "Carabina") == 0) {
            printf("\nHai equipaggiato una CARABINA (%d gittata)", GITTATA_CARABINA);
        } else if (strcmp(carta.nomeCarta, "Remington") == 0) {
            printf("\nHai equipaggiato una REMINGTON (%d gittata)", GITTATA_REMINGTON);
        } else if (strcmp(carta.nomeCarta, "Schofield") == 0) {
            printf("\nHai equipaggiato una SCHOFIELD (%d gittata)", GITTATA_SCHOFIELD);
        } else if (strcmp(carta.nomeCarta, "Winchester") == 0) {
            printf("\nHai equipaggiato una WINCHESTER (%d gittata)", GITTATA_WINCHESTER);
        } else if (strcmp(carta.nomeCarta, "Volcanic") == 0) {
            printf("\nHai equipaggiato una VOLCANIC (%d gittata)", GITTATA_VOLCANIC);
        }

        if(armaInUso != NULL) {
            // spostamento della carta dalla mano al mazzo delle carte in gioco
            giocatore->carteGioco.carte[posizioneArmaInUso] = carta;
            rimuoviCartaMazzo(&giocatore->carteMano, posizioneCarta);
        } else {
            // aggiunta della carta dalla mano al mazzo delle carte in gioco
            aggiungiCartaMazzo(&giocatore->carteGioco, carta);
            rimuoviCartaMazzo(&giocatore->carteMano, posizioneCarta);
        }

        // ricalcolo della gittata del giocatore, dato che è stata cambiata arma
        giocatore->gittata = calcolaGittata(giocatore);
        // QUARTA PARTE: carte in gioco
    } else if (carta.tipologiaCarta == EFFETTO) {
        // verifico che la carta scelta non sia già in gioco
        if(possiedeCartaInGioco(*giocatore, carta.nomeCarta)) {
            printf("\nHai già una carta '%s' in gioco!", carta.nomeCarta);
            return false;
        }

        if (strcmp(carta.nomeCarta, "Barile") == 0) {
            printf("\nLa carta 'Barile' ti consente, fintanto che è in gioco, di schivare una carta 'Bang!'.\n"
                   "Quando vieni colpito, puoi scegliere di estrarre una carta: se questa è di Cuori, allora l'attacco è annullato.");
            aggiungiCartaInGioco(giocatore, giocatore, posizioneCarta);
            printf("\nHai giocato una carta 'Barile'!");
        } else if (strcmp(carta.nomeCarta, "Mirino") == 0) {
            printf("\nLa carta 'Mirino' ti consente, fintanto che è in gioco, di vedere tutti gli altri giocatori a una distanza diminuita di uno.");
            aggiungiCartaInGioco(giocatore, giocatore, posizioneCarta);
            printf("\nHai equipaggiato la carta 'Mirino'!");
        } else if (strcmp(carta.nomeCarta, "Mustang") == 0) {
            printf("\nLa carta 'Mustang' ti consente, fintanto che è in gioco, di essere visto a distanza aumentata di uno dagli altri giocatori.");
            aggiungiCartaInGioco(giocatore, giocatore, posizioneCarta);
            printf("\nHai equipaggiato la carta 'Mustang'!");
        } else if (strcmp(carta.nomeCarta, "Dinamite") == 0) {
            printf("\n%s DESCRIZIONE %s", MEZZO_SEPARATORE, MEZZO_SEPARATORE);
            printf("\nLa carta 'Dinamite' resta innocua per un intero giro.\n"
                   "Al tuo prossimo turno, prima di iniziare devi estrarre una carta che,\n "
                   "se sarà di un numero compreso tra 2 e 9 e del seme di Picche,\n"
                   "farà esplodere la dinamite, facendoti perdere 3 punti vita.\n"
                   "Se, invece, la dinamite non esplode, allora sarà passata\n"
                   "al giocatore successivo, che effettuerà le stesse azioni,\n"
                   "finché la dinamite non esplode o viene scartata.");
            printf("\n%s DESCRIZIONE %s\n", MEZZO_SEPARATORE, MEZZO_SEPARATORE);

            aggiungiCartaInGioco(giocatore, giocatore, posizioneCarta);
            printf("\nHai messo un gioco una 'Dinamite'!\n");
        } else if (strcmp(carta.nomeCarta, "Prigione") == 0) {
            // puntatore al giocatore da mettere in prigione
            Giocatore *giocatoreScelto = NULL;

            printf("\n%s DESCRIZIONE %s", MEZZO_SEPARATORE, MEZZO_SEPARATORE);
            printf("\nLa carta 'Prigione' ti permette di mettere in prigione\n"
                   "un giocatore qualsiasi, tranne lo Sceriffo. Un giocatore in prigione,\n"
                   "prima di giocare il suo turno, estrae una carta:\n"
                   "se questa non ha il seme di Cuori, allora\n"
                   "la carta prigione è scartata e il giocatore salta il turno\n"
                   "(pur rimanendo bersaglio degli altri giocatori), altrimenti,\n"
                   "la prigione è scartata e il turno è giocabile normalmente.");
            printf("\n%s DESCRIZIONE %s\n", MEZZO_SEPARATORE, MEZZO_SEPARATORE);

            do {
                // chiedo all'utente di inserire un nome
                do {
                    giocatoreScelto = selezionaGiocatore(nGiocatori, giocatori, posizioneGiocatore, false, false);

                    // verifico che l'utente non sia lo sceriffo
                    if(giocatoreScelto->ruoloAssegnato == SCERIFFO) {
                        printf("\nNon puoi mettere in giocatore lo sceriffo!");
                        giocatoreScelto = NULL;
                    // verifico che l'utente non sia già in prigione
                    } else if(possiedeCartaInGioco(*giocatore, "Prigione")) {
                        printf("\nIl giocatore è già in prigione.");
                        giocatoreScelto = NULL;
                    }
                } while (giocatoreScelto == NULL); // in questo modo il ciclo si ripete finché un giocatore non è trovato

                printf("\nConfermi la tua scelta di mettere in prigione '%s'?\n"
                       "%c/%c) ", giocatoreScelto->nomeUtente, PROMPT_CONFERMA, PROMPT_RIFIUTA);
                scanf(" %c", &confermaAzione);
                if (confermaAzione == PROMPT_RIFIUTA) {
                    printf("\nDesideri scegliere un altro giocatore? Inserendo no, tornerai al menu principale\n"
                           "%c/%c) ", PROMPT_CONFERMA, PROMPT_RIFIUTA);
                    scanf(" %c", &confermaAzione);
                    if (confermaAzione == PROMPT_RIFIUTA) {
                        printf("\nTorno al menu principale!");
                        return false;
                    } else {
                        printf("\nScegli un altro giocatore!");
                        giocatoreScelto = NULL; // annullo il giocatore scelto, così il ciclo si ripete
                    }
                }
            } while (giocatoreScelto == NULL);

            aggiungiCartaInGioco(giocatore, giocatoreScelto, posizioneCarta);
            printf("\n%s ha messo in prigione %s!", giocatore->nomeUtente, giocatoreScelto->nomeUtente);
        }
    }

    return true;
}

/**
 * Subroutine che rimuove una carta dalla mano di un giocatore, per inserirla nel mazzo delle carte in gioco di
 * un altro giocatore.
 *
 * @param giocatoreCarta Puntatore al giocatore dalla cui mano sarà selezionata la carta.
 * @param giocatoreBersaglio Puntatore al giocatore a cui sarà data la carta.
 * @param carta La posizione della carta da giocare.
 */
void aggiungiCartaInGioco(Giocatore* giocatoreCarta, Giocatore* giocatoreBersaglio, int carta) {
    aggiungiCartaMazzo(&giocatoreBersaglio->carteGioco, giocatoreCarta->carteMano.carte[carta]);
    rimuoviCartaMazzo(&giocatoreCarta->carteMano, carta);
}

/**
 * Subroutine che effettua l'azione di pesca dal mazzo di pesca per un giocatore.
 * Se il mazzo di pesca non ha abbastanza carte, la subroutine effettua uno scambio con quello di scarti.
 *
 * @param mazzoPesca Puntatore al mazzo di pesca, da cui pescare le carte.
 * @param mazzoScarti Puntatore al mazzo di scarti, da scambiare eventualmente con quello di pesca.
 * @param giocatore Puntatore al giocatore che deve pescare le carte.
 * @param nCarte Il numero di carte da pescare.
 */
void pescaCarte(Mazzo *mazzoPesca, Mazzo *mazzoScarti, Giocatore *giocatore, int nCarte) {
    int i;
    // nel caso in cui il mazzo di pesca termini, contatore per le carte da estrarre dopo lo swap
    int carteRimanenti;

    // inserisco le carte pescate nella mano del giocatore
    if(nCarte > mazzoPesca->numeroCarte) {
        printf("\nIl mazzo di pesca è terminato! Lo scambio con quello degli scarti.");

        // pesco le ultime carte rimanenti nel mazzo di pesca
        carteRimanenti = nCarte - mazzoPesca->numeroCarte;
        for(i = 0; i < mazzoPesca->numeroCarte; i++) {
            aggiungiCartaMazzo(&giocatore->carteMano, mazzoPesca->carte[mazzoPesca->numeroCarte - i - 1]);
            rimuoviCartaMazzo(mazzoPesca, mazzoPesca->numeroCarte - i - 1);
        }

        // scambio il mazzo di pesca e quello degli scarti
        free(mazzoPesca->carte);
        mazzoPesca->carte = mazzoScarti->carte;
        mazzoPesca->numeroCarte = mazzoScarti->numeroCarte;
        mazzoScarti->carte = NULL;
        mazzoScarti->numeroCarte = 0;
        mischiaMazzo(mazzoPesca);

        // pesco le carte rimanenti
        for(i = 0; i < carteRimanenti; i++) {
            aggiungiCartaMazzo(&giocatore->carteMano, mazzoPesca->carte[mazzoPesca->numeroCarte - i - 1]);
            rimuoviCartaMazzo(mazzoPesca, mazzoPesca->numeroCarte - i - 1);
        }
    } else {
        // pesco le carte normalmente
        for(i = 0; i < nCarte; i++) {
            aggiungiCartaMazzo(&giocatore->carteMano, mazzoPesca->carte[mazzoPesca->numeroCarte - i - 1]);
            rimuoviCartaMazzo(mazzoPesca, mazzoPesca->numeroCarte - i - 1);
        }
    }
}

/**
 * Subroutine che richiede al giocatore una carta da scartare dalla sua mano e la sposta nel mazzo degli scarti.
 *
 * @param mazzoMano Il mazzo della mano del giocatore.
 * @param mazzoScarti Il mazzo contenente le carte scartate.
 */
void scartaCarta(Mazzo *mazzoMano, Mazzo *mazzoScarti) {
    // indice contenente la carta scelta (che è però va da 1 a n)
    int cartaScelta;

    do {
        // mostro al giocatore le carte nella sua mano
        mostraCarteMazzo(*mazzoMano);
        printf("\nScegli una carta da scartare:\n"
               "?) ");
        scanf("%d", &cartaScelta);
        // verifico la correttezza dell'input
        if(cartaScelta <= 0 || cartaScelta > mazzoMano->numeroCarte)
            printf("\nInserisci un numero tra %d e %d!\n", 1, mazzoMano->numeroCarte);
    } while(cartaScelta <= 0 || cartaScelta > mazzoMano->numeroCarte);

    // aggiungo la carta al mazzo degli scarti
    aggiungiCartaMazzo(mazzoScarti, mazzoMano->carte[cartaScelta - 1]);
    // rimuovo la carta dalla mano del giocatore
    rimuoviCartaMazzo(mazzoMano, cartaScelta - 1);
}

/**
 * Funzione che restituisce l'arma attualmente in uso da un giocatore.
 *
 * @param giocatore Il giocatore di cui ottenere l'arma.
 * @return Un puntatore all'arma in uso dal giocatore. Se il giocatore non ha un'arma, restituisce NULL.
 */
Carta* prendiArmaGiocatore(Giocatore *giocatore) {
    int i;

    for(i = 0; i < giocatore->carteGioco.numeroCarte; i++) {
        if(giocatore->carteGioco.carte[i].tipologiaCarta == ARMA)
            return &giocatore->carteGioco.carte[i];
    }

    return NULL;
}

/**
 * Funzione booleana che determina se la mano del giocatore è piena oppure no.
 * Il mazzo della mano è pieno se il numero di carte è pari al numero massimo di punti vita.
 *
 * @param giocatore Il giocatore di cui verificare la mano
 * @return Vero se carteMano ha lo stesso numero di carte di maxPuntiVita, altrimenti falso
 */
bool manoPiena(Giocatore giocatore) {
    return giocatore.carteMano.numeroCarte == prendiVitaMassima(giocatore);
}

/**
 * Funzione che restituisce la vita massima di un giocatore in base al suo ruolo.
 *
 * @param giocatore Il giocatore di cui si vuole conoscere la vita massima.
 * @return Il numero massimo di punti vita di un giocatore.
 */
int prendiVitaMassima(Giocatore giocatore) {
    int maxPuntiVita;

    // determino il numero massimo di punti vita di un giocatore in base al ruolo
    if(giocatore.ruoloAssegnato == SCERIFFO) {
        maxPuntiVita = PUNTI_VITA_SCERIFFO;
    } else {
        maxPuntiVita = PUNTI_VITA_GENERICO;
    }

    return maxPuntiVita;
}

int calcolaGittata(Giocatore *giocatore) {
    int gittata = 0;
    // puntatore alla carta contenente l'arma del giocatore
    Carta* armaInUso = prendiArmaGiocatore(giocatore);

    // arma non trovata, quindi il giocatore è equipaggiato con una Colt.45
    if(armaInUso == NULL) {
        gittata += GITTATA_COLT;
    // arma trovata
    } else {
        if (strcmp(armaInUso->nomeCarta, "Carabina") == 0) {
            gittata += GITTATA_CARABINA;
        } else if (strcmp(armaInUso->nomeCarta, "Remington") == 0) {
            gittata += GITTATA_REMINGTON;
        } else if (strcmp(armaInUso->nomeCarta, "Schofield") == 0) {
            gittata += GITTATA_SCHOFIELD;
        } else if (strcmp(armaInUso->nomeCarta, "Winchester") == 0) {
            gittata += GITTATA_WINCHESTER;
        } else if (strcmp(armaInUso->nomeCarta, "Volcanic") == 0) {
            gittata += GITTATA_VOLCANIC;
        }
    }

    /*
     * TECNICAMENTE il mirino non influenza la gittata, bensì la distanza a cui un giocatore vede gli altri
    if(possiedeCartaInGioco(giocatore, "Mirino")) {
        gittata += 1;
    }
     */

    return gittata;
}

/**
 * Subroutine che rimuove a un giocatore un numero dato di punti vita.
 * La subroutine gestisce anche l'eventuale morte del giocatore, distribuendo bonus
 * e malus nel caso si verifichino specifici eventi, e permettendo anche a un player
 * di salvarsi dalla morte tramite la carta 'Birra'.
 *
 * @param giocatoreFerito Puntatore al giocatore a cui rimuovere i punti vita.
 * @param giocatoreAttaccante Puntatore al giocatore che ha attaccato quello ferito. Se il danno è dato da carte in gioco/eventi, vale NULL.
 * @param puntiVitaRimossi I punti vita da rimuovere.
 * @param mazzoPesca Puntatore al mazzo di pesca.
 * @param mazzoScarti Puntatore al mazzo di scarti.
 */
void rimuoviPuntiVita(Giocatore *giocatoreFerito, Giocatore *giocatoreAttaccante, int puntiVitaRimossi, Mazzo *mazzoPesca, Mazzo *mazzoScarti) {
    // posizione della (se esistente) carta birra, che può essere usata per salvarsi in punto di morte
    int cartaBirra;
    // nome del ruolo dell'utente morto
    char nomeRuolo[NOME_RUOLO_LEN_MAX + 1];

    // logica di morte del giocatore
    if(puntiVitaRimossi >= giocatoreFerito->puntiVita) {
        printf("\nIl giocatore %s è stato ferito a morte!", giocatoreFerito->nomeUtente);

        cartaBirra = cercaCartaMazzoPerNome(giocatoreFerito->carteMano, "Birra");
        // il giocatore può usare una carta birra per salvarsi
        if(cartaBirra == -1) { // se la carta non è presente, il giocatore è morto
            printf("\nIl giocatore %s non può salvarsi, ed è pertanto eliminato dal gioco!", giocatoreFerito->nomeUtente);

            // rivelazione del ruolo
            prendiNomeRuolo(giocatoreFerito->ruoloAssegnato, nomeRuolo);
            printf("\nRivelazione del ruolo del giocatore morto: %s!", nomeRuolo);

            // il player è stato ucciso da un altro giocatore, e non dal gioco: verifica dei bonus e malus
            if(giocatoreAttaccante != NULL) {
                // malus: eliminazione del viceceriffo da parte dello sceriffo
                if(giocatoreFerito->ruoloAssegnato == VICESCERIFFO && giocatoreAttaccante->ruoloAssegnato == SCERIFFO) {
                    printf("\nUn VICESCERIFFO è stato ucciso dallo sceriffo! %s deve scartare tutte le sue carte!", giocatoreAttaccante->nomeUtente);
                    while (giocatoreAttaccante->carteMano.numeroCarte > 0) {
                        scartaCarta(&giocatoreAttaccante->carteMano, mazzoScarti);
                    }
                // bonus: eliminazione di un fuorilegge
                } else if(giocatoreFerito->ruoloAssegnato == FUORILEGGE) {
                    printf("\nE' stato eliminato un FUORILEGGE! %s può pescare 3 carte!", giocatoreAttaccante->nomeUtente);
                    pescaCarte(mazzoPesca, mazzoScarti, giocatoreAttaccante, 3);
                }
            }
        } else {
            printf("\n%s possiede una carta 'Birra'! Usandola, si salva da morte certa, rimanendo però con un punto vita!", giocatoreFerito->nomeUtente);
            giocatoreFerito->puntiVita = 1; // imposto la vita a uno e termino la funzione
            return;
        }
    }
    // rimozione dei punti vita persi
    giocatoreFerito->puntiVita -= puntiVitaRimossi;
    // e spostamento delle carte del morto nel mazzo degli scarti
    for(int i = 0; i < giocatoreFerito->carteMano.numeroCarte; i++) {
        aggiungiCartaMazzo(mazzoScarti, giocatoreFerito->carteMano.carte[i]);
        rimuoviCartaMazzo(&giocatoreFerito->carteMano, i);
    }
}

void mostraVitaGiocatori(int nGiocatori, Giocatore giocatori[nGiocatori]) {
    int i;

    printf("\n%s VITA GIOCATORI %s", MEZZO_SEPARATORE, MEZZO_SEPARATORE);
    for(i = 0; i < nGiocatori; i++) {
        if(giocatori[i].puntiVita > 0)
            printf("\n*) Punti vita di %s: %d", giocatori[i].nomeUtente, giocatori[i].puntiVita);
    }
    printf("\n%s VITA GIOCATORI %s\n", MEZZO_SEPARATORE, MEZZO_SEPARATORE);
}

/**
 * Funzione che permette a un giocatore di scegliere tra una lista di giocatori mostrando un prompt di scelta,
 * effettuando tutti i controlli relativi al nome scelto. Restituisce il puntatore al giocatore selezionato.
 *
 * @param nGiocatori Numero totale di giocatori.
 * @param giocatori Array dei giocatori.
 * @param posizioneGiocatore Posizione del giocatore che sta scegliendo un altro giocatore.
 * @return Puntatore al giocatore scelto.
 */
Giocatore *selezionaGiocatore(int nGiocatori, Giocatore giocatori[nGiocatori], int posizioneGiocatore, bool mostraDistanze, bool calcolaDistanze) {
    int i;
    // variabile che contiene il nome del giocatore selezionato
    char nomeGiocatoreScelto[NOME_UTENTE_LEN + 1];
    // puntatore al giocatore che è stato selezionato
    Giocatore* giocatoreScelto = NULL;
    // puntatore all'array contenente le distanze dei giocatori
    int* distanze = NULL;

    if(mostraDistanze || calcolaDistanze)
        distanze = calcolaDistanzeGiocatori(nGiocatori, giocatori, posizioneGiocatore);

    // ciclo di scelta
    do {
        // mostro la lista dei giocatori
        printf("\n%s GIOCATORI %s", MEZZO_SEPARATORE, MEZZO_SEPARATORE);
        for (i = 0; i < nGiocatori; i++) {
            if (strcmp(giocatori[i].nomeUtente, giocatori[posizioneGiocatore].nomeUtente) == 0 || giocatori[i].puntiVita == 0)
                continue;
            if(mostraDistanze) {
                printf("\n*) %s (distanza: %d)", giocatori[i].nomeUtente, distanze[i]);
            } else {
                printf("\n*) %s", giocatori[i].nomeUtente);
            }
        }
        printf("\n%s GIOCATORI %s\n", MEZZO_SEPARATORE, MEZZO_SEPARATORE);

        // chiedo il nome del giocatore da scegliere
        printf("\nInserisci il nome del giocatore scelto.\n"
               "?) ");
        scanf(" %49s", nomeGiocatoreScelto);

        // verifico che il nome fornito sia valido
        // il nome del giocatore non può esssere uguale a quello di chi sceglie
        if (strcmp(nomeGiocatoreScelto, giocatori[posizioneGiocatore].nomeUtente) != 0) {
            // itero finché un giocatore con il nome fornito non è trovato, o fino alla fine della lista
            for (i = 0; i < nGiocatori && giocatoreScelto == NULL; i++) {
                // un giocatore è stato trovato
                if (strcmp(giocatori[i].nomeUtente, nomeGiocatoreScelto) == 0) {
                    // se richiesto, considero anche la distanza nella scelta
                    if(calcolaDistanze && distanze[i] > giocatori[posizioneGiocatore].gittata) {
                        printf("\nIl giocatore scelto è troppo lontano!");
                    // il giocatore è morto
                    } else if (giocatori[i].puntiVita < 1) {
                        printf("\nIl giocatore scelto è invalido! Inserisci un altro nome.");
                    } else {
                        giocatoreScelto = &giocatori[i];
                    }
                    break;
                }
            }

            if(giocatoreScelto == NULL) {
                if(i == nGiocatori) // se il ciclo termina e giocatoreScelto è null, significa che il nome fornito non esiste
                    printf("\nNon esiste un giocatore con il nome fornito!");
                printf("\nIl giocatore fornito è invalido! Scegline un altro.");
            }
        } else {
            printf("\nNon puoi scegliere te stesso!");
        }
    } while (giocatoreScelto == NULL); // in questo modo il ciclo si ripete finché un giocatore non è trovato

    return giocatoreScelto;
}

bool possiedeCartaInGioco(Giocatore giocatore, char nomeCarta[NOME_CARTA_LEN + 1]) {
    return cercaCartaMazzoPerNome(giocatore.carteGioco, nomeCarta) != -1;
}