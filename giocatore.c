#include "giocatore.h"

/**
 * Funzione che associa ad ogni elemento dell'enumerazione 'Ruoli' un nome,
 * che viene inserito nel puntatore alla stringa fornito.
 *
 * @param ruoli Il ruolo di cui si vuole ottenere il nome.
 * @param nomeRuolo La stringa in cui inserire il nome del ruolo.
 */
void prendiNomeRuolo(Ruoli ruoli, char nomeRuolo[]) {
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

    // allocazione dinamica dell'array delle distanze
    distanze = (int*) calloc(nGiocatori, sizeof(int));
    assertPuntatoreNonNull(distanze, "\nImpossibile allocare dinamicamente memoria.");

    // la distanza dal giocatore a sé stesso è convenzionalmente 0
    distanze[posizioneGiocatore] = 0;

    // calcolo la distanza da ogni giocatore
    for(i = 0; i < nGiocatori; i++) {
        // la distanza del giocatore da sé stesso è già stata impostata
        if(i != posizioneGiocatore)
            distanze[i] = calcolaDistanzaGiocatore(nGiocatori, giocatori, posizioneGiocatore, i);
    }

    // restituisco l'array con le distanze
    return distanze;
}

/**
 * Funzione che calcola la distanza tra due giocatori.
 * Si considera come "giocatore1" quello di cui si verifica l'effetto della carta "mirino",
 * mentre come "giocatore2" quello di cui si verifica la carta "mustang".
 * Il calcolo della distanza è effettuato rimuovendo dalla conta i giocatori che sono morti durante la partita.
 *
 * @param nGiocatori Numero di giocatori.
 * @param giocatori Array di giocatori.
 * @param posizioneGiocatore1 Posizione del primo giocatore.
 * @param posizioneGiocatore2 Posizione del secondo giocatore.
 * @return La distanza tra giocatore1 e giocatore2.
 */
int calcolaDistanzaGiocatore(int nGiocatori, Giocatore giocatori[nGiocatori], int posizioneGiocatore1, int posizioneGiocatore2) {
    int i;
    int distanza = 0;
    // la distanza viene calcolata una volta da sinistra a destra, poi da destra a sinistra
    int distanzaGiroOrario = 0, distanzaGiroAntiOrario = 0;

    i = posizioneGiocatore1;
    // calcolo distanza in senso orario
    do {
        i = (i + 1) % nGiocatori; // scorro l'array con il modulo per tornare all'inizio
        if(giocatori[i].puntiVita > 0) // filtro i giocatori morti
            distanzaGiroOrario++;
    } while (i != posizioneGiocatore2);

    // calcolo distanza in senso anti orario
    i = posizioneGiocatore1;
    do {
        i = (i - 1) % nGiocatori; // scorro l'array con il modulo per tornare all'inizio
        if(i < 0)
            i += nGiocatori; // se i è negativo, il risultato del modulo deve essere reso positivo
        if(giocatori[i].puntiVita > 0) // filtro i giocatori morti
            distanzaGiroAntiOrario++;
    } while (i != posizioneGiocatore2);

    // la distanza vera è la più piccola tra le due
    if(distanzaGiroAntiOrario < distanzaGiroOrario) {
        distanza = distanzaGiroAntiOrario;
    } else {
        distanza = distanzaGiroOrario;
    }

    // verifico che il giocatore1 possegga una carta mirino
    if(possiedeCartaInGioco(giocatori[posizioneGiocatore1], CARTA_MIRINO) && distanza > 1)
        distanza--;
    // verifico che il giocatore2 abbia un mustang
    if(possiedeCartaInGioco(giocatori[posizioneGiocatore2], CARTA_MUSTANG))
        distanza++;

    // restituisco la distanza calcolata
    return distanza;
}

/**
 * Subroutine che mostra all'utente la sua distanza dagli altri giocatori.
 *
 * @param nGiocatori Il numero di giocatori in partita.
 * @param posizioneGiocatore L'indice del giocatore da cui calcolare le distanze.
 * @param giocatori I giocatori in partita.
 */
void mostraDistanze(int nGiocatori, Giocatore giocatori[nGiocatori], int posizioneGiocatore) {
    int i;
    // array contenente le distanze da ogni giocatore
    int *distanze = NULL;

    // calcolo dell'array delle distanze
    distanze = calcolaDistanzeGiocatori(nGiocatori, giocatori, posizioneGiocatore);

    printf("\n%s DISTANZE %s", MEZZO_SEPARATORE, MEZZO_SEPARATORE);

    // itero sulle distanze calcolate
    for(i = 0; i < nGiocatori; i++) {
        if(i != posizioneGiocatore && giocatori[i].puntiVita > 0) { // non è necessario mostrare la posizione del giocatore stesso e dei morti
            // è necessario evidenziare lo sceriffo tra gli altri giocatori
            if(giocatori[i].ruoloAssegnato == SCERIFFO) {
                printf("\n*) Lo SCERIFFO, %s, dista %d posizioni.", giocatori[i].nomeUtente, distanze[i]);
            } else {
                printf("\n*) Il giocatore %s dista %d posizioni.", giocatori[i].nomeUtente, distanze[i]);
            }
        }
    }

    printf("\n%s DISTANZE %s", MEZZO_SEPARATORE, MEZZO_SEPARATORE);
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

    // itero su tutti i giocatori
    for(i = 0; i < nGiocatori; i++) {
        if(strcmp(giocatori[i].nomeUtente, giocatori[posizioneGiocatore].nomeUtente) != 0 && giocatori[i].puntiVita > 0) {
            printf("\n*) Carte in gioco di %s", giocatori[i].nomeUtente);
            mostraCarteMazzo(giocatori[i].carteGioco);
            printf("\n");
        }
    }
}

/**
 * Funzione che gioca la carta dalla mano di un giocatore.
 * La carta, dopo essere stata giocata, NON viene scartata dalla mano del giocatore. Invece,
 * la funzione restituisce "true" o "false", se la carta è stata giocata o meno.
 * Se un giocatore gioca una carta "in gioco" o "arma", questa viene spostata nel mazzo di carte in gioco.
 *
 * @param mazzoPesca Puntatore al mazzo delle carte di pesca.
 * @param mazzoScarti Puntatore al mazzo delle carte scartate.
 * @param nGiocatori Numero totale di giocatori.
 * @param giocatori Array contenente i giocatori.
 * @param posizioneGiocatore Posizione del giocatore che gioca la carta
 * @param posizioneCarta Posizione della carta giocata nel mazzo della mano.
 * @return True, se la carta viene effettivamente giocata, false altrimenti.
 */
