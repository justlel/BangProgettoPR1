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
    // array delle distanze
    int* distanze = NULL;

    /* La 'distanza' tiene conto della distanza "grezza", contando solo i giocatori in vita.
    * La 'distanzaUtente', invece, è quella che viene mostrata al giocatore dopo aver preso
    * in considerazione gli effetti di altre carte, inseriti in 'effettiCarteDistanza'
    */
    int i, distanza = 1, effettiCarteDistanza = 0, distanzaUtente;

    // variabile booleana di appoggio per ricordarsi al termine della funzione se il mustang è presente o meno
    bool mustang = false;


    distanze = (int*) calloc(nGiocatori, sizeof(int));
    if(distanze == NULL) {
        printf("\nImpossibile allocare dinamicamente memoria.");
        exit(-1); // TODO: utils
    }

    // verifica che l'utente abbia un mirino in gioco, che permette di vedere gli altri a distanza diminuita.
    if(possiedeCartaInGioco(giocatori[posizioneGiocatore], "Mirino")) {
        effettiCarteDistanza = -1;
    }

    // la distanza dal giocatore a sé stesso è convenzionalmente 0
    distanze[posizioneGiocatore] = 0;

    // conta delle distanze dei giocatori alla DESTRA dell'utente
    for(i = posizioneGiocatore + 1; i < nGiocatori; i++) {
        // se un giocatore morto si trova tra due giocatori in vita, allora quella distanza non conta
        if(giocatori[i].puntiVita == 0)
            continue;
        // incrementa la distanza di 1 dato che c'è un giocatore vivo
        distanza++;

        // verifico che il giocatore in considerazione abbia o meno una carta 'mustang', che aumenta di 1 la distanza
        if(possiedeCartaInGioco(giocatori[i], "Mustang")) {
            effettiCarteDistanza++;
            mustang = true;
        }

        // calcolo della distanza prendendo in considerazione le carte speciali
        distanzaUtente = distanza + effettiCarteDistanza;
        // e ricordando che non può mai essere < 1
        if(distanzaUtente < 1)
            distanzaUtente = 1;

        // inserisco il valore della distanza nell'array
        distanze[i] = distanzaUtente;
        // 'resetto' gli effetti delle carte distanza, dato che il mustang (a differenza del mirino) vale sul singolo giocatore
        if(mustang) {
            effettiCarteDistanza--;
            mustang = false;
        }
    }

    // calcolo dei giocatori andando verso SINISTRA, stesso procedimento
    for(i = posizioneGiocatore - 1; i >= 0; i--) {
        // se un giocatore morto si trova tra due giocatori in vita, allora quella distanza non conta
        if(giocatori[i].puntiVita == 0)
            continue;
        // incrementa la distanza di 1 dato che c'è un giocatore vivo
        distanza++;

        // verifico che il giocatore in considerazione abbia o meno una carta 'mustang', che aumenta di 1 la distanza
        mustang = false;
        if(possiedeCartaInGioco(giocatori[i], "Mustang")) {
            effettiCarteDistanza++;
            mustang = true;
        }

        // calcolo della distanza prendendo in considerazione le carte speciali
        distanzaUtente = distanza + effettiCarteDistanza;
        // e ricordando che non può mai essere < 1
        if(distanzaUtente < 1)
            distanzaUtente = 1;

        // inserisco il valore della distanza nell'array
        distanze[i] = distanzaUtente;
        // 'resetto' gli effetti delle carte distanza, dato che il mustang (a differenza del mirino) vale sul singolo giocatore
        if(mustang) {
            effettiCarteDistanza--;
            mustang = false;
        }
    }

    // restituisco l'array con le distanze
    return distanze;
}

/**
 * Subroutine mostra all'utente la sua distanza dagli altri giocatori.
 *
 * @param nGiocatori Il numero di giocatori in partita.
 * @param posizioneGiocatore L'indice del giocatore da cui calcolare le distanze.
 * @param giocatori I giocatori in partita.
 */
void mostraDistanze(int nGiocatori, Giocatore giocatori[], int posizioneGiocatore) {
    int i;
    // calcolo le distanze con la funzione 'calcolaDistanzeGiocatori'
    int* distanze = calcolaDistanzeGiocatori(nGiocatori, giocatori, posizioneGiocatore);

    // itero sulle distanze calcolate
    for(i = 0; i < nGiocatori; i++) {
        if(i != posizioneGiocatore) { // non è necessario mostrare la posizione dal giocatore
            // è necessario evidenziare lo sceriffo tra gli altri giocatori
            if(giocatori[i].ruoloAssegnato == SCERIFFO) {
                printf("\nSCERIFFO) Lo SCERIFFO, %s, dista %d posizioni.", giocatori[i].nomeUtente, distanze[i]);
            } else {
                printf("\n*) Il giocatore %s dista %d posizioni.", giocatori[i].nomeUtente, distanze[i]);
            }
        }
    }
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
        if(strcmp(giocatori[i].nomeUtente, giocatori[posizioneGiocatore].nomeUtente) != 0) {
            printf("\n------ Carte in gioco di %s ------", giocatori[i].nomeUtente);
            mostraCarteMazzo(giocatori[i].carteGioco);
        }
    }
}