bool giocaCarta(Mazzo *mazzoPesca, Mazzo *mazzoScarti, int nGiocatori, Giocatore giocatori[nGiocatori], int posizioneGiocatore, int posizioneCarta) {
    // variabile intera utilizzata nei cicli interni alla funzione
    int i;
    // puntatore al giocatore che sta giocando la carta al momento
    Giocatore* giocatore = &giocatori[posizioneGiocatore];
    // carta da giocare
    Carta carta = giocatori[posizioneGiocatore].carteMano.carte[posizioneCarta];
    // variabili di appoggio utilizzate nei cicli do-while interni per chiedere conferma delle scelte prese dal giocatore
    bool confermaCarta = false;
    char confermaAzione;

    // PRIMA PARTE: carte istantanee
    if (carta.tipologiaCarta == ISTANTANEA) {
        if (strcmp(carta.nomeCarta, CARTA_BANG) == 0) {
            // puntatore al giocatore bersagliato
            Giocatore* giocatoreScelto = NULL;
            // carta estratta nel caso in cui ci sia in gioco un barile
            Carta cartaEstratta;

            printf("\n%s DESCRIZIONE %s", MEZZO_SEPARATORE, MEZZO_SEPARATORE);
            printf("\nLa carta %s ti permette di infliggere danni a un giocatore.", CARTA_BANG);
            printf("\n%s DESCRIZIONE %s\n", MEZZO_SEPARATORE, MEZZO_SEPARATORE);

            printf("\nLa tua gittata attuale è: %d", giocatore->gittata);

            // chiedo all'utente a quale giocatore vorrebbe sparare, finché non conferma la sua scelta
            do {
                // chiedo di selezionare un giocatore, tenendo conto e mostrando le distanze
                giocatoreScelto = selezionaGiocatore(nGiocatori, giocatori, posizioneGiocatore, true, true);

                // se giocatoreScelto == NULL, significa che non ci sono giocatori raggiungibili con la gittata attuale
                if(giocatoreScelto != NULL) {
                    // chiedo all'utente di confermare la sua scelta. se rifiuta, gli chiedo se desidera cambiare giocatore
                    // oppure tornare al menu principale
                    do {
                        printf("\nStai per sparare a %s! Confermi la tua scelta?\n"
                               "%c/%c) ", giocatoreScelto->nomeUtente, PROMPT_CONFERMA, PROMPT_RIFIUTA);
                        scanf(" %c", &confermaAzione);

                        if (confermaAzione == PROMPT_RIFIUTA) {
                            printf("\nDesideri cambiare giocatore? Inserendo no, tornerai al menu principale.\n"
                                   "%c/%c) ", PROMPT_CONFERMA, PROMPT_RIFIUTA);
                            scanf(" %c", &confermaAzione);

                            if (confermaAzione == PROMPT_RIFIUTA) {
                                printf("\nTorno al menu principale!");
                                return false; // la carta 'bang!' non è stata giocata
                            } else if(confermaAzione == PROMPT_CONFERMA) {
                                confermaCarta = false; // chiedo all'utente di selezionare un altro giocatore
                            } else {
                                printf("\nInserisci un valore valido!");
                            }
                        } else if(confermaAzione == PROMPT_CONFERMA) {
                            confermaCarta = true; // l'utente ha confermato lo sparo
                        } else {
                            printf("\nInserisci un valore valido!");
                        }
                    } while (confermaAzione != PROMPT_CONFERMA && confermaAzione != PROMPT_RIFIUTA);
                } else {
                    return false; // la carta non può essere giocata
                }
            } while (!confermaCarta);

            // logging
            scriviCartaGiocataAvversarioSuLog(giocatore->nomeUtente, giocatoreScelto->nomeUtente, carta);

            svuotaBuffer();

            // verifica di eventuali azioni per evitare lo sparo
            // primo elemento: carta barile
            if (possiedeCartaInGioco(*giocatoreScelto, CARTA_BARILE)) {
                printf("\nIl giocatore %s possiede una carta '%s'. Sarà estratta una carta che, se sarà"
                       "del seme di Cuori, annullerà completamente l'attacco.", giocatoreScelto->nomeUtente, CARTA_BARILE);

                printf("\nPremi 'Invio' per estrarre una carta.");
                getchar();

                // estraggo una carta (che sarà spostata nel mazzo scarti) attraverso la funzione 'estraiCarta'.
                cartaEstratta = estraiCarta(mazzoPesca, mazzoScarti);
                // logging
                scriviBarileSuLog(giocatore->nomeUtente, cartaEstratta);
                // attacco mancato
                if(cartaEstratta.tipologiaCarta == CUORI) {
                    printf("\nLa carta estratta è di Cuori! L'attacco è mancato!");
                    return true; // la carta è stata comunque giocata
                } else {
                    printf("\nLa carta estratta non è una carta di Cuori!");
                }
            }

            // secondo elemento: carta mancato
            int cartaMancato = cercaCartaMazzoPerNome(giocatoreScelto->carteMano, CARTA_MANCATO);
            // il giocatore possiede un 'Mancato!'
            if (cartaMancato != -1) {
                svuotaSchermo();
                printf("\nIl giocatore %s possiede una carta '%s'! Se deciderà di giocarla, potrà evitare l'attacco.", giocatoreScelto->nomeUtente, CARTA_MANCATO);
                printf("\nPassa lo schermo a %s per permettergli di scegliere se giocare la carta.\n"
                       "Premi 'Invio' per continuare.", giocatoreScelto->nomeUtente);
                getchar();

                do {
                    printf("\n%s ha giocato una carta '%s' contro di te: desideri scartare una carta '%s' ed evitare l'attacco?\n"
                           "%c/%c) ", giocatore->nomeUtente, CARTA_BANG, CARTA_MANCATO, PROMPT_CONFERMA, PROMPT_RIFIUTA);
                    scanf(" %c", &confermaAzione);
                    if (confermaAzione != PROMPT_CONFERMA && confermaAzione != PROMPT_RIFIUTA)
                        printf("\nInserisci un valore valido!");
                } while (confermaAzione != PROMPT_CONFERMA && confermaAzione != PROMPT_RIFIUTA);

                // verifico la scelta del giocatore attaccato
                if(confermaAzione == PROMPT_CONFERMA) {
                    printf("\n%s ha giocato un '%s' ed evita l'attacco!", giocatoreScelto->nomeUtente, CARTA_MANCATO);

                    // scarto la carta 'Mancato!' appena giocata
                    spostaCartaMazzo(&giocatoreScelto->carteMano, mazzoScarti, cartaMancato);

                    // logging
                    scriviMancatoSuLog(giocatore->nomeUtente);

                    return true; // la carta 'Bang!' è stata comunque giocata, quindi termino la funzione
                } else {
                    printf("\n%s ha deciso di non giocare un '%s'!", giocatoreScelto->nomeUtente, CARTA_MANCATO);
                }
            }

            // termine della funzione: rimozione del punto vita e chiusura
            printf("\n%s ha sparato a %s, che perde quindi un punto vita!", giocatore->nomeUtente, giocatoreScelto->nomeUtente);
            rimuoviPuntiVita(giocatoreScelto, giocatore, 1, mazzoPesca, mazzoScarti);
        } else if (strcmp(carta.nomeCarta, CARTA_BIRRA) == 0) {
            printf("\n%s DESCRIZIONE %s", MEZZO_SEPARATORE, MEZZO_SEPARATORE);
            printf("\nLa carta '%s' ti permette di recuperare un punto vita!", CARTA_BIRRA);
            printf("\n%s DESCRIZIONE %s\n", MEZZO_SEPARATORE, MEZZO_SEPARATORE);

            // verifico che il giocatore non abbia già il massimo della vita
            if(giocatore->puntiVita != prendiVitaMassima(*giocatore)) {
                printf("\n%s ha utilizzato una '%s' e guadagna un punto vita!", giocatore->nomeUtente, CARTA_BIRRA);
                // logging
                scriviCartaGiocataSuLog(giocatore->nomeUtente, carta);
                // incremento i punti vita
                giocatori[posizioneGiocatore].puntiVita++;
            } else {
                printf("\nHai già il massimo della vita!");
                return false; // la carta non può essere giocata
            }
            svuotaBuffer();
        } else if (strcmp(carta.nomeCarta, CARTA_DILIGENZA) == 0) {
            printf("\n%s DESCRIZIONE %s", MEZZO_SEPARATORE, MEZZO_SEPARATORE);
            printf("\nLa carta '%s' ti permette di pescare 2 carte!", CARTA_DILIGENZA);
            printf("\n%s DESCRIZIONE %s\n", MEZZO_SEPARATORE, MEZZO_SEPARATORE);

            printf("\n%s pesca 2 carte!", giocatore->nomeUtente);
            // logging
            scriviCartaGiocataSuLog(giocatore->nomeUtente, carta);
            // faccio pescare le carte al giocatore
            pescaCarte(mazzoPesca, mazzoScarti, &giocatori[posizioneGiocatore], 2);
            svuotaBuffer();
        } else if (strcmp(carta.nomeCarta, CARTA_PANICO) == 0) {
            // posizione della carta del mazzo che si vuole rubare
            int posizioneCartaDaRubare = 0;
            // posizione del giocatore scelto
            int posizioneGiocatoreScelto = 0;
            // puntatore al giocatore scelto
            Giocatore* giocatoreScelto = NULL;
            // puntatore alla carta che si intende rubare
            Carta* cartaDaRubare = NULL;
            // carattere che rappresenta il mazzo scelto per rubare la carta
            char mazzoSceltoPrompt;
            // e il puntatore al mazzo effettivo
            Mazzo* mazzoScelto = NULL;
            // booleano usato nel ciclo per la conferma del mazzo. esso si ripete finché non la variabile è uguale a True
            bool confermaMazzo = false;


            printf("\n%s DESCRIZIONE %s", MEZZO_SEPARATORE, MEZZO_SEPARATORE);
            printf("\nLa carta '%s' ti permette di rubare una carta a un giocatore di distanza 1.", CARTA_PANICO);
            printf("\n%s DESCRIZIONE %s", MEZZO_SEPARATORE, MEZZO_SEPARATORE);

            // verifico che ci sia un giocatore a distanza 1
            if(esisteGiocatoreVicino(nGiocatori, giocatori, posizioneGiocatore, 1)) {
                // chiedo all'utente il nome del giocatore che intende bersagliare
                do {
                    do {
                        // chiedo al giocatore di sceglierne un altro, mostrando le distanze ma non prendendole in considerazione nella scelta
                        giocatoreScelto = selezionaGiocatore(nGiocatori, giocatori, posizioneGiocatore, true, false);

                        // valido la distanza
                        // calcolo della posizione del giocatore
                        posizioneGiocatoreScelto = 0;
                        while (strcmp(giocatoreScelto->nomeUtente, giocatori[posizioneGiocatoreScelto].nomeUtente) != 0) {
                            posizioneGiocatoreScelto++;
                        }

                        // verifico che la distanza sia corretta
                        if(calcolaDistanzaGiocatore(nGiocatori, giocatori, posizioneGiocatore, posizioneGiocatoreScelto) > 1) {
                            printf("\nQuesto giocatore è troppo lontano!");
                            giocatoreScelto = NULL; // annullo la scelta per far ripetere il ciclo
                        // verifico che l'utente scelto abbia effettivamente delle carte
                        } else if(giocatoreScelto->carteMano.numeroCarte < 1 && giocatoreScelto->carteGioco.numeroCarte < 1) {
                            printf("\nIl giocatore non ha nessuna carta!");
                            giocatoreScelto = NULL; // annullo la scelta per far ripetere il ciclo
                        }

                        // potrebbe essere che l'unico giocatore disponibile non abbia carte, quindi chiedo all'utente
                        // se preferisce sceglierne un altro o tornare al menu principale
                        if(giocatoreScelto == NULL) {
                            do {
                                printf("\nVuoi scegliere un altro giocatore?\n"
                                       "%c/%c) ", PROMPT_CONFERMA, PROMPT_RIFIUTA);
                                scanf(" %c", &confermaAzione);
                                if(confermaAzione == PROMPT_RIFIUTA) {
                                    printf("\nTorno al menu principale!");
                                    svuotaBuffer();
                                    return false; // la carta non è stata giocata
                                } else if(confermaAzione != PROMPT_CONFERMA && confermaAzione != PROMPT_RIFIUTA) {
                                    printf("\nInserisci un azione valida!");
                                }
                            } while (confermaAzione != PROMPT_CONFERMA && confermaAzione != PROMPT_RIFIUTA);
                        }
                    } while(giocatoreScelto == NULL);

                    // infine, chiedo all'utente di confermare la scelta
                    do {
                        printf("\nHai scelto di rubare una carta a %s. Confermi?\n"
                               "%c/%c) ", giocatoreScelto->nomeUtente, PROMPT_CONFERMA, PROMPT_RIFIUTA);
                        scanf(" %c", &confermaAzione);
                        if(confermaAzione == PROMPT_RIFIUTA) {
                            giocatoreScelto = NULL; // annullo la scelta del giocatore

                            // chiedo al giocatore se ha cambiato idea sulla carta da giocare
                            printf("\nScelta annullata! Desideri tornare al menu principale?\n"
                                   "%c/%c) ", PROMPT_CONFERMA, PROMPT_RIFIUTA);
                            scanf(" %c", &confermaAzione);
                            if(confermaAzione == PROMPT_CONFERMA) {
                                printf("\nTorno al menu principale!");
                                svuotaBuffer();
                                return false; // la carta non è stata giocata
                            } else if(confermaAzione == PROMPT_RIFIUTA) {
                                giocatoreScelto = NULL; // annullo la scelta precedente per permetterne un'altra
                            } else {
                                printf("\nInserisci un valore valido!");
                            }
                        }
                        if(confermaAzione != PROMPT_CONFERMA && confermaAzione != PROMPT_RIFIUTA) {
                            printf("\nInserisci un valore valido!");
                        }
                    } while (confermaAzione != PROMPT_CONFERMA && confermaAzione != PROMPT_RIFIUTA);

                    // se il giocatore è stato trovato, entro nel ciclo.
                    // dato che all'interno del ciclo il giocatore potrebbe cambiare idea e sceglierne un altro,
                    // chiudo il ciclo se si intraprende questa scelta
                    // inoltre, ripeto il ciclo finché non è stata trovata una carta da rubare
                    while (giocatoreScelto != NULL && cartaDaRubare == NULL) {
                        // chiedo all'utente la posizione della carta da rubare
                        do {
                            // chiedo all'utente da che mazzo intende rubare la carta
                            do {
                                printf("\nScegli il mazzo da cui rubare una carta!");
                                printf("\n%s CARTE MANO %s", MEZZO_SEPARATORE, MEZZO_SEPARATORE);
                                mostraCarteMazzo(giocatoreScelto->carteMano);
                                printf("\n\n%s CARTE GIOCO %s", MEZZO_SEPARATORE, MEZZO_SEPARATORE);
                                mostraCarteMazzo(giocatoreScelto->carteGioco);
                                printf("\nChe mazzo scegli? (%c) per il mazzo di mano, (%c) per quello di gioco.\n"
                                       "?) ", PROMPT_MAZZO_MANO, PROMPT_MAZZO_GIOCO);
                                scanf(" %c", &mazzoSceltoPrompt);

                                // verifico che il carattere inserito sia valido
                                if (mazzoSceltoPrompt != PROMPT_MAZZO_MANO && mazzoSceltoPrompt != PROMPT_MAZZO_GIOCO) {
                                    printf("\nInserisci un valore valido!");
                                // verifico che il mazzo scelto abbia delle carte
                                } else if ((mazzoSceltoPrompt == PROMPT_MAZZO_MANO && giocatoreScelto->carteMano.numeroCarte < 1) ||
                                           (mazzoSceltoPrompt == PROMPT_MAZZO_GIOCO && giocatoreScelto->carteGioco.numeroCarte < 1)) {
                                    printf("\nIl mazzo scelto è vuoto!");
                                } else {
                                    if(mazzoSceltoPrompt == PROMPT_MAZZO_MANO) {
                                        mazzoScelto = &giocatoreScelto->carteMano;
                                    } else {
                                        mazzoScelto = &giocatoreScelto->carteGioco;
                                    }
                                    confermaMazzo = true; // il mazzo è stato scelto in modo corretto
                                }
                            } while (!confermaMazzo);

                            // mostro le carte disponibili
                            printf("\nQueste sono le carte di %s:", giocatoreScelto->nomeUtente);
                            mostraCarteMazzo(*mazzoScelto);

                            printf("\nInserisci la posizione della carta da rubare!\n"
                                   "?) ");
                            scanf("%d", &posizioneCartaDaRubare);

                            // verifico che la posizione sia valida
                            if(posizioneCartaDaRubare <= 0 || posizioneCartaDaRubare > giocatoreScelto->carteMano.numeroCarte)
                                printf("\nInserisci una posizione valida!");
                        } while (posizioneCartaDaRubare <= 0 || posizioneCartaDaRubare > giocatoreScelto->carteMano.numeroCarte);
                        // salvo da parte la carta indicata dal giocatore
                        cartaDaRubare = &mazzoScelto->carte[posizioneCartaDaRubare - 1];

                        // mostro al giocatore la carta da rubare
                        printf("\nHai deciso di rubare questa carta!");
                        printf("\n%s CARTA %s\n", MEZZO_SEPARATORE, MEZZO_SEPARATORE);
                        mostraCarta(*cartaDaRubare);
                        printf("\n%s CARTA %s", MEZZO_SEPARATORE, MEZZO_SEPARATORE);

                        // chiedo al giocatore di confermare la scelta della carta
                        do {
                            printf("\nConfermi la tua scelta?\n"
                                   "%c/%c) ", PROMPT_CONFERMA, PROMPT_RIFIUTA);
                            scanf(" %c", &confermaAzione);

                            if(confermaAzione == PROMPT_RIFIUTA) {
                                // controllo che il giocatore voglia cambiare carta, cambiare giocatore, o altrimenti torno al menu principale
                                printf("\nDesideri cambiare carta?\n"
                                       "%c/%c) ", PROMPT_CONFERMA, PROMPT_RIFIUTA);
                                scanf(" %c", &confermaAzione);
                                // il giocatore potrebbe voler cambiare giocatore, o uscire
                                if(confermaAzione == PROMPT_RIFIUTA) {
                                    printf("\nDesideri cambiare giocatore? Inserendo no, tornerai al menu principale.\n"
                                           "%c/%c) ", PROMPT_CONFERMA, PROMPT_RIFIUTA);
                                    scanf(" %c", &confermaAzione);
                                    if(confermaAzione == PROMPT_RIFIUTA) {
                                        printf("\nTorno al menu principale!");
                                        svuotaBuffer();
                                        return false; // la carta non è stata giocata
                                    } else if(confermaAzione == PROMPT_CONFERMA) {
                                        printf("\nBene, scegli un altro giocatore!");
                                        giocatoreScelto = NULL; // resetto il giocatore scelto, così il ciclo si ripete
                                    } else {
                                        printf("\nInserisci un'azione valida!");
                                    }
                                } else if(confermaAzione == PROMPT_CONFERMA) {
                                    cartaDaRubare = NULL; // resetto la carta scelta, così il ciclo si ripete
                                } else {
                                    printf("\nInserisci un'azione valida!");
                                }
                            } else if(confermaAzione != PROMPT_CONFERMA && confermaAzione != PROMPT_RIFIUTA) {
                                printf("\nInserisci un'azione valida!");
                            }
                        } while (confermaAzione != PROMPT_CONFERMA && confermaAzione != PROMPT_RIFIUTA);
                    }
                } while(giocatoreScelto == NULL);

                // logging
                scriviCartaRubataPanicoSuLog(giocatore->nomeUtente, giocatoreScelto->nomeUtente,
                                             mazzoScelto->carte[posizioneCartaDaRubare - 1]);

                // la scelta è stata confermata, quindi rimuovo la carta dal mazzo da rubare e la aggiungo a quello del giocatore
                printf("\n%s ruba una carta '%s' a %s!", giocatore->nomeUtente, cartaDaRubare->nomeCarta, giocatoreScelto->nomeUtente);
                spostaCartaMazzo(mazzoScelto, &giocatore->carteMano, posizioneCartaDaRubare - 1);
                svuotaBuffer();
            } else {
                printf("\nNon esiste nessun giocatore a distanza uno!");
                svuotaBuffer();
                return false; // la carta non è stata giocata
            }
        } else if (strcmp(carta.nomeCarta, CARTA_CATBALOU) == 0) {
            // variabile contenente il carattere usato per scegliere il mazzo da cui scartare
            char mazzoScelto;
            // puntatore al giocatore a cui far scartare una carta
            Giocatore* giocatoreScelto = NULL;
            // booleano usato nel ciclo per la conferma del mazzo. esso si ripete finché non la variabile è uguale a True
            bool confermaMazzo = false;

            printf("\n%s DESCRIZIONE %s", MEZZO_SEPARATORE, MEZZO_SEPARATORE);
            printf("\nLa carta '%s' ti permette di far scartare una carta\n"
                   "a un giocatore a scelta, a prescindere dalla distanza.", CARTA_CATBALOU);
            printf("\n%s DESCRIZIONE %s\n", MEZZO_SEPARATORE, MEZZO_SEPARATORE);

            do {
                // mostro all'utente la lista dei giocatori in vita, senza considerare le distanze
                giocatoreScelto = selezionaGiocatore(nGiocatori, giocatori, posizioneGiocatore, false, false);

                // verifico che il giocatore abbia effettivamente delle carte
                if(giocatoreScelto->carteMano.numeroCarte > 0 || giocatoreScelto->carteGioco.numeroCarte > 0) {
                    // e quindi chiedo conferma
                    do {
                        printf("\nConfermi la tua di far scartare una carta a '%s'?\n"
                               "%c/%c) ", giocatoreScelto->nomeUtente, PROMPT_CONFERMA, PROMPT_RIFIUTA);
                        scanf(" %c", &confermaAzione);
                        if (confermaAzione == PROMPT_RIFIUTA) {
                            printf("\nDesideri scegliere un altro giocatore? Inserendo no, tornerai al menu principale\n"
                                   "%c/%c) ", PROMPT_CONFERMA, PROMPT_RIFIUTA);
                            scanf(" %c", &confermaAzione);
                            if (confermaAzione == PROMPT_RIFIUTA) {
                                printf("\nTorno al menu principale!");
                                svuotaBuffer();
                                return false;
                            } else {
                                printf("\nScegli un altro giocatore!");
                                giocatoreScelto = NULL; // annullo il giocatore scelto, così il ciclo si ripete
                            }
                        } else if(confermaAzione != PROMPT_RIFIUTA && confermaAzione != PROMPT_CONFERMA) {
                            printf("\nInserisci un'azione valida!");
                        }
                    } while (confermaAzione != PROMPT_RIFIUTA && confermaAzione != PROMPT_CONFERMA);
                } else {
                    // se l'if di sopra è falso, il giocatore non ha carte, quindi chiedo all'utente se vuole uscire
                    printf("\nIl giocatore scelto non ha alcuna carta!");

                    do {
                        printf("\nDesideri scegliere un altro giocatore?"
                               "%c/%c) ", PROMPT_CONFERMA, PROMPT_RIFIUTA);
                        scanf(" %c", &confermaAzione);
                        if(confermaAzione == PROMPT_RIFIUTA) {
                            printf("\nTorno al menu principale!");
                            svuotaBuffer();
                            return false;
                        } else if(confermaAzione == PROMPT_CONFERMA) {
                            giocatoreScelto = NULL; // annullo la scelta precedente per ripetere la richiesta del giocatore
                        } else {
                            printf("\nInserisci un'azione valida!");
                        }
                    } while (confermaAzione != PROMPT_CONFERMA && confermaAzione == PROMPT_RIFIUTA);
                }
            } while (giocatoreScelto == NULL);

            // logging
            scriviCartaGiocataSuLog(giocatore->nomeUtente, carta);

            // lo schermo deve passare al giocatore che deve scartare
            svuotaSchermo();
            printf("\n%s ha giocato un '%s' contro %s, che deve scartare una carta!", giocatore->nomeUtente, CARTA_CATBALOU,
                   giocatoreScelto->nomeUtente);
            printf("\nPassa lo schermo a %s per permettergli di scegliere quale carta scartare.\n"
                   "Premi 'Invio' appena sei pronto.", giocatoreScelto->nomeUtente);
            svuotaBuffer();
            getchar();

            do {
                // chiedo all'utente se vuole scartare dal mazzo della mano o da quello con le carte in gioco
                printf("\nScegli se scartare dal mazzo di mano o da quello delle carte in gioco!");
                printf("\n%s CARTE MANO %s", MEZZO_SEPARATORE, MEZZO_SEPARATORE);
                mostraCarteMazzo(giocatoreScelto->carteMano);
                printf("\n\n%s CARTE GIOCO %s", MEZZO_SEPARATORE, MEZZO_SEPARATORE);
                mostraCarteMazzo(giocatoreScelto->carteGioco);
                printf("\nChe mazzo scegli? (%c) per il mazzo di mano, (%c) per quello di gioco.\n"
                       "?) ", PROMPT_MAZZO_MANO, PROMPT_MAZZO_GIOCO);
                scanf(" %c", &mazzoScelto);

                if (mazzoScelto != PROMPT_MAZZO_MANO && mazzoScelto != PROMPT_MAZZO_GIOCO) {
                    printf("\nInserisci un valore valido!");
                // verifico che il mazzo scelto abbia delle carte
                } else if (mazzoScelto == PROMPT_MAZZO_MANO && giocatoreScelto->carteMano.numeroCarte < 1 ||
                           mazzoScelto == PROMPT_MAZZO_GIOCO && giocatoreScelto->carteGioco.numeroCarte < 1) {
                    printf("\nIl mazzo scelto è vuoto!");
                } else {
                    confermaMazzo = true; // il mazzo è stato scelto in modo corretto
                }
            } while (!confermaMazzo);

            // scarto la carta e svuoto lo schermo
            if(mazzoScelto == PROMPT_MAZZO_MANO) {
                scartaCarta(giocatoreScelto, &giocatoreScelto->carteMano, mazzoScarti);
            } else if(mazzoScelto == PROMPT_MAZZO_GIOCO) {
                scartaCarta(giocatoreScelto, &giocatoreScelto->carteGioco, mazzoScarti);
            }

            svuotaSchermo();
            printf("\n%s ha scartato una carta per effetto di un '%s'!", giocatoreScelto->nomeUtente, CARTA_CATBALOU);
            svuotaBuffer();

        } else if (strcmp(carta.nomeCarta, CARTA_MANCATO) == 0) {
            printf("\n%s DESCRIZIONE %s", MEZZO_SEPARATORE, MEZZO_SEPARATORE);
            printf("\nLa carta '%s' ti permette di schivare un '%s'.", CARTA_MANCATO, CARTA_BANG);
            printf("\n%s DESCRIZIONE %s\n", MEZZO_SEPARATORE, MEZZO_SEPARATORE);

            printf("\nNon puoi giocare questa carta direttamente, ma solo quando stai subendo uno sparo!");
            svuotaBuffer();
            return false; // questa carta non può essere giocata dalla mano
        } else if (strcmp(carta.nomeCarta, CARTA_GATLING) == 0) {
            printf("\n%s DESCRIZIONE %s", MEZZO_SEPARATORE, MEZZO_SEPARATORE);
            printf("\nLa carta '%s' ti permette di rimuovere a tutti i giocatori un punto vita.", CARTA_GATLING);
            printf("\n%s DESCRIZIONE %s\n", MEZZO_SEPARATORE, MEZZO_SEPARATORE);

            printf("\n%s ha giocato un '%s': tutti i giocatori perdono un punto vita!", giocatore->nomeUtente, CARTA_GATLING);
            // logging
            scriviCartaGiocataSuLog(giocatore->nomeUtente, carta);

            // itero sui giocatori per togliere un punto vita
            for (i = 0; i < nGiocatori; i++) {
                if (i != posizioneGiocatore && giocatori[i].puntiVita > 0) { // filtro i giocatori morti e l'attaccante
                    rimuoviPuntiVita(&giocatori[i], giocatore, 1, mazzoPesca, mazzoScarti);
                }
            }
            svuotaBuffer();
        } else if (strcmp(carta.nomeCarta, CARTA_SALOON) == 0) {
            printf("\n%s DESCRIZIONE %s", MEZZO_SEPARATORE, MEZZO_SEPARATORE);
            printf("\nLa carta '%s' ti permette di aggiungere a ogni giocatore un punto vita.", CARTA_SALOON);
            printf("\n%s DESCRIZIONE %s\n", MEZZO_SEPARATORE, MEZZO_SEPARATORE);

            printf("\n%s ha giocato un '%s': tutti i giocatori guadagnano un punto vita!", giocatore->nomeUtente, CARTA_SALOON);
            // logging
            scriviCartaGiocataSuLog(giocatore->nomeUtente, carta);

            for (i = 0; i < nGiocatori; i++) {
                // filtro i giocatori morti e quelli che hanno già vita massima
                if (giocatori[i].puntiVita > 0 && giocatori[i].puntiVita < prendiVitaMassima(giocatori[i]))
                    giocatori[i].puntiVita++;
            }
            svuotaBuffer();
        } else if (strcmp(carta.nomeCarta, CARTA_WELLSFARGO) == 0) {
            printf("\n%s DESCRIZIONE %s", MEZZO_SEPARATORE, MEZZO_SEPARATORE);
            printf("\nLa carta '%s' ti permette di pescare 3 carte.", CARTA_WELLSFARGO);
            printf("\n%s DESCRIZIONE %s\n", MEZZO_SEPARATORE, MEZZO_SEPARATORE);

            printf("\n%s pesca 3 carte!", giocatore->nomeUtente);
            // logging
            scriviCartaGiocataSuLog(giocatore->nomeUtente, carta);
            pescaCarte(mazzoPesca, mazzoScarti, &giocatori[posizioneGiocatore], 3);
            svuotaBuffer();
        }
    // SECONDA PARTE: carte istantanee speciali
    } else if (carta.tipologiaCarta == ISTANTANEA_SPECIAL) {
        if (strcmp(carta.nomeCarta, CARTA_DUELLO) == 0) {
            // variabile di appoggio che contiene la posizione della carta bang da scartare a ogni turno del duello
            // se la carta non esiste, il valore assunto è '-1'
            int posizioneCartaBang;
            // e il puntatore al giocatore
            Giocatore* giocatoreScelto = NULL;
            // puntatori al, rispettivamente, vincitore e perdente del duello
            Giocatore* vincitoreDuello = NULL, *perdenteDuello = NULL;
            // variabile di appoggio che contiene il giocatore che deve giocare il turno corrente del duello
            Giocatore* duellanteTurnoCorrente;

            printf("\n%s DESCRIZIONE %s", MEZZO_SEPARATORE, MEZZO_SEPARATORE);
            printf("\nLa carta '%s' ti permette di sfidare qualsiasi giocatore ad un duello.\n"
                   "Ad ogni round, viene estratta una carta '%s' del partecipante al turno corrente:\n"
                   "il primo che non ne possiede una, perde 3 punti vita!", CARTA_DUELLO, CARTA_BANG);
            printf("\n%s DESCRIZIONE %s", MEZZO_SEPARATORE, MEZZO_SEPARATORE);


            do {
                // chiedo all'utente di selezionare un giocatore, senza considerare le distanze
                giocatoreScelto = selezionaGiocatore(nGiocatori, giocatori, posizioneGiocatore, false, false);

                // chiedo di confermare la scelta
                do {
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
                            svuotaBuffer();
                            return false; // la carta non è stata giocata
                        } else if(confermaAzione != PROMPT_RIFIUTA && confermaAzione != PROMPT_CONFERMA) {
                            printf("\nInserisci un valore valido!");
                        }
                    } else if(confermaAzione != PROMPT_RIFIUTA && confermaAzione != PROMPT_CONFERMA) {
                        printf("\nInserisci un valore valido!");
                    }
                } while (confermaAzione != PROMPT_CONFERMA && confermaAzione != PROMPT_RIFIUTA);
            } while (giocatoreScelto == NULL);

            // logging
            scriviCartaGiocataSuLog(giocatore->nomeUtente, carta);

            // il giocatore è confermato, e il duello può avere inizio
            printf("\nSi dia inizio al duello tra %s e %s!", giocatore->nomeUtente, giocatoreScelto->nomeUtente);
            printf("\nEstrarrò una carta '%s' una volta a testa per ciascun giocatore. Il primo a finirle perderà un punto vita!", CARTA_BANG);
            printf("\nIl primo ad iniziare sarà il giocatore sfidato!");
            duellanteTurnoCorrente = giocatoreScelto;

            // svuoto il buffer
            svuotaBuffer();
            do {
                printf("\nE' il turno di %s!", duellanteTurnoCorrente->nomeUtente);
                printf("\nPremi invio per continuare.");
                getchar();

                // cerco la carta 'Bang!' nel mazzo del duellante
                posizioneCartaBang = cercaCartaMazzoPerNome(duellanteTurnoCorrente->carteMano, CARTA_BANG);

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
                    // logging
                    scriviCartaScartataSuLog(duellanteTurnoCorrente->nomeUtente, duellanteTurnoCorrente->carteMano.carte[posizioneCartaBang]);
                    // rimuovo la carta dalla mano e la inserisco nel mazzo scarti
                    spostaCartaMazzo(&duellanteTurnoCorrente->carteMano, mazzoScarti, posizioneCartaBang);
                    printf("\n%s scarta una carta '%s' e supera il turno!", duellanteTurnoCorrente->nomeUtente, CARTA_BANG);

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
            printf("\n%s ha esaurito le carte '%s', quindi %s vince il duello!", perdenteDuello->nomeUtente, CARTA_BANG, vincitoreDuello->nomeUtente);
            printf("\n%s FINE DUELLO %s\n", MEZZO_SEPARATORE, MEZZO_SEPARATORE);

            // rimuovo i punti vita dello sconfitto
            rimuoviPuntiVita(perdenteDuello, vincitoreDuello, 3, mazzoPesca, mazzoScarti);

        } else if (strcmp(carta.nomeCarta, CARTA_EMPORIO) == 0) {
            // variabile di appoggio per i cicli interni
            int j = 0;
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

            printf("\n%s DESCRIZIONE %s", MEZZO_SEPARATORE, MEZZO_SEPARATORE);
            printf("\nLa carta '%s' estrae dal mazzo di pesca un numero di carte pari a quello dei giocatori in vita,\n"
                   "per poi permettere ad ogni utente (a partire da chi ha giocato la carta) di pescare una carta da questo mazzo.", CARTA_EMPORIO);
            printf("\n%s DESCRIZIONE %s\n", MEZZO_SEPARATORE, MEZZO_SEPARATORE);

            // logging
            scriviCartaGiocataSuLog(giocatore->nomeUtente, carta);

            // calcolo dei giocatori in vita
            for (i = 0; i < nGiocatori; i++) {
                if (giocatori[i].puntiVita > 0)
                    nCarteRivelate++;
            }

            // allocazione dinamica dell'array di carte rivelate
            carteRivelate = (Carta*) calloc(nCarteRivelate, sizeof(Carta));
            assertPuntatoreNonNull(carteRivelate, "\nErrore: impossibile allocare dinamicamente l'array di carte da rivelare.");

            printf("\nEssendoci %d giocatori in vita, saranno rivelate %d carte.", nCarteRivelate, nCarteRivelate);

            // le carte del mazzo di pesca non sono sufficienti
            if(nCarteRivelate > mazzoPesca->numeroCarte) {
                swapRimanenti = nCarteRivelate - mazzoPesca->numeroCarte;
                // estraggo prima le carte rimanenti dal mazzo, poi lo scambio con quello di pesca e prendo le ultime
                for(i = 0; i < mazzoPesca->numeroCarte; i++) {
                    // sposto la carta nell'array di quelle rivelate
                    carteRivelate[i] = mazzoPesca->carte[mazzoPesca->numeroCarte - i - 1];
                    rimuoviCartaMazzo(mazzoPesca, mazzoPesca->numeroCarte - i - 1);
                }
                // swappo i mazzi
                scambiaPescaScarti(mazzoPesca, mazzoScarti);

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
                svuotaBuffer();
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
                // logging
                scriviCartePescateSuLog(giocatore->nomeUtente, giocatore->carteMano, 1);

                // rimozione della carta dal mazzetto pescato
                // questa azione viene effettuata "shiftando" di una posizione a sinistra tutte le carte a destra di quella scelta
                for (i = cartaScelta; i < nCarteRivelate; i++) {
                    carteRivelate[i - 1] = carteRivelate[i];
                }

                // è stata rivelata una carta
                nCarteRivelate--;
                // rialloco l'array al termine dell'operazione
                if(nCarteRivelate > 0) { // primo caso: ci sono altre carte che devono essere pescate
                    carteRivelate = (Carta*) realloc(carteRivelate, nCarteRivelate * sizeof(Carta));
                    assertPuntatoreNonNull(carteRivelate, "\nErrore: impossibile riallocare l'array di carte rivelate.");
                } else if(nCarteRivelate == 0) { // secondo caso: le carte sono terminate, posso liberare l'array
                    free(carteRivelate);
                } else { // terzo caso: numero negativo, errore
                    printf("\nErrore: il numero di carte rivelate è negativo.");
                    exit(-1);
                }

                // uso il modulo per tornare all'inizio nel caso in cui dovessi aver raggiunto la fine dell'array giocatori
                do {
                    j = (j + 1) % nGiocatori;
                } while (giocatori[j].puntiVita < 1);
            }

            printf("\nEstrazione terminata!");
            svuotaBuffer();

        } else if (strcmp(carta.nomeCarta, CARTA_INDIANI) == 0) {
            // variabile d'appoggio che contiene la posizione della carta 'Bang!' di ogni giocatore.
            // vale '-1' se il giocatore non possiede la carta
            int cartaBang;

            printf("\n%s DESCRIZIONE %s", MEZZO_SEPARATORE, MEZZO_SEPARATORE);
            printf("\nAttacco di indiani in arrivo! Ogni giocatore (eccetto chi ha giocato la carta)\n"
                   "deve giocare una carta '%s', oppure perdere un punto vita!", CARTA_BANG);
            printf("\n%s DESCRIZIONE %s\n", MEZZO_SEPARATORE, MEZZO_SEPARATORE);

            // logging
            scriviCartaGiocataSuLog(giocatore->nomeUtente, carta);

            // pulisco il buffer
            svuotaBuffer();
            for (i = 0; i < nGiocatori; i++) {
                // il giocatore deve essere in vita, e chi ha giocato la carta non viene attaccato
                if (i != posizioneGiocatore && giocatori[i].puntiVita > 0) {
                    cartaBang = cercaCartaMazzoPerNome(giocatori[i].carteMano, CARTA_BANG);
                    printf("\nTocca a %s! Premi 'Invio' quando sei pronto.", giocatori[i].nomeUtente);
                    getchar(); // aspetto che venga premuto invio

                    if (cartaBang == -1) {
                        printf("\n%s non possiede una carta '%s', e perde quindi un punto vita!", giocatori[i].nomeUtente, CARTA_BANG);
                        rimuoviPuntiVita(&giocatori[i], NULL, 1, mazzoPesca, mazzoScarti);
                    } else {
                        printf("\n%s scarta una carta '%s' ed evita l'attacco degli indiani!", giocatori[i].nomeUtente, CARTA_BANG);

                        // logging
                        scriviCartaScartataSuLog(giocatori[i].nomeUtente, giocatori[i].carteMano.carte[cartaBang]);

                        // scarto la carta 'Bang!' giocata
                        spostaCartaMazzo(&giocatori[i].carteMano, mazzoScarti, cartaBang);
                    }
                }
            }
        }
        // TERZA PARTE: armi
    } else if (carta.tipologiaCarta == ARMA) {
        // intero contenente, se esiste, l'arma già in gioco
        int posizioneArmaInUso;
        // puntatore all'arma attualmente in uso da un giocatore
        Carta armaInUso = prendiArmaGiocatore(*giocatore);

        if(strcmp(armaInUso.nomeCarta, CARTA_COLT45) != 0) {
            // se il giocatore non ha in gioco una colt 45 (arma di default), chiedo di confermare
            do {
                printf("Hai già un'arma equipaggiata: %s. Sei sicuro di voler cambiare arma?\n"
                       "%c/%c) ", armaInUso.nomeCarta, PROMPT_CONFERMA, PROMPT_RIFIUTA);
                scanf(" %c", &confermaAzione);
                if(confermaAzione == PROMPT_RIFIUTA) {
                    svuotaBuffer();
                    return false; // il giocatore ha cambiato idea
                } else if(confermaAzione != PROMPT_RIFIUTA && confermaAzione != PROMPT_CONFERMA) {
                    printf("\nInserisci un valore valido!");
                }
            } while (confermaAzione != PROMPT_RIFIUTA && confermaAzione != PROMPT_CONFERMA);

            // calcolo la posizione dell'arma in uso
            posizioneArmaInUso = cercaCartaMazzoPerNome(giocatore->carteGioco, armaInUso.nomeCarta);

            scriviCartaScartataSuLog(giocatore->nomeUtente, armaInUso);
            // sposto la vecchia arma nel mazzo di scarti se questa non era una colt. 45
            spostaCartaMazzo(&giocatore->carteGioco, mazzoScarti, posizioneArmaInUso);
        }
        // se l'arma è una colt, la sovrascrivo direttamente, non essendoci carte in gioco da scartare
        // difatti, la funzione "prendiArmaGiocatore" restituisce una colt se non ci sono armi, che però
        // non si trova effettivamente nel mazzo delle carte in gioco.
        // aggiungo la nuova carta
        aggiungiCartaInGioco(giocatore, giocatore, posizioneCarta);

        if (strcmp(carta.nomeCarta, CARTA_CARABINA) == 0) {
            printf("\nHai equipaggiato una '%s' (%d gittata)", CARTA_CARABINA, GITTATA_CARABINA);
        } else if (strcmp(carta.nomeCarta, CARTA_REMINGTON) == 0) {
            printf("\nHai equipaggiato una '%s' (%d gittata)", CARTA_REMINGTON, GITTATA_REMINGTON);
        } else if (strcmp(carta.nomeCarta, CARTA_SCHOFIELD) == 0) {
            printf("\nHai equipaggiato una '%s'' (%d gittata)", CARTA_SCHOFIELD, GITTATA_SCHOFIELD);
        } else if (strcmp(carta.nomeCarta, CARTA_WINCHESTER) == 0) {
            printf("\nHai equipaggiato una '%s' (%d gittata)", CARTA_WINCHESTER, GITTATA_WINCHESTER);
        } else if (strcmp(carta.nomeCarta, CARTA_VOLCANIC) == 0) {
            printf("\nHai equipaggiato una '%s' (%d gittata)", CARTA_VOLCANIC, GITTATA_VOLCANIC);
        }

        // ricalcolo della gittata del giocatore, dato che è stata cambiata arma
        giocatore->gittata = calcolaGittata(*giocatore);

        // logging
        scriviCartaGiocataSuLog(giocatore->nomeUtente, carta);
        svuotaBuffer();
    // QUARTA PARTE: carte in gioco
    } else if (carta.tipologiaCarta == EFFETTO) {
        // la carta prigione viene verificata come prima, perché le altre sono tutte carte che sono giocate e inserite
        // nel proprio mazzo di gioco
        if (strcmp(carta.nomeCarta, CARTA_PRIGIONE) == 0) {
            // puntatore al giocatore da mettere in prigione
            Giocatore *giocatoreScelto = NULL;

            printf("\n%s DESCRIZIONE %s", MEZZO_SEPARATORE, MEZZO_SEPARATORE);
            printf("\nLa carta '%s' ti permette di mettere in prigione\n"
                   "un giocatore qualsiasi, tranne lo Sceriffo. Un giocatore in prigione,\n"
                   "prima di giocare il suo turno, estrae una carta:\n"
                   "se questa non ha il seme di Cuori, allora\n"
                   "la carta prigione è scartata e il giocatore salta il turno\n"
                   "(pur rimanendo bersaglio degli altri giocatori), altrimenti,\n"
                   "la prigione è scartata e il turno è giocabile normalmente.", CARTA_PRIGIONE);
            printf("\n%s DESCRIZIONE %s\n", MEZZO_SEPARATORE, MEZZO_SEPARATORE);

            do {
                // chiedo all'utente di inserire un nome
                do {
                    // seleziono un giocatore senza considerare le distanze
                    giocatoreScelto = selezionaGiocatore(nGiocatori, giocatori, posizioneGiocatore, false, false);

                    // verifico che l'utente non sia lo sceriffo
                    if(giocatoreScelto->ruoloAssegnato == SCERIFFO) {
                        printf("\nNon puoi mettere in prigione lo sceriffo!");
                        giocatoreScelto = NULL; // annullo la scelta
                        // verifico che l'utente non sia già in prigione
                    } else if(possiedeCartaInGioco(*giocatoreScelto, CARTA_PRIGIONE)) {
                        printf("\nIl giocatore è già in prigione.");
                        giocatoreScelto = NULL; // annullo la scelta
                    }

                    // chiedo all'utente se vuole inserire un altro giocatore
                    // potrebbe essere che l'unico giocatore disponibile sia già in cella
                    if(giocatoreScelto == NULL) {
                        do {
                            printf("\nVuoi scegliere un altro giocatore? Inserendo no, tornerai al menu principale.\n"
                                   "%c/%c) ", PROMPT_CONFERMA, PROMPT_RIFIUTA);
                            scanf(" %c", &confermaAzione);
                            if(confermaAzione != PROMPT_CONFERMA && confermaAzione != PROMPT_RIFIUTA)
                                printf("\nInserisci un'azione valida!");
                        } while (confermaAzione != PROMPT_CONFERMA && confermaAzione != PROMPT_RIFIUTA);
                        if(confermaAzione == PROMPT_RIFIUTA) {
                            printf("\nTorno al menu principale!");
                            svuotaBuffer();
                            return false;
                        }
                    }
                } while (giocatoreScelto == NULL); // in questo modo il ciclo si ripete finché un giocatore non è trovato

                // chiedo al giocatore di confermare la scelta
                do {
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
                        } else if(confermaAzione == PROMPT_CONFERMA) {
                            printf("\nScegli un altro giocatore!");
                            giocatoreScelto = NULL; // annullo il giocatore scelto, così il ciclo si ripete
                        } else {
                            printf("\nInserisci un valore valido!");
                        }
                    } else if(confermaAzione != PROMPT_RIFIUTA && confermaAzione != PROMPT_CONFERMA) {
                        printf("\nInserisci un valore valido!");
                    }
                } while (confermaAzione != PROMPT_CONFERMA && confermaAzione != PROMPT_RIFIUTA);
            } while (giocatoreScelto == NULL);

            // aggiungo la prigione tra le carte in gioco del giocatore bersaglio
            aggiungiCartaInGioco(giocatore, giocatoreScelto, posizioneCarta);
            printf("\n%s ha messo in prigione %s!", giocatore->nomeUtente, giocatoreScelto->nomeUtente);
            // logging
            scriviCartaGiocataAvversarioSuLog(giocatore->nomeUtente, giocatoreScelto->nomeUtente, carta);
            svuotaBuffer();
        } else { // qui invece verifico tutte le altre carte possibili
            // verifico che la carta scelta non sia già in gioco
            if(!possiedeCartaInGioco(*giocatore, carta.nomeCarta)) {
                if (strcmp(carta.nomeCarta, CARTA_BARILE) == 0) {
                    printf("\n%s DESCRIZIONE %s", MEZZO_SEPARATORE, MEZZO_SEPARATORE);
                    printf("\nLa carta '%s' ti consente, fintanto che è in gioco, di schivare una carta '%s'.\n"
                           "Quando vieni colpito, puoi estrarre una carta: se questa è di Cuori, allora l'attacco è annullato.", CARTA_BARILE, CARTA_BANG);
                    printf("\n%s DESCRIZIONE %s\n", MEZZO_SEPARATORE, MEZZO_SEPARATORE);

                    aggiungiCartaInGioco(giocatore, giocatore, posizioneCarta);
                    printf("\nHai giocato una carta '%s'!", CARTA_BARILE);
                    // logging
                    scriviCartaGiocataSuLog(giocatore->nomeUtente, carta);
                    svuotaBuffer();
                } else if (strcmp(carta.nomeCarta, CARTA_MIRINO) == 0) {
                    printf("\n%s DESCRIZIONE %s", MEZZO_SEPARATORE, MEZZO_SEPARATORE);
                    printf("\nLa carta '%s' ti consente, fintanto che è in gioco,\n"
                           " di vedere tutti gli altri giocatori a una distanza diminuita di uno.", CARTA_MIRINO);
                    printf("\n%s DESCRIZIONE %s\n", MEZZO_SEPARATORE, MEZZO_SEPARATORE);

                    aggiungiCartaInGioco(giocatore, giocatore, posizioneCarta);
                    printf("\nHai equipaggiato la carta '%s'!", CARTA_MIRINO);
                    // logging
                    scriviCartaGiocataSuLog(giocatore->nomeUtente, carta);
                    svuotaBuffer();
                } else if (strcmp(carta.nomeCarta, CARTA_MUSTANG) == 0) {
                    printf("\n%s DESCRIZIONE %s", MEZZO_SEPARATORE, MEZZO_SEPARATORE);
                    printf("\nLa carta '%s' ti consente, fintanto che è in gioco,\n"
                           "di essere visto a distanza aumentata di uno dagli altri giocatori.", CARTA_MUSTANG);
                    printf("\n%s DESCRIZIONE %s\n", MEZZO_SEPARATORE, MEZZO_SEPARATORE);

                    aggiungiCartaInGioco(giocatore, giocatore, posizioneCarta);
                    printf("\nHai equipaggiato la carta '%s'!", CARTA_MUSTANG);
                    // logging
                    scriviCartaGiocataSuLog(giocatore->nomeUtente, carta);
                    svuotaBuffer();
                } else if (strcmp(carta.nomeCarta, CARTA_DINAMITE) == 0) {
                    printf("\n%s DESCRIZIONE %s", MEZZO_SEPARATORE, MEZZO_SEPARATORE);
                    printf("\nLa carta '%s' resta innocua per un intero giro.\n"
                           "Al tuo prossimo turno, prima di iniziare devi estrarre una carta che,\n "
                           "se sarà di un numero compreso tra 2 e 9 e del seme di Picche,\n"
                           "farà esplodere la dinamite, facendoti perdere 3 punti vita.\n"
                           "Se, invece, la dinamite non esplode, allora sarà passata\n"
                           "al giocatore successivo, che effettuerà le stesse azioni,\n"
                           "finché la dinamite non esplode o viene scartata.", CARTA_DINAMITE);
                    printf("\n%s DESCRIZIONE %s\n", MEZZO_SEPARATORE, MEZZO_SEPARATORE);

                    aggiungiCartaInGioco(giocatore, giocatore, posizioneCarta);
                    printf("\nHai messo un gioco una '%s'!\n", CARTA_DINAMITE);
                    // logging
                    scriviCartaGiocataSuLog(giocatore->nomeUtente, carta);
                    svuotaBuffer();
                }
            } else {
                printf("\nHai già una carta '%s' in gioco!", carta.nomeCarta);
                svuotaBuffer();
                return false;
            }
        }
    }
    return true; // la carta è stata giocata correttamente
}