// TODO: Aggiungere documentazione
bool giocaCarta(int nGiocatori, Giocatore giocatori[nGiocatori], int posizioneGiocatore, Carta carta, Mazzo* mazzoPesca) {
    int i;
    Giocatore *giocatore = &giocatori[posizioneGiocatore];
    bool confermaCarta;
    char confermaAzione;

    // PRIMA PARTE: carte istantanee
    if (carta.tipologiaCarta == ISTANTANEA) {
        if (strcmp(carta.nomeCarta, "Bang!") == 0) {
            int j;
            // intero che contiene la gittata massima dell'utente (fino a quale distanza gli altri utenti possono essere colpiti)
            int gittata = 0;
            // puntatore all'array contenente le distanze degli altri giocatori
            int* distanzeGiocatori = calcolaDistanzeGiocatori(nGiocatori, giocatori, posizioneGiocatore);
            // il nome del giocatore a cui si intende sparare
            char nomeBersaglio[NOME_UTENTE_LEN + 1];
            // e il puntatore al giocatore bersagliato
            Giocatore* giocatoreBersaglio = NULL;
            // arma in uso dal giocatore, ottenuta dalla funzione prendiArmaGiocatore
            Carta armaInUso = prendiArmaGiocatore(*giocatore);

            // inizializzazione dell'array a 0, poi la dimensione sarà riaggiustata al calcolo delle distanze
            int *giocatoriRaggiungibili = calloc(0, sizeof(int));
            if (giocatoriRaggiungibili == NULL) {
                printf("\nImpossibile allocare dinamicamente memoria.");
                exit(-1);
            }

            printf("\nLa carta 'Bang!' ti permette di infliggere danni a un giocatore.");
            // calcolo della gittata
            printf("\nAl momento, la tua gittata totale è data da questi fattori:");

            if (strcmp(armaInUso.nomeCarta, "Carabina") == 0) {
                printf("\nArma: CARABINA (+%d gittata)", GITTATA_CARABINA);
                gittata += GITTATA_CARABINA;
            } else if (strcmp(armaInUso.nomeCarta, "Remington") == 0) {
                printf("\nArma: REMINGTON (+%d gittata)", GITTATA_REMINGTON);
                gittata += GITTATA_REMINGTON;
            } else if (strcmp(armaInUso.nomeCarta, "Schofield") == 0) {
                printf("\nArma: SCHOFIELD (+%d gittata)", GITTATA_SCHOFIELD);
                gittata += GITTATA_SCHOFIELD;
            } else if (strcmp(armaInUso.nomeCarta, "Winchester") == 0) {
                printf("\nArma: WINCHESTER (+%d gittata)", GITTATA_WINCHESTER);
                gittata += GITTATA_WINCHESTER;
            } else if (strcmp(armaInUso.nomeCarta, "Volcanic") == 0) {
                printf("\nArma: VOLCANIC (+%d gittata)", GITTATA_VOLCANIC);
                gittata += GITTATA_VOLCANIC;
            }

            // mostro all'utente i giocatori e le loro distanze
            printf("\nConsiderate le carte a disposizione, questi sono i giocatori a cui puoi infliggere danno:");
            mostraDistanze(nGiocatori, giocatori, posizioneGiocatore);

            // chiedo all'utente a quale giocatore vorrebbe sparare, finché non conferma la sua scelta
            do {
                printf("\nInserisci il nome del giocatore a cui vorresti sparare: ");
                do {
                    scanf(" %49s", nomeBersaglio);
                    // il nome fornito deve essere valido
                    if(strcmp(nomeBersaglio, giocatore->nomeUtente) == 0) {
                        printf("\nNon puoi sparare a te stesso!");
                    } else {
                        for(j = 0; j < nGiocatori; j++) {
                            if(strcmp(nomeBersaglio, giocatori[j].nomeUtente) == 0) {
                                if (giocatori[j].puntiVita < 1) {
                                    printf("\nNon puoi sparare a questo giocatore!");
                                } else if(distanzeGiocatori[j] < gittata) {
                                    printf("\nQuesto giocatore è troppo lontano!");
                                } else {
                                    giocatoreBersaglio = &giocatori[j];
                                }
                                break; // un giocatore con il nome fornito (anche se invalido) è già stato trovato
                            }
                        }
                    }

                } while (giocatoreBersaglio == NULL);
                printf("\nStai per sparare a %s! Confermi la tua scelta?\n"
                       "%c/%c) ", nomeBersaglio, PROMPT_CONFERMA, PROMPT_RIFIUTA);
                confermaAzione = getchar();
                confermaCarta = confermaAzione != PROMPT_RIFIUTA;
                if (!confermaCarta) {
                    printf("\nDesideri cambiare giocatore? Inserendo no, tornerai al menu principale."
                           "%c/%c) ", PROMPT_CONFERMA, PROMPT_RIFIUTA);
                    confermaAzione = getchar();
                    if (confermaAzione == PROMPT_RIFIUTA) {
                        printf("\nTorno al menu principale!");
                        free(giocatoriRaggiungibili);
                        return false;
                    }
                }
            } while (!confermaCarta);

            // verifica di eventuali azioni per evitare lo sparo
            if (possiedeCartaInGioco(*giocatoreBersaglio, "Barile")) {
                printf("\nIl giocatore %s possiede una carta 'Barile'. Sarà estratta una carta che, se sarà"
                       "del seme di Cuori, annullerà completamente l'attacco.",
                       nomeBersaglio);
                Carta cartaEstratta = pescaCimaMazzo(mazzoPesca, 1)[0];
                if (cartaEstratta.semeCarta == CUORI) {
                    printf("\nLa carta estratta è un %d di Cuori! L'attacco è mancato!", cartaEstratta.numeroCarta);
                    free(giocatoriRaggiungibili);
                    return true;
                } else {
                    printf("\nLa carta estratta non è una carta di Cuori!");
                }
            }
            int indiceCartaMancato = cercaCartaMazzoPerNome(giocatoreBersaglio->carteMano, "Mancato!");
            if (indiceCartaMancato != -1) {
                printf("\nIl giocatore %s possiede una carta Mancato! Se deciderà di giocarla, potrà evitare l'attacco.",
                       nomeBersaglio);
                printf("\nDesideri scartare una carta 'Mancato!' ed evitare l'attacco?\n"
                       "%c/%c)", PROMPT_CONFERMA, PROMPT_RIFIUTA);
                getchar(); // pulisco il buffer
                confermaAzione = getchar();
                if (confermaAzione == PROMPT_CONFERMA) {
                    printf("\n%s gioca un 'Mancato!' ed evita l'attacco!", nomeBersaglio);
                    rimuoviCartaMazzo(&giocatoreBersaglio->carteMano,
                                      giocatoreBersaglio->carteMano.carte[indiceCartaMancato]);
                    return true;
                } else {
                    printf("\n%s non ha giocato un 'Mancato!'.", nomeBersaglio);
                }
            }
            // rimozione del punto vita e fine della funzione
            printf("\n%s ha sparato a %s, che perde quindi un punto vita!", giocatore->nomeUtente, nomeBersaglio);
            rimuoviPuntiVita(giocatoreBersaglio, 1);
        } else if (strcmp(carta.nomeCarta, "Birra") == 0) {
            // verifico che il giocatore non abbia già il massimo della vita
            if ((giocatore->ruoloAssegnato == SCERIFFO && giocatore->puntiVita == PUNTI_VITA_SCERIFFO) ||
                (giocatore->ruoloAssegnato != SCERIFFO && giocatore->puntiVita == PUNTI_VITA_GENERICO)
                    ) {
                printf("\nHai già il massimo della vita!");
                return false;
            }
            printf("\n%s ha utilizzato una birra e guadagna un punto vita!", giocatore->nomeUtente);
            giocatori[posizioneGiocatore].puntiVita++;
        } else if (strcmp(carta.nomeCarta, "Diligenza") == 0) {
            printf("\n%s pesca 2 carte!", giocatore->nomeUtente);
            pescaCarte(mazzoPesca, &giocatori[posizioneGiocatore], 2);
        } else if (strcmp(carta.nomeCarta, "Panico!") == 0) {
            int j;
            // puntatore all'array contenente le distanze dagli altri giocatori
            int* distanzeGiocatori = calcolaDistanzeGiocatori(nGiocatori, giocatori, posizioneGiocatore);
            // nome del giocatore a cui sarà rubata una carta
            char nomeBersaglio[NOME_UTENTE_LEN + 1];
            // e il suo puntatore
            Giocatore* giocatoreBersaglio = NULL;

            // mostro le distanze dagli altri giocatori
            mostraDistanze(nGiocatori, giocatori, posizioneGiocatore);

            // chiedo all'utente il nome del giocatore che intende bersagliare
            do {
                printf("\nInserisci il nome del giocatore a cui rubare una carta!\n"
                       "?) ");
                scanf(" %49s", nomeBersaglio);
                if(strcmp(nomeBersaglio, giocatore->nomeUtente) == 0) {
                    printf("\nNon puoi rubare una carta a te stesso!");
                } else {
                    for (j = 0; j < nGiocatori; j++) {
                        if(giocatori[j].puntiVita < 1) {
                            printf("\nNon puoi sparare a questo giocatore!");
                        } else if(distanzeGiocatori[j] != 1) {
                            printf("\nPuoi selezionare solo giocatori a distanza uno!");
                        } else {
                            giocatoreBersaglio = &giocatori[j];
                        }
                    }
                }
            } while(giocatoreBersaglio == NULL);

            printf("\nBene! Queste sono le carte di %s:", nomeBersaglio);
        } else if (strcmp(carta.nomeCarta, "CatBalou!") == 0) {
            int j, indiceGiocatoreScelto;
            char giocatoreScelto[NOME_UTENTE_LEN + 1];
            bool ripetizioneCiclo = true;

            do {
                printf("\nScegli il giocatore a cui far scartare una carta!");
                for (j = 0; j < nGiocatori; j++) {
                    if (strcmp(giocatori[j].nomeUtente, giocatore->nomeUtente) == 0 || giocatori[j].puntiVita == 0)
                        continue;
                    printf("*) %s", giocatori[j].nomeUtente);
                }

                printf("?) ");
                do {
                    scanf(" %49s", giocatoreScelto);
                    if (strcmp(giocatoreScelto, giocatore->nomeUtente) != 0) {
                        for (j = 0; ripetizioneCiclo || j < nGiocatori; j++) {
                            if (strcmp(giocatori[j].nomeUtente, giocatoreScelto) == 0) {
                                indiceGiocatoreScelto = j;
                                ripetizioneCiclo = false;
                            }
                        }
                    }
                    ripetizioneCiclo = ripetizioneCiclo && giocatori[indiceGiocatoreScelto].puntiVita > 0;
                    if (ripetizioneCiclo)
                        printf("\nIl nome inserito non è valido! Inserisci un altro utente: ");
                } while (ripetizioneCiclo);

                printf("\nConfermi la tua scelta di sparare a %s?"
                       "%c/%c) ", giocatori[indiceGiocatoreScelto].nomeUtente, PROMPT_CONFERMA, PROMPT_RIFIUTA);
                confermaAzione = getchar();
                confermaCarta = confermaAzione != PROMPT_RIFIUTA;
                if (!confermaCarta) {
                    printf("\nDesideri scegliere un altro giocatore? Inserendo no, tornerai al menu principale\n"
                           "%c/%c) ", PROMPT_CONFERMA, PROMPT_RIFIUTA);
                    confermaAzione = getchar();
                    if (confermaAzione == PROMPT_RIFIUTA) {
                        printf("\nTorno al menu principale!");
                        return false;
                    }
                }
            } while (!confermaCarta);

            printf("\n%s ha giocato un Cat Balou contro %s, che deve scartare una carta!", giocatore->nomeUtente,
                   giocatoreScelto);
            printf("\nPassa lo schermo a %s per permettergli di scegliere quale carta scartare.", giocatoreScelto);
            scartaCarta(&giocatori[indiceGiocatoreScelto], NULL);
        } else if (strcmp(carta.nomeCarta, "Mancato!") == 0) {
            printf("\nNon puoi giocare questa carta direttamente, ma solo quando stai subendo uno sparo!");
        } else if (strcmp(carta.nomeCarta, "Gatling") == 0) {
            int j;

            printf("\n%s ha giocato un Gatling: tutti i giocatori perdono un punto vita!", giocatore->nomeUtente);
            for (j = 0; j < nGiocatori; j++) {
                if (j != posizioneGiocatore && giocatori[j].puntiVita > 0) {
                    rimuoviPuntiVita(&giocatori[j], 1);
                }
            }
        } else if (strcmp(carta.nomeCarta, "Saloon") == 0) {
            int j;

            printf("\n%s ha giocato un Saloon: tutti i giocatori guadagnano un punto vita!", giocatore->nomeUtente);
            for (j = 0; j < nGiocatori; j++) {
                if (j != posizioneGiocatore && giocatori[j].puntiVita > 0) {
                    giocatori[j].puntiVita++;
                }
            }
        } else if (strcmp(carta.nomeCarta, "WellsFargo") == 0) {
            printf("\n%s pesca 3 carte!", giocatore->nomeUtente);
            pescaCarte(mazzoPesca, &giocatori[posizioneGiocatore], 3);
        }
        // SECONDA PARTE: carte istantanee speciali
    } else if (carta.tipologiaCarta == ISTANTANEA_SPECIAL) {
        if (strcmp(carta.nomeCarta, "Duello") == 0) {
            int j, indiceGiocatoreScelto, turnoDuello = 0, cartaBangDaScartare;
            char giocatoreScelto[NOME_UTENTE_LEN + 1];
            bool ripetizioneCiclo, vittoriaSfidante = false, vittoriaSfidato = false;
            Giocatore *vincitoreDuello;

            printf("\nInserisci il nome del giocatore da sfidare a duello!");
            for (j = 0; j < nGiocatori; j++) {
                if (strcmp(giocatori[j].nomeUtente, giocatore->nomeUtente) == 0 || giocatori[j].puntiVita == 0)
                    continue;
                printf("*) %s", giocatori[j].nomeUtente);
            }

            printf("?) ");
            do {
                do {
                    scanf(" %49s", giocatoreScelto);
                    if (strcmp(giocatoreScelto, giocatore->nomeUtente) != 0) {
                        for (j = 0; ripetizioneCiclo || j < nGiocatori; j++) {
                            if (strcmp(giocatori[j].nomeUtente, giocatoreScelto) == 0) {
                                indiceGiocatoreScelto = j;
                                ripetizioneCiclo = false;
                            }
                        }
                    }
                    ripetizioneCiclo = ripetizioneCiclo && giocatori[indiceGiocatoreScelto].puntiVita > 0;
                    if (ripetizioneCiclo)
                        printf("\nIl nome inserito non è valido! Inserisci un altro utente: ");
                } while (ripetizioneCiclo);

                printf("\nSei sicuro di voler sfidare %s a duello?\n"
                       "%c/%c) ", giocatori[indiceGiocatoreScelto].nomeUtente, PROMPT_CONFERMA, PROMPT_RIFIUTA);
                confermaAzione = getchar();
                confermaCarta = confermaAzione != PROMPT_RIFIUTA;
                if (!confermaCarta) {
                    printf("\nDesideri scegliere un altro giocatore? Inserendo no, tornerai al menu principale\n"
                           "%c/%c) ", PROMPT_CONFERMA, PROMPT_RIFIUTA);
                    confermaAzione = getchar();
                    if (confermaAzione == PROMPT_RIFIUTA) {
                        printf("\nTorno al menu principale!");
                        return false;
                    }
                }
            } while (!confermaCarta);

            printf("\nSi dia inizio al duello tra %s e %s!", giocatore->nomeUtente,
                   giocatori[indiceGiocatoreScelto].nomeUtente);
            printf("\nEstrarrò una carta 'Bang!' una volta a testa per ciascun giocatore. Il primo a finirle perderà un punto vita!");

            do {
                // è il turno del giocatore sfidato
                if (turnoDuello % 2 == 0) {
                    cartaBangDaScartare = cercaCartaMazzoPerNome(giocatori[indiceGiocatoreScelto].carteMano, "Bang!");
                    vittoriaSfidante = cartaBangDaScartare == -1;
                    if (vittoriaSfidante) {
                        printf("\n%s scarta una carta Bang! Ora tocca allo sfidante.",
                               giocatori[indiceGiocatoreScelto].nomeUtente);
                        rimuoviCartaMazzo(&giocatori[indiceGiocatoreScelto].carteMano,
                                          giocatori[indiceGiocatoreScelto].carteMano.carte[cartaBangDaScartare]);
                    }
                    // è il turno dello sfidante
                } else {
                    cartaBangDaScartare = cercaCartaMazzoPerNome(giocatore->carteMano, "Bang!");
                    vittoriaSfidato = cartaBangDaScartare == -1;
                    if (vittoriaSfidato) {
                        printf("\n%s scarta una carta Bang! Ora tocca al giocatore sfidato.", giocatore->nomeUtente);
                        rimuoviCartaMazzo(&giocatori[indiceGiocatoreScelto].carteMano,
                                          giocatori[indiceGiocatoreScelto].carteMano.carte[cartaBangDaScartare]);
                    }
                }
                turnoDuello++;
            } while (!vittoriaSfidante && !vittoriaSfidato);

            if (vittoriaSfidante) {
                vincitoreDuello = giocatore;
            } else {
                vincitoreDuello = &giocatori[indiceGiocatoreScelto];
            }
            printf("%s ha esaurito le carte Bang!, e pertanto perde il duello!", vincitoreDuello->nomeUtente);
            rimuoviPuntiVita(vincitoreDuello, 3);
        } else if (strcmp(carta.nomeCarta, "Emporio") == 0) {
            printf("\nLa carta 'Emporio' estrae dal mazzo di pesca un numero di carte pari a quello dei giocatori in vita,"
                   "per poi permettere ad ogni utente (a partire da chi ha giocato la carta) di pescare una carta da questo mazzo.");
            int j, k, giocatoriInVita, cartaScelta, nCarteEstratte = 0;
            Carta *carteEstratte = NULL; // array dinamico contenente le carte estratte dal mazzo di pesca
            Carta *carteEstratteTmp = NULL; // array utilizzato quando viene rimossa una carta dal mazzo di sopra

            // calcolo dei giocatori in vita
            for (j = 0; j < nGiocatori; j++) {
                if (giocatori[j].puntiVita > 0)
                    giocatoriInVita++;
            }
            printf("\nEssendoci %d giocatori in vita, saranno pescate %d carte.", giocatoriInVita, giocatoriInVita);

            // creazione dell'array dinamico contenente tante carte estratte quanti sono i giocatori in vita
            carteEstratte = (Carta *) calloc(giocatoriInVita, sizeof(Carta));
            if (carteEstratte == NULL) {
                printf("\nErrore: impossibile allocare dinamicamente"); // TODO: 'utils'
                exit(-1);
            }
            // e allocazione della memoria sufficiente per l'array in cui inserire temporaneamente le carte che non sono state selezionate
            // di dimensione giocatoriInVita - 1, dato che si può estrarre solo una carta alla volta per giocatore
            carteEstratteTmp = (Carta *) calloc(giocatoriInVita - 1, sizeof(Carta));
            if (carteEstratteTmp == NULL) {
                printf("\nErrore: impossibile allocare dinamicamente"); // TODO: 'utils'
                exit(-1);
            }

            // popolazione dell'array dinamico appena dichiarato
            carteEstratte = pescaCimaMazzo(mazzoPesca, giocatoriInVita);

            // a partire dal giocatore che ha estratto la carta, chiedo di pescarne una da quelle appena ottenute
            j = posizioneGiocatore;
            while (nCarteEstratte < giocatoriInVita) {
                printf("\n%s, è il tuo turno di pescare!", giocatori[j].nomeUtente);
                if (giocatori[j].carteMano.numeroCarte == giocatori[j].puntiVita) {
                    printf("\nMa prima devi scegliere una carta da scartare.");
                    scartaCarta(&giocatori[j], NULL);
                }

                printf("\nScegli una carta da pescare:");
                for (k = 0; k < giocatoriInVita - nCarteEstratte; k++) {
                    printf("%d) %s", k + 1, carteEstratte[k].nomeCarta);
                }
                do {
                    printf("\nInserisci il numero della carta da pescare: \n"
                           "?) ");
                    scanf("%d", &cartaScelta);
                    if (cartaScelta <= 0 || cartaScelta > giocatoriInVita)
                        printf("\nIl valore inserito non è valido!");
                } while (cartaScelta <= 0 || cartaScelta > giocatoriInVita);
                printf("\nBene! Hai scelto di pescare una carta '%s'.", carteEstratte[cartaScelta - 1].nomeCarta);

                // rimozione della carta dal mazzetto pescato e aggiunta alla mano del giocatore
                for (k = 0; k < giocatoriInVita - nCarteEstratte; k++) {
                    if (k != cartaScelta - 1)
                        carteEstratteTmp[k] = carteEstratte[k];
                }
                // è stata estratta una carta
                nCarteEstratte++;
                // aggiunta della carta estratta al mazzo del giocatore
                aggiungiCartaMazzo(&giocatori[j].carteMano, carteEstratte[cartaScelta - 1]);
                // elimino il vecchio mazzetto di carte
                free(carteEstratte);
                // e al suo posto uso quello senza la carta che è stata selezionata
                carteEstratte = carteEstratteTmp;
                // TODO: Da verificare
                // creo un nuovo puntatore a un array vuoto per il mazzo temporaneo, perché se utilizzassi
                // una realloc() starei modificando anche carteEstratte
                carteEstratteTmp = (Carta *) calloc(giocatoriInVita - nCarteEstratte, sizeof(Carta));
                // uso il modulo per tornare all'inizio nel caso in cui dovessi aver raggiunto la fine dell'array giocatori
                j = (j + 1) % nGiocatori;
            }
            free(carteEstratte);
            free(carteEstratteTmp);
            printf("\nEstrazione terminata!");
        } else if (strcmp(carta.nomeCarta, "Indiani") == 0) {
            int j, posizioneCartaBang;

            printf("\nAttacco di indiani in arrivo! Ogni giocatore (eccetto chi ha giocato la carta) deve giocare"
                   "una carta 'Bang!', oppure perdere un punto vita!");
            for (j = 0; j < nGiocatori; j++) {
                if (j != posizioneGiocatore && giocatori[j].puntiVita > 0) {
                    posizioneCartaBang = cercaCartaMazzoPerNome(giocatori[j].carteMano, "Bang!");
                    if (posizioneCartaBang == -1) {
                        printf("\n%s non possiede una carta 'Bang!', e perde quindi un punto vita!",
                               giocatori[j].nomeUtente);
                        rimuoviPuntiVita(&giocatori[j], 1);
                    } else {
                        printf("\n%s scarta una carta 'Bang!' ed evita l'attacco degli indiani!",
                               giocatori[j].nomeUtente);
                        rimuoviCartaMazzo(&giocatori[j].carteMano, giocatori[j].carteMano.carte[posizioneCartaBang]);
                    }
                }
            }
        }
        // TERZA PARTE: armi
    } else if (carta.tipologiaCarta == ARMA) {

        // QUARTA PARTE: carte in gioco
    } else if (carta.tipologiaCarta == EFFETTO) {
        if (strcmp(carta.nomeCarta, "Barile") == 0) {
            printf("\nLa carta 'Barile' ti consente, fintanto che è in gioco, di schivare una carta 'Bang!'.\n"
                   "Quando vieni colpito, puoi scegliere di estrarre una carta: se questa è di Cuori, allora l'attacco è annullato.");
            aggiungiCartaInGioco(giocatore, giocatore, carta);
            printf("\nHai giocato una carta 'Barile'!");
        } else if (strcmp(carta.nomeCarta, "Mirino") == 0) {
            printf("\nLa carta 'Mirino' ti consente, fintanto che è in gioco, di vedere tutti gli altri giocatori a una distanza diminuita di uno.");
            aggiungiCartaInGioco(giocatore, giocatore, carta);
            printf("\nHai equipaggiato la carta 'Mirino'!");
        } else if (strcmp(carta.nomeCarta, "Mustang") == 0) {
            printf("\nLa carta 'Mustang' ti consente, fintanto che è in gioco, di essere visto a distanza aumentata di uno dagli altri giocatori.");
            aggiungiCartaInGioco(giocatore, giocatore, carta);
            printf("\nHai equipaggiato la carta 'Mustang'!");
        } else if (strcmp(carta.nomeCarta, "Dinamite") == 0) {
            printf("\nLa carta 'Dinamite' resta innocua per un intero giro. Al tuo prossimo turno, prima di iniziare devi estrarre una carta che, "
                   "se sarà di un numero compreso tra 2 e 9 e del seme di Picche, farà esplodere la dinamite, facendoti perdere 3 punti vita. Se, invece, "
                   "la dinamite non esplode, allora sarà passata al giocatore successivo, che effettuerà le stesse azioni, finché la dinamite non esplode o "
                   "viene scartata.");
            aggiungiCartaInGioco(giocatore, giocatore, carta);
            printf("\nHai messo un gioco una 'Dinamite'!");
        } else if (strcmp(carta.nomeCarta, "Prigione") == 0) {
            char nomeGiocatoreScelto[NOME_UTENTE_LEN];
            Giocatore *giocatoreScelto = NULL;
            bool giaInPrigione = false;

            // TODO: un giocatore che è già in prigione non può esserci rimesso
            printf("\nLa carta 'Prigione' ti permette di mettere in prigione un giocatore qualsiasi, tranne lo Sceriffo. Un giocatore in prigione, "
                   "prima di giocare il suo turno, estrae una carta: se questa non ha il seme di Cuori, allora la carta prigione è scartata e il giocatore "
                   "salta il turno (pur rimanendo bersaglio degli altri giocatori), altrimenti la prigione è scartata e il turno è giocabile normalmente.");
            printf("\nSu chi intendi giocare la carta?\n");
            for (i = 0; i < nGiocatori; i++) {
                if (giocatori[i].puntiVita > 0 && giocatori[i].ruoloAssegnato != SCERIFFO &&
                    strcmp(giocatori[i].nomeUtente, giocatore->nomeUtente) != 0) {
                    printf("\n*) %s", giocatori[i].nomeUtente);
                }
            }
            do {
                printf("?) ");
                scanf(" %48s", nomeGiocatoreScelto);
                for (int j = 0; j < nGiocatori; j++) {
                    if (strcmp(giocatori[j].nomeUtente, nomeGiocatoreScelto) == 0) {
                        giocatoreScelto = &giocatori[j];
                    }
                }
                if (strcmp(giocatore->nomeUtente, giocatoreScelto->nomeUtente) == 0) {
                    printf("\nNon puoi giocare la prigione su te stesso.");
                } else if (giocatoreScelto->ruoloAssegnato == SCERIFFO) {
                    printf("\nNon puoi giocare la prigione sullo sceriffo.");
                } else if (giocatoreScelto == NULL || giocatoreScelto->puntiVita == 0) {
                    printf("\nIl nome del giocatore non è valido.");
                } else if((giaInPrigione = possiedeCartaInGioco(*giocatoreScelto, "Prigione"))) { // TODO: abbastanza schifoso
                    printf("\nQuesto giocatore è già in prigione!");
                }
            } while (giocatoreScelto == NULL || giocatoreScelto->ruoloAssegnato == SCERIFFO ||
                     giocatoreScelto->puntiVita == 0 || giaInPrigione);
            aggiungiCartaInGioco(giocatore, giocatoreScelto, carta);
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
 * @param carta La carta da giocare.
 */
void aggiungiCartaInGioco(Giocatore* giocatoreCarta, Giocatore* giocatoreBersaglio, Carta carta) {
    rimuoviCartaMazzo(&giocatoreCarta->carteMano, carta);
    aggiungiCartaMazzo(&giocatoreBersaglio->carteGioco, carta);
}

/**
 * Subroutine che effettua l'azione di pesca dal mazzo di pesca per un giocatore.
 * Se l'utente possiede già il massimo numero di carte, allora gli viene richiesto di scartarne a scelta.
 *
 * @param mazzoPesca Puntatore al mazzo di pesca, da cui pescare le carte.
 * @param giocatore Puntatore al giocatore che deve pescare le carte.
 * @param nCarte Il numero di carte da pescare.
 */
void pescaCarte(Mazzo* mazzoPesca, Giocatore* giocatore, int nCarte) {
    int i;
    // array contenente le carte pescate dal mazzo di pesca
    Carta* cartePescate = pescaCimaMazzo(mazzoPesca, nCarte);

    // se il giocatore ha troppe carte, allora gli si chiede di scartare quelle extra
    if(giocatore->carteMano.numeroCarte == giocatore->puntiVita) {
        printf("\nHai già %d carte! Scegli %d carte da scartare.", giocatore->puntiVita, giocatore->puntiVita - nCarte);
        for(i = 0; i < giocatore->puntiVita - nCarte; i++) {
            scartaCarta(&giocatore->carteMano, NULL);
        }
    }

    // aggiungo le carte pescate alla mano del giocatore, e le stampo a schermo
    printf("\nQueste sono le carte che hai pescato!");
    for(i = 0; i < nCarte; i++) {
        aggiungiCartaMazzo(&giocatore->carteMano, cartePescate[i]);
        printf("\n%d) ", i+1);
        mostraCarta(cartePescate[i]);
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

    printf("\nScegli una carta da scartare:");
    // mostro al giocatore le carte nella sua mano
    mostraCarteInMano(*mazzoMano);
    do {
        printf("?) ");
        scanf("%d", &cartaScelta);
        // verifico la correttezza dell'input
        if(cartaScelta <= 0 || cartaScelta > mazzoMano->numeroCarte)
            printf("\nInserisci un numero tra %d e %d!\n", 1, mazzoMano->numeroCarte);
    } while(cartaScelta <= 0 || cartaScelta > mazzoMano->numeroCarte);

    // aggiungo la carta al mazzo degli scarti
    aggiungiCartaMazzo(mazzoScarti, mazzoMano->carte[cartaScelta - 1]);
    // rimuovo la carta dalla mano del giocatore
    rimuoviCartaMazzo(mazzoMano, mazzoMano->carte[cartaScelta - 1]);
}

/**
 * Funzione che restituisce l'arma attualmente in uso da un giocatore.
 *
 * @param giocatore Il giocatore di cui ottenere l'arma
 * @return L'arma in uso dal giocatore
 */
Carta prendiArmaGiocatore(Giocatore giocatore) {
    int i;

    for(i = 0; i < giocatore.carteGioco.numeroCarte; i++) {
        if(giocatore.carteGioco.carte[i].tipologiaCarta == ARMA)
            return giocatore.carteGioco.carte[i];
    }
}