/**
 * Subroutine che rimuove una carta dalla mano di un giocatore, per inserirla nel mazzo delle carte in gioco di
 * un altro giocatore (o anche del giocatore stesso).
 *
 * @param giocatoreCarta Puntatore al giocatore dalla cui mano sarà selezionata la posizioneCarta.
 * @param giocatoreBersaglio Puntatore al giocatore a cui sarà data la posizioneCarta.
 * @param posizioneCarta La posizione della posizioneCarta da giocare.
 */
void aggiungiCartaInGioco(Giocatore* giocatoreCarta, Giocatore* giocatoreBersaglio, int posizioneCarta) {
    spostaCartaMazzo(&giocatoreCarta->carteMano, &giocatoreBersaglio->carteGioco, posizioneCarta);
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
    // primo caso: il mazzo di pesca non contiene abbastanza carte, quindi bisogna scambiarlo con quello di scarti
    if(nCarte > mazzoPesca->numeroCarte) {
        // pesco le ultime carte rimanenti nel mazzo di pesca
        carteRimanenti = nCarte - mazzoPesca->numeroCarte;
        for(i = 0; i < mazzoPesca->numeroCarte; i++) {
            spostaCartaMazzo(mazzoPesca, &giocatore->carteMano, mazzoPesca->numeroCarte - i - 1);
        }

        // scambio il mazzo di pesca e quello degli scarti
        scambiaPescaScarti(mazzoPesca, mazzoScarti);

        // pesco le carte rimanenti
        for(i = 0; i < carteRimanenti; i++) {
            spostaCartaMazzo(mazzoPesca, &giocatore->carteMano, mazzoPesca->numeroCarte - i - 1);
        }
    } else { // secondo caso: pesco le carte normalmente
        for(i = 0; i < nCarte; i++) {
            spostaCartaMazzo(mazzoPesca, &giocatore->carteMano, mazzoPesca->numeroCarte - i - 1);
        }
    }

    // logging
    scriviCartePescateSuLog(giocatore->nomeUtente, giocatore->carteMano, nCarte);
}

/**
 * Subroutine che richiede al giocatore una carta da scartare dalla sua mano e la sposta nel mazzo degli scarti.
 *
 * @param giocatore Puntatore al giocatore che deve scartare.
 * @param mazzoDaScartare Puntatore al mazzo da cui scartare la carta.
 * @param mazzoScarti Puntatore al mazzo degli scarti.
 */
void scartaCarta(Giocatore *giocatore, Mazzo *mazzoDaScartare, Mazzo *mazzoScarti) {
    // indice contenente la carta scelta (che è però va da 1 a n)
    int cartaScelta;

    // verifico che il mazzo non sia vuoto
    if(mazzoDaScartare->numeroCarte > 0) {
        do {
            // mostro al giocatore le carte nella sua mano
            mostraCarteMazzo(*mazzoDaScartare);
            printf("\nScegli una carta da scartare:\n"
                   "?) ");
            scanf("%d", &cartaScelta);
            // verifico la correttezza dell'input
            if(cartaScelta <= 0 || cartaScelta > mazzoDaScartare->numeroCarte)
                printf("\nInserisci un numero tra %d e %d!\n", 1, mazzoDaScartare->numeroCarte);
        } while(cartaScelta <= 0 || cartaScelta > mazzoDaScartare->numeroCarte);

        // logging
        scriviCartaScartataSuLog(giocatore->nomeUtente, mazzoDaScartare->carte[cartaScelta - 1]);

        // scarto la carta
        spostaCartaMazzo(mazzoDaScartare, mazzoScarti, cartaScelta - 1);
    } else {
        printf("\nIl mazzo è vuoto!\n");
    }
}

/**
 * Funzione che restituisce l'arma attualmente in uso da un giocatore.
 *
 * @param giocatore Il giocatore di cui ottenere l'arma.
 * @return Arma usata dal giocatore. Se il giocatore non ha un'arma, restituisce una Colt 45 generata sul momento.
 */
Carta prendiArmaGiocatore(Giocatore giocatore) {
    int i;
    // valori della Colt. 45 da generare sul momento.
    int numeroColt;
    semiCarta semeColt;

    // scorro tutte le carte in gioco, finché non trovo un'arma.
    for(i = 0; i < giocatore.carteGioco.numeroCarte; i++) {
        if(giocatore.carteGioco.carte[i].tipologiaCarta == ARMA)
            return giocatore.carteGioco.carte[i];
    }

    // se la funzione non è terminata, il giocatore non ha un'arma in gioco, quindi restituisco una COLT 45 con valori statici
    // Non essendo il seme e il numero della colt rilevanti per il gioco, possono essere impostati in maniera statica,
    // anche perché la colt non va mai a finire né nel mazzo di pesca né in quello di scarti.
    return (Carta) {CARTA_COLT45, ARMA, 1, CUORI};
}

/**
 * Funzione che restituisce la vita massima di un giocatore in base al suo ruolo.
 *
 * @param giocatore Il giocatore di cui si vuole conoscere la vita massima.
 * @return Il numero massimo di punti vita di un giocatore.
 */
int prendiVitaMassima(Giocatore giocatore) {
    // determino il numero massimo di punti vita di un giocatore in base al ruolo
    return giocatore.ruoloAssegnato == SCERIFFO ? PUNTI_VITA_SCERIFFO : PUNTI_VITA_GENERICO;
}

/**
 * Funzione che calcola la gittata di un giocatore, prendendo in considerazione l'arma in uso.
 *
 * @param giocatore Giocatore di cui prendere l'arma.
 * @return La gittata del giocatore.
 */
int calcolaGittata(Giocatore giocatore) {
    int gittata = 1;
    // carta contenente l'arma del giocatore
    Carta armaInUso = prendiArmaGiocatore(giocatore);

    // calcolo la gittata in base all'arma usata
    if (strcmp(armaInUso.nomeCarta, CARTA_CARABINA) == 0) {
        gittata = GITTATA_CARABINA;
    } else if (strcmp(armaInUso.nomeCarta, CARTA_REMINGTON) == 0) {
        gittata = GITTATA_REMINGTON;
    } else if (strcmp(armaInUso.nomeCarta, CARTA_SCHOFIELD) == 0) {
        gittata = GITTATA_SCHOFIELD;
    } else if (strcmp(armaInUso.nomeCarta, CARTA_WINCHESTER) == 0) {
        gittata = GITTATA_WINCHESTER;
    } else if (strcmp(armaInUso.nomeCarta, CARTA_VOLCANIC) == 0) {
        gittata = GITTATA_VOLCANIC;
    } else if (strcmp(armaInUso.nomeCarta, CARTA_COLT45) == 0) {
        gittata = GITTATA_COLT;
    }

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
    int i;
    // posizione della (se esistente) carta birra, che può essere usata per salvarsi in punto di morte
    int cartaBirra;
    // nome del ruolo dell'utente morto
    char nomeRuolo[NOME_RUOLO_LEN_MAX + 1];

    // logica di morte del giocatore
    if(puntiVitaRimossi >= giocatoreFerito->puntiVita) {
        cartaBirra = cercaCartaMazzoPerNome(giocatoreFerito->carteMano, CARTA_BIRRA);
        // il giocatore può usare una carta birra per salvarsi
        if(cartaBirra == -1) { // se la carta non è presente, il giocatore è morto
            printf("\n%s MORTE %s", MEZZO_SEPARATORE, MEZZO_SEPARATORE);
            printf("\nIl giocatore %s è stato ferito a morte!", giocatoreFerito->nomeUtente);
            printf("\nIl giocatore %s non può salvarsi, ed è pertanto eliminato dal gioco!", giocatoreFerito->nomeUtente);

            giocatoreFerito->puntiVita = 0;

            // rivelazione del ruolo
            prendiNomeRuolo(giocatoreFerito->ruoloAssegnato, nomeRuolo);
            printf("\nRivelazione del ruolo del giocatore morto: %s!", nomeRuolo);

            // il player è stato ucciso da un altro giocatore, e non dal gioco: verifica dei bonus e malus
            if(giocatoreAttaccante != NULL) {
                // malus: eliminazione del viceceriffo da parte dello sceriffo
                if(giocatoreFerito->ruoloAssegnato == VICESCERIFFO && giocatoreAttaccante->ruoloAssegnato == SCERIFFO) {
                    printf("\nUn VICESCERIFFO è stato ucciso dallo sceriffo! %s deve scartare tutte le sue carte!", giocatoreAttaccante->nomeUtente);
                    while (giocatoreAttaccante->carteMano.numeroCarte > 0) {
                        scartaCarta(giocatoreAttaccante, &giocatoreAttaccante->carteMano, mazzoScarti);
                    }
                // bonus: eliminazione di un fuorilegge
                } else if(giocatoreFerito->ruoloAssegnato == FUORILEGGE) {
                    printf("\nE' stato eliminato un FUORILEGGE! %s può pescare 3 carte!", giocatoreAttaccante->nomeUtente);
                    pescaCarte(mazzoPesca, mazzoScarti, giocatoreAttaccante, 3);
                }
            }
            printf("\n%s MORTE %s\n", MEZZO_SEPARATORE, MEZZO_SEPARATORE);

            // spostamento delle carte del morto nel mazzo degli scarti
            for(i = 0; i < giocatoreFerito->carteMano.numeroCarte; i++) {
                spostaCartaMazzo(&giocatoreFerito->carteMano, mazzoScarti, i);
            }
            for(i = 0; i < giocatoreFerito->carteGioco.numeroCarte; i++) {
                spostaCartaMazzo(&giocatoreFerito->carteGioco, mazzoScarti, i);
            }

            scriviMorteGiocatoreSuLog(giocatoreFerito->nomeUtente);
        } else {
            printf("\n%s è stato ferito a morte, ma possiede una carta '%s'!\n"
                   "Usandola, si salva da morte certa, rimanendo però con un solo punto vita!", giocatoreFerito->nomeUtente, CARTA_BIRRA);

            giocatoreFerito->puntiVita = 1; // imposto la vita a uno
            spostaCartaMazzo(&giocatoreFerito->carteMano, mazzoScarti, cartaBirra);
        }
    } else { // il giocatore non è ferito a morte
        // rimozione dei punti vita persi
        giocatoreFerito->puntiVita -= puntiVitaRimossi;
    }

    // logging
    if(giocatoreAttaccante == NULL) {
        scriviPuntiVitaPersiSuLog(giocatoreFerito->nomeUtente, "", puntiVitaRimossi);
    } else {
        scriviPuntiVitaPersiSuLog(giocatoreFerito->nomeUtente, giocatoreAttaccante->nomeUtente, puntiVitaRimossi);
    }
}

/**
 * Subroutine che mostra la vita di tutti i giocatori attualmente non morti.
 *
 * @param nGiocatori Il numero di giocatori totali.
 * @param giocatori L'array di giocatori.
 */
void mostraVitaGiocatori(int nGiocatori, Giocatore giocatori[nGiocatori]) {
    int i;

    printf("\n%s VITA GIOCATORI %s", MEZZO_SEPARATORE, MEZZO_SEPARATORE);
    for(i = 0; i < nGiocatori; i++) {
        if(giocatori[i].puntiVita > 0) // filtro per escludere i giocatori morti
            printf("\n*) Punti vita di %s: %d", giocatori[i].nomeUtente, giocatori[i].puntiVita);
    }
    printf("\n%s VITA GIOCATORI %s\n", MEZZO_SEPARATORE, MEZZO_SEPARATORE);
}

/**
 * Funzione che permette a un giocatore di scegliere tra una lista di giocatori mostrando un prompt di scelta,
 * effettuando tutti i controlli relativi al nome scelto. Restituisce il puntatore al giocatore selezionato.
 * Se non ci sono giocatori attaccabili ad una certa distanza, la funzione restituisce "NULL".
 * Se richiesto dagli ultimi due parametri, la funzione prende in considerazione per la scelta anche le distanze
 * ('calcolaDistanze'), le mostra e basta nel prompt di scelta ('mostraDistanze') o anche entrambi.
 *
 * @param nGiocatori Numero totale di giocatori.
 * @param giocatori Array dei giocatori.
 * @param posizioneGiocatore Posizione del giocatore che sta scegliendo un altro giocatore.
 * @return Puntatore al giocatore scelto, NULL se non ci sono giocatori validi.
 */
Giocatore *selezionaGiocatore(int nGiocatori, Giocatore giocatori[nGiocatori], int posizioneGiocatore, bool mostraDistanze, bool calcolaDistanze) {
    int i;
    // variabile booleana per assicurarsi che ci sia almeno un giocatore attaccabile
    bool giocatoreAttaccabile = true;
    // variabile booleana per verificare che un giocatore sia stato trovato
    bool giocatoreTrovato;
    // variabile che contiene il nome del giocatore selezionato
    char nomeGiocatoreScelto[NOME_UTENTE_LEN + 1];
    // puntatore al giocatore che è stato selezionato
    Giocatore* giocatoreScelto = NULL;
    // puntatore all'array contenente le distanze dei giocatori
    int* distanze = NULL;

    // calcolo le distanze, se richiesto dalla funzione
    if(mostraDistanze || calcolaDistanze)
        distanze = calcolaDistanzeGiocatori(nGiocatori, giocatori, posizioneGiocatore);

    // verifico che ci sia almeno un giocatore che può essere raggiunto
    if(calcolaDistanze)
        giocatoreAttaccabile = esisteGiocatoreVicino(nGiocatori, giocatori, posizioneGiocatore, giocatori[posizioneGiocatore].gittata);


    if(giocatoreAttaccabile) {
        // ciclo di scelta
        do {
            // se il giocatore ha cambiato idea in precedenza, resetto il valore di giocatoreTrovato
            giocatoreTrovato = false;
            // mostro la lista dei giocatori
            printf("\n\n%s GIOCATORI %s", MEZZO_SEPARATORE, MEZZO_SEPARATORE);
            for (i = 0; i < nGiocatori; i++) {
                // filtro i giocatori morti e il giocatore stesso
                if (strcmp(giocatori[i].nomeUtente, giocatori[posizioneGiocatore].nomeUtente) != 0 && giocatori[i].puntiVita > 0) {
                    if(mostraDistanze) { // decido se mostrare le distanze o no
                        printf("\n*) %s (distanza: %d)", giocatori[i].nomeUtente, distanze[i]);
                    } else {
                        printf("\n*) %s", giocatori[i].nomeUtente);
                    }
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
                for (i = 0; i < nGiocatori && !giocatoreTrovato; i++) {
                    if (strcmp(giocatori[i].nomeUtente, nomeGiocatoreScelto) == 0) {
                        // un giocatore è stato trovato nella lista
                        giocatoreTrovato = true;
                        // se richiesto, considero anche la distanza nella scelta
                        if(calcolaDistanze && distanze[i] > giocatori[posizioneGiocatore].gittata) {
                            printf("\nIl giocatore scelto è troppo lontano!");
                        // il giocatore scelto è morto
                        } else if (giocatori[i].puntiVita < 1) {
                            printf("\nIl giocatore scelto è morto!");
                        // il giocatore scelto è valido
                        } else {
                            giocatoreScelto = &giocatori[i];
                        }
                    }
                }

                if(giocatoreScelto == NULL) {
                    printf("\nIl giocatore fornito è invalido! Scegline un altro.");
                }
            } else {
                printf("\nNon puoi scegliere te stesso!");
            }
        } while (giocatoreScelto == NULL); // in questo modo il ciclo si ripete finché un giocatore non è trovato
    } else {
        printf("\nNon esiste un giocatore attaccabile con la tua gittata!");
    }

    if(distanze != NULL)
        free(distanze); // liberazione della memoria

    return giocatoreScelto;
}

/**
 * Funzione che verifica che ci sia, nel mazzo delle carte in gioco di un giocatore, una carta col nome dato.
 *
 * @param giocatore Il giocatore da verificare.
 * @param nomeCarta Il nome della carta da cercare.
 * @return True se la carta è in gioco, False altrimenti.
 */
bool possiedeCartaInGioco(Giocatore giocatore, char nomeCarta[]) {
    return cercaCartaMazzoPerNome(giocatore.carteGioco, nomeCarta) != -1;
}

/**
 * Funzione che restituisce true o false, a seconda che esista un giocatore ad una distanza pari
 * a quella data dal giocatore fornito.
 *
 * @param nGiocatori Numero di giocatori.
 * @param giocatori Array dei giocatori.
 * @param posizioneGiocatore Posizione del giocatore da cui misurare le distanze.
 * @param distanza La distanza da verificare.
 * @return True se esiste un giocatore vicino, False altrimenti
 */
bool esisteGiocatoreVicino(int nGiocatori, Giocatore giocatori[nGiocatori], int posizioneGiocatore, int distanza) {
    int i;
    // array di distanza dei giocatori
    int* distanzaGiocatori = calcolaDistanzeGiocatori(nGiocatori, giocatori, posizioneGiocatore);
    // booleano per verificare che il giocatore sia stato trovato
    bool giocatoreTrovato = false;

    for(i = 0; i < nGiocatori && !giocatoreTrovato; i++) {
        // esiste un giocatore in vita ad una distanza uguale o minore a quella data
        if(giocatori[i].puntiVita > 0 && strcmp(giocatori[i].nomeUtente, giocatori[posizioneGiocatore].nomeUtente) != 0 && distanzaGiocatori[i] <= distanza) {
            giocatoreTrovato = true;
        }
    }
    // libero la memoria
    free(distanzaGiocatori);

    return giocatoreTrovato;
}