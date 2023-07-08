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
        distanze[i] = calcolaDistanzaGiocatore(nGiocatori, giocatori, posizioneGiocatore, i);

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
int calcolaDistanzaGiocatore(int nGiocatori, Giocatore giocatori[], int posizioneGiocatore1, int posizioneGiocatore2) {
    int i = posizioneGiocatore1;
    int distanza = 0;
    // la distanza viene calcolata una volta da sinistra a destra, poi da destra a sinistra
    int distanzaGiroOrario = 0, distanzaGiroAntiOrario = 0;

    // calcolo distanza in senso orario
    do {
        i = (i + 1) % nGiocatori;
        if(giocatori[i].puntiVita > 0) // filtro i giocatori morti
            distanzaGiroOrario++;
    } while (i != posizioneGiocatore2);

    // calcolo distanza in senso anti orario
    i = posizioneGiocatore1;
    do {
        i = (i - 1) % nGiocatori;
        if(i < 0)
            i += nGiocatori; // se i è negativo, il risultato del modulo deve essere reso positivo
        if(giocatori[i].nomeUtente > 0) // filtro i giocatori morti
            distanzaGiroAntiOrario++;
    } while (i != posizioneGiocatore2);

    // la distanza vera è la più piccola tra le due
    if(distanzaGiroAntiOrario < distanzaGiroOrario) {
        distanza = distanzaGiroAntiOrario;
    } else {
        distanza = distanzaGiroOrario;
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

/**
 * Funzione che gioca la carta dalla mano di un giocatore.
 * La carta, dopo essere stata giocata, NON viene scartata dalla mano del giocatore. Invece,
 * la funzione restituisce "true" o "false", se la carta è stata giocata o meno.
 * Se un giocatore gioca una carta "in gioco" o "arma", questa viene spostata nel mazzo di carte in gioco.
 *
 * @param mazzoPesca Puntatore al mazzo delle carte di pesca.
 * @param mazzoScarti Puntatore al mazzo delle carte scartate.
 * @param nGiocatori Numero totale di giocatori.
 * @param giocatori Array di giocatori totali.
 * @param posizioneGiocatore Posizione del giocatore che gioca la carta
 * @param posizioneCarta Posizione della carta giocata.
 * @return True, se la carta viene effettivamente giocata, false altrimenti.
 */
bool giocaCarta(Mazzo *mazzoPesca, Mazzo *mazzoScarti, int nGiocatori, Giocatore giocatori[nGiocatori], int posizioneGiocatore, int posizioneCarta) {
    int i;
    // puntatore al giocatore che sta giocando la carta al momento
    Giocatore* giocatore = &giocatori[posizioneGiocatore];
    // carta da giocare
    Carta carta = giocatori[posizioneGiocatore].carteMano.carte[posizioneCarta];
    // variabili di appoggio utilizzate nei cicli do-while interni per chiedere conferma delle scelte
    bool confermaCarta = false;
    char confermaAzione;

    // PRIMA PARTE: carte istantanee
    if (carta.tipologiaCarta == ISTANTANEA) {
        if (strcmp(carta.nomeCarta, "Bang!") == 0) {
            // e il puntatore al giocatore bersagliato
            Giocatore* giocatoreScelto = NULL;
            // carta estratta nel caso in cui ci sia in gioco un barile
            Carta cartaEstratta;

            printf("\nLa carta 'Bang!' ti permette di infliggere danni a un giocatore.");

            printf("\nLa tua gittata attuale è: %d", giocatore->gittata);

            // chiedo all'utente a quale giocatore vorrebbe sparare, finché non conferma la sua scelta
            do {
                giocatoreScelto = selezionaGiocatore(nGiocatori, giocatori, posizioneGiocatore, true, true);

                if(giocatoreScelto == NULL)
                    return false; // la carta non può essere giocata

                // chiedo all'utente di confermare la sua scelta
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
            } while (!confermaCarta);

            scriviCartaGiocataAvversario(giocatore->nomeUtente, giocatoreScelto->nomeUtente, carta);

            // verifica di eventuali azioni per evitare lo sparo
            // primo elemento: carta barile
            if (possiedeCartaInGioco(*giocatoreScelto, "Barile")) {
                printf("\nIl giocatore %s possiede una carta 'Barile'. Sarà estratta una carta che, se sarà"
                       "del seme di Cuori, annullerà completamente l'attacco.", giocatoreScelto->nomeUtente);

                printf("\nPremi 'Invio' per estrarre una carta.");
                while (getchar() != '\n') // aspetto che il giocatore prema invio
                    continue;
                getchar();

                // estraggo una carta (che sarà spostata nel mazzo scarti) attraverso la funzione 'estraiCarta'.
                cartaEstratta = estraiCarta(mazzoPesca, mazzoScarti);
                // attacco mancato
                if (cartaEstratta.tipologiaCarta == CUORI) {
                    printf("\nLa carta estratta è di Cuori! L'attacco è mancato!");
                    scriviBarileSuLog(giocatore->nomeUtente, cartaEstratta);
                    return true;
                // attacco riuscito
                } else {
                    printf("\nLa carta estratta non è una carta di Cuori!");
                }
                scriviBarileSuLog(giocatore->nomeUtente, cartaEstratta);
            }

            // secondo elemento: carta mancato
            int cartaMancato = cercaCartaMazzoPerNome(giocatoreScelto->carteMano, "Mancato");
            // il giocatore possiede un 'Mancato!'
            if (cartaMancato != -1) {
                svuotaSchermo();
                printf("\nIl giocatore %s possiede una carta 'Mancato'! Se deciderà di giocarla, potrà evitare l'attacco.", giocatoreScelto->nomeUtente);
                printf("\nPassa lo schermo a %s per permettergli di scegliere se giocare la carta.\n"
                       "Premi 'Invio' per continuare.", giocatoreScelto->nomeUtente);
                while (getchar() != '\n') // aspetto che il giocatore sia sullo schermo
                    continue;
                getchar();

                do {
                    printf("\n%s ha giocato una carta 'Bang!' contro di te: desideri scartare una carta 'Mancato!' ed evitare l'attacco?\n"
                           "%c/%c) ", giocatore->nomeUtente, PROMPT_CONFERMA, PROMPT_RIFIUTA);
                    scanf(" %c", &confermaAzione);

                    // verifico la scelta del giocatore attaccato

                    if (confermaAzione == PROMPT_CONFERMA) {
                        printf("\n%s ha giocato un 'Mancato' ed evita l'attacco!", giocatoreScelto->nomeUtente);

                        // scarto la carta 'Mancato!' appena giocata
                        aggiungiCartaMazzo(mazzoScarti, giocatore->carteMano.carte[cartaMancato]);
                        rimuoviCartaMazzo(&giocatoreScelto->carteMano, cartaMancato);

                        scriviMancatoSuLog(giocatore->nomeUtente);

                        return true; // la carta 'Bang!' è stata comunque giocata, quindi termino la funzione
                    } else if(confermaAzione == PROMPT_RIFIUTA) {
                        printf("\n%s ha deciso di non giocare un 'Mancato'!", giocatoreScelto->nomeUtente);
                    } else {
                        printf("\nInserisci un valore valido!");
                    }
                } while (confermaAzione != PROMPT_CONFERMA && confermaAzione != PROMPT_RIFIUTA);
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
            scriviCartaGiocataSuLog(giocatore->nomeUtente, carta);
            giocatori[posizioneGiocatore].puntiVita++;
        } else if (strcmp(carta.nomeCarta, "Diligenza") == 0) {
            printf("\n%s pesca 2 carte!", giocatore->nomeUtente);
            scriviCartaGiocataSuLog(giocatore->nomeUtente, carta);
            pescaCarte(mazzoPesca, mazzoScarti, &giocatori[posizioneGiocatore], 2);
        } else if (strcmp(carta.nomeCarta, "Panico!") == 0) {
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


            printf("\nLa carta 'Panico!' ti permette di rubare una carta a un giocatore di distanza 1.");

            // verifico che ci sia un giocatore a distanza 1
            if(esisteGiocatoreVicino(nGiocatori, giocatori, posizioneGiocatore, 1)) {
                // chiedo all'utente il nome del giocatore che intende bersagliare
                do {
                    do {
                        // chiedo al giocatore di sceglierne un altro, mostrando le distanze
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
                            giocatoreScelto = NULL;
                        // verifico che l'utente scelto abbia effettivamente delle carte
                        } else if(giocatoreScelto->carteMano.numeroCarte < 1 && giocatoreScelto->carteGioco.numeroCarte < 1) {
                            printf("\nIl giocatore non ha nessuna carta!");
                            giocatoreScelto = NULL;
                        }

                        // potrebbe essere che l'unico giocatore disponibile non abbia carte, quindi chiedo all'utente
                        // se preferisce sceglierne un altro o tornare al menu prinicpale
                        if(giocatoreScelto == NULL) {
                            do {
                                printf("\nIl giocatore scelto non è valido. Vuoi sceglierne un altro?\n"
                                       "%c/%c) ", PROMPT_CONFERMA, PROMPT_RIFIUTA);
                                scanf(" %c", &confermaAzione);
                                if(confermaAzione == PROMPT_RIFIUTA) {
                                    printf("\nTorno al menu principale!");
                                    return false;
                                } else if(confermaAzione != PROMPT_CONFERMA && confermaAzione != PROMPT_RIFIUTA) {
                                    printf("\nInserisci un azione valida!");
                                }
                            } while (confermaAzione != PROMPT_CONFERMA && confermaAzione != PROMPT_RIFIUTA);
                        }
                    } while(giocatoreScelto == NULL);

                    // e chiedo all'utente di confermare la scelta
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
                                return false;
                            } else if(confermaAzione == PROMPT_RIFIUTA) {
                                giocatoreScelto = NULL;
                            } else {
                                printf("\nInserisci un valore valido!");
                            }
                        } else if(confermaAzione != PROMPT_CONFERMA && confermaAzione != PROMPT_RIFIUTA) {
                            printf("\nInserisci un valore valido!");
                        }
                    } while (confermaAzione != PROMPT_CONFERMA && confermaAzione != PROMPT_RIFIUTA);

                    // se il giocatore è stato trovato, entro nel ciclo.
                    // dato che all'interno del ciclo il giocatore potrebbe cambiare idea e sceglierne un altro,
                    // chiudo il ciclo se si intraprende questa scelta
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
                                       "%c/%c) ", PROMPT_MAZZO_MANO, PROMPT_MAZZO_GIOCO, PROMPT_MAZZO_MANO, PROMPT_MAZZO_GIOCO);
                                scanf(" %c", &mazzoSceltoPrompt);

                                if (mazzoSceltoPrompt != PROMPT_MAZZO_MANO && mazzoSceltoPrompt != PROMPT_MAZZO_GIOCO) {
                                    printf("\nInserisci un valore valido!");
                                    confermaAzione = false;
                                    // verifico che il mazzo scelto abbia delle carte
                                } else if (mazzoSceltoPrompt == PROMPT_MAZZO_MANO && giocatoreScelto->carteMano.numeroCarte < 1 ||
                                           mazzoSceltoPrompt == PROMPT_MAZZO_GIOCO && giocatoreScelto->carteGioco.numeroCarte < 1) {
                                    printf("\nIl mazzo scelto è vuoto!");
                                    confermaAzione = false;
                                } else {
                                    if(mazzoSceltoPrompt == PROMPT_MAZZO_MANO) {
                                        mazzoScelto = &giocatoreScelto->carteMano;
                                    } else {
                                        mazzoScelto = &giocatoreScelto->carteGioco;
                                    }
                                    confermaAzione = true; // il mazzo è stato scelto in modo corretto
                                }
                            } while (!confermaAzione);


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
                                // il giocatore potrebbe giocatore, o uscire
                                if(confermaAzione == PROMPT_RIFIUTA) {
                                    printf("\nDesideri cambiare giocatore? Inserendo no, tornerai al menu principale.\n"
                                           "%c/%c) ", PROMPT_CONFERMA, PROMPT_RIFIUTA);
                                    scanf(" %c", &confermaAzione);
                                    if(confermaAzione == PROMPT_RIFIUTA) {
                                        printf("\nTorno al menu principale!");
                                        return false;
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

                scriviCartaRubataPanico(giocatore->nomeUtente, giocatoreScelto->nomeUtente, mazzoScelto->carte[posizioneCartaDaRubare - 1]);

                // la scelta è stata confermata, quindi rimuovo la carta dal mazzo da rubare e la aggiungo a quello del giocatore
                printf("\n%s ruba una carta '%s' a %s!", giocatore->nomeUtente, cartaDaRubare->nomeCarta, giocatoreScelto->nomeUtente);
                spostaCartaMazzo(mazzoScelto, &giocatore->carteMano, posizioneCartaDaRubare - 1);

            } else {
                printf("\nNon esiste nessun giocatore a distanza uno!");
                return false;
            }
        } else if (strcmp(carta.nomeCarta, "CatBalou") == 0) {
            // variabile contenente il carattere usato per scegliere il mazzo da cui scartare
            char mazzoScelto;
            // puntatore al giocatore a cui far scartare una carta
            Giocatore* giocatoreScelto = NULL;

            printf("\n%s DESCRIZIONE %s", MEZZO_SEPARATORE, MEZZO_SEPARATORE);
            printf("\nLa carta 'CatBalou' ti permette di far scartare una carta\n"
                   "a un giocatore a scelta, a prescindere dalla distanza.");
            printf("\n%s DESCRIZIONE %s\n", MEZZO_SEPARATORE, MEZZO_SEPARATORE);

            do {
                // mostro all'utente la lista dei giocatori in vita, escluso sé stesso
                giocatoreScelto = selezionaGiocatore(nGiocatori, giocatori, posizioneGiocatore, false, false);

                // verifico che il giocatore abbia effettivamente delle carte
                if(giocatoreScelto->carteMano.numeroCarte > 0 || giocatoreScelto->carteGioco.numeroCarte > 0) {
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
                } else {
                    // il giocatore non ha carte, chiedo all'utente se vuole uscire
                    printf("\nIl giocatore scelto non ha alcuna carta!");

                    do {
                        printf("\nDesideri scegliere un altro giocatore?"
                               "%c/%c) ", PROMPT_CONFERMA, PROMPT_RIFIUTA);
                        scanf(" %c", &confermaAzione);
                        if(confermaAzione == PROMPT_RIFIUTA) {
                            printf("\nTorno al menu principale!");
                            return false;
                        } else if(confermaAzione == PROMPT_CONFERMA) {
                            giocatoreScelto = NULL; // annullo la scelta precedente
                        } else {
                            printf("\nInserisci un'azione valida!");
                        }
                    } while (confermaAzione != PROMPT_CONFERMA && confermaAzione == PROMPT_RIFIUTA);
                }
            } while (giocatoreScelto == NULL);

            scriviCartaGiocataSuLog(giocatore->nomeUtente, carta);

            // lo schermo deve passare al giocatore che deve scartare
            svuotaSchermo();
            printf("\n%s ha giocato un CatBalou contro %s, che deve scartare una carta!", giocatore->nomeUtente,
                   giocatoreScelto->nomeUtente);
            printf("\nPassa lo schermo a %s per permettergli di scegliere quale carta scartare.\n"
                   "Premi 'Invio' appena sei pronto.", giocatoreScelto->nomeUtente);
            while (getchar() != '\n')
                continue;
            getchar();

            do {
                // chiedo all'utente se vuole scartare dal mazzo della mano o da quello con le carte in gioco
                printf("\nScegli se scartare dal mazzo di mano o da quello delle carte in gioco!");
                printf("\n%s CARTE MANO %s", MEZZO_SEPARATORE, MEZZO_SEPARATORE);
                mostraCarteMazzo(giocatoreScelto->carteMano);
                printf("\n\n%s CARTE GIOCO %s", MEZZO_SEPARATORE, MEZZO_SEPARATORE);
                mostraCarteMazzo(giocatoreScelto->carteGioco);
                printf("\nChe mazzo scegli? (%c) per il mazzo di mano, (%c) per quello di gioco.\n"
                       "%c/%c) ", PROMPT_MAZZO_MANO, PROMPT_MAZZO_GIOCO, PROMPT_MAZZO_MANO, PROMPT_MAZZO_GIOCO);
                scanf(" %c", &mazzoScelto);

                if (mazzoScelto != PROMPT_MAZZO_MANO && mazzoScelto != PROMPT_MAZZO_GIOCO) {
                    printf("\nInserisci un valore valido!");
                    confermaAzione = false;
                // verifico che il mazzo scelto abbia delle carte
                } else if (mazzoScelto == PROMPT_MAZZO_MANO && giocatoreScelto->carteMano.numeroCarte < 1 ||
                           mazzoScelto == PROMPT_MAZZO_GIOCO && giocatoreScelto->carteGioco.numeroCarte < 1) {
                    printf("\nIl mazzo scelto è vuoto!");
                    confermaAzione = false;
                } else {
                    confermaAzione = true; // il mazzo è stato scelto in modo corretto
                }
            } while (confermaAzione);

            // scarto la carta e svuoto lo schermo
            if(mazzoScelto == PROMPT_MAZZO_MANO) {
                scartaCarta(giocatoreScelto, &giocatoreScelto->carteMano, mazzoScarti);
            } else if(mazzoScelto == PROMPT_MAZZO_GIOCO) {
                scartaCarta(giocatoreScelto, &giocatoreScelto->carteGioco, mazzoScarti);
            }

            svuotaSchermo();
            printf("\n%s ha scartato una carta per effetto di un Catbalou!", giocatoreScelto->nomeUtente);

        } else if (strcmp(carta.nomeCarta, "Mancato") == 0) {
            printf("\nNon puoi giocare questa carta direttamente, ma solo quando stai subendo uno sparo!");
            return false;
        } else if (strcmp(carta.nomeCarta, "Gatling") == 0) {
            printf("\n%s ha giocato un Gatling: tutti i giocatori perdono un punto vita!", giocatore->nomeUtente);
            scriviCartaGiocataSuLog(giocatore->nomeUtente, carta);

            for (i = 0; i < nGiocatori; i++) {
                if (i != posizioneGiocatore && giocatori[i].puntiVita > 0) {
                    rimuoviPuntiVita(&giocatori[i], giocatore, 1, mazzoPesca, mazzoScarti);
                }
            }
        } else if (strcmp(carta.nomeCarta, "Saloon") == 0) {
            printf("\n%s ha giocato un Saloon: tutti i giocatori guadagnano un punto vita!", giocatore->nomeUtente);
            scriviCartaGiocataSuLog(giocatore->nomeUtente, carta);

            for (i = 0; i < nGiocatori; i++) {
                if (i != posizioneGiocatore && giocatori[i].puntiVita > 0 && giocatori[i].puntiVita < prendiVitaMassima(giocatori[i])) {
                    giocatori[i].puntiVita++;
                }
            }
        } else if (strcmp(carta.nomeCarta, "WellsFargo") == 0) {
            printf("\n%s pesca 3 carte!", giocatore->nomeUtente);
            scriviCartaGiocataSuLog(giocatore->nomeUtente, carta);
            pescaCarte(mazzoPesca, mazzoScarti, &giocatori[posizioneGiocatore], 3);
        }
        // SECONDA PARTE: carte istantanee speciali
    } else if (carta.tipologiaCarta == ISTANTANEA_SPECIAL) {
        if (strcmp(carta.nomeCarta, "Duello") == 0) {
            printf("\n%s DESCRIZIONE %s", MEZZO_SEPARATORE, MEZZO_SEPARATORE);
            printf("\nLa carta 'Duello' ti permette di sfidare qualsiasi giocatore ad un duello.\n"
                   "Ad ogni round, viene estratta una carta 'Bang!' del partecipante al turno corrente:\n"
                   "il primo che non ne possiede una, perde 3 punti vita!");
            printf("\n%s DESCRIZIONE %s", MEZZO_SEPARATORE, MEZZO_SEPARATORE);

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
                            return false;
                        } else if(confermaAzione != PROMPT_RIFIUTA && confermaAzione != PROMPT_CONFERMA) {
                            printf("\nInserisci un valore valido!");
                        }
                    } else if(confermaAzione != PROMPT_RIFIUTA && confermaAzione != PROMPT_CONFERMA) {
                        printf("\nInserisci un valore valido!");
                    }
                } while (confermaAzione != PROMPT_CONFERMA && confermaAzione != PROMPT_RIFIUTA);
            } while (giocatoreScelto == NULL);

            scriviCartaGiocataSuLog(giocatore->nomeUtente, carta);

            // il giocatore è confermato, e il duello può avere inizio
            printf("\nSi dia inizio al duello tra %s e %s!", giocatore->nomeUtente, giocatoreScelto->nomeUtente);
            printf("\nEstrarrò una carta 'Bang!' una volta a testa per ciascun giocatore. Il primo a finirle perderà un punto vita!");
            printf("\nIl primo ad iniziare sarà il giocatore sfidato!");
            duellanteTurnoCorrente = giocatoreScelto;

            // svuoto il buffer
            while (getchar() != '\n')
                continue;
            do {
                printf("\nE' il turno di %s!", duellanteTurnoCorrente->nomeUtente);
                printf("\nPremi invio per continuare.");
                getchar();
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
                    scriviCartaScartataSuLog(duellanteTurnoCorrente->nomeUtente, duellanteTurnoCorrente->carteMano.carte[posizioneCartaBang]);
                    spostaCartaMazzo(&duellanteTurnoCorrente->carteMano, mazzoScarti, posizioneCartaBang);
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
            printf("\n%s FINE DUELLO %s\n", MEZZO_SEPARATORE, MEZZO_SEPARATORE);

            // rimuovo i punti vita dello sconfitto
            rimuoviPuntiVita(perdenteDuello, vincitoreDuello, 3, mazzoPesca, mazzoScarti);

        } else if (strcmp(carta.nomeCarta, "Emporio") == 0) {
            printf("\n%s DESCRIZIONE %s", MEZZO_SEPARATORE, MEZZO_SEPARATORE);
            printf("\nLa carta 'Emporio' estrae dal mazzo di pesca un numero di carte pari a quello dei giocatori in vita,\n"
                   "per poi permettere ad ogni utente (a partire da chi ha giocato la carta) di pescare una carta da questo mazzo.");
            printf("\n%s DESCRIZIONE %s", MEZZO_SEPARATORE, MEZZO_SEPARATORE);

            scriviCartaGiocataSuLog(giocatore->nomeUtente, carta);

            // variabili di appoggio per i cicli interni
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
                scriviCartePescateSuLog(giocatore->nomeUtente, giocatore->carteMano, 1);

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
            // variabile d'appoggio che contiene la posizione della carta 'Bang!' di ogni giocatore.
            // vale '-1' se il giocatore non possiede la carta
            int cartaBang;

            printf("\n%s DESCRIZIONE %s", MEZZO_SEPARATORE, MEZZO_SEPARATORE);
            printf("\nAttacco di indiani in arrivo! Ogni giocatore (eccetto chi ha giocato la carta)\n"
                   "deve giocare una carta 'Bang!', oppure perdere un punto vita!");
            printf("\n%s DESCRIZIONE %s\n", MEZZO_SEPARATORE, MEZZO_SEPARATORE);

            // pulisco il buffer
            while (getchar() != '\n')
                continue;
            for (i = 0; i < nGiocatori; i++) {
                // il giocatore deve essere in vita, e chi ha giocato la carta non viene attaccato
                if (i != posizioneGiocatore && giocatori[i].puntiVita > 0) {
                    cartaBang = cercaCartaMazzoPerNome(giocatori[i].carteMano, "Bang!");
                    printf("\nTocca a %s! Premi 'Invio' quando sei pronto.", giocatori[i].nomeUtente);
                    getchar();
                    if (cartaBang == -1) {
                        printf("\n%s non possiede una carta 'Bang!', e perde quindi un punto vita!", giocatori[i].nomeUtente);
                        rimuoviPuntiVita(&giocatori[i], NULL, 1, mazzoPesca, mazzoScarti);
                    } else {
                        printf("\n%s scarta una carta 'Bang!' ed evita l'attacco degli indiani!", giocatori[i].nomeUtente);

                        // scarto la carta 'Bang!' giocata
                        spostaCartaMazzo(&giocatori[i].carteMano, mazzoScarti, cartaBang);

                        scriviCartaScartataSuLog(giocatore->nomeUtente, carta);
                    }
                }
            }

            scriviCartaGiocataSuLog(giocatore->nomeUtente, carta);
        }
        // TERZA PARTE: armi
    } else if (carta.tipologiaCarta == ARMA) {
        // intero contenente, se esiste, l'arma già in gioco
        int posizioneArmaInUso;
        // puntatore all'arma attualmente in uso da un giocatore
        Carta armaInUso = prendiArmaGiocatore(*giocatore);

        if(strcmp(armaInUso.nomeCarta, "Colt. 45") != 0) {
            // se il giocatore non ha in gioco una colt 45 (arma di default), chiedo di confermare
            do {
                printf("Hai già un'arma equipaggiata: %s. Sei sicuro di voler cambiare arma?\n"
                       "%c/%c) ", armaInUso.nomeCarta, PROMPT_CONFERMA, PROMPT_RIFIUTA);
                scanf(" %c", &confermaAzione);
                if(confermaAzione == PROMPT_RIFIUTA) {
                    return false; // il giocatore ha cambiato idea
                } else if(confermaAzione != PROMPT_RIFIUTA && confermaAzione != PROMPT_CONFERMA) {
                    printf("\nInserisci un valore valido!");
                }
            } while (confermaAzione != PROMPT_RIFIUTA && confermaAzione != PROMPT_CONFERMA);

            // calcolo la posizione dell'arma in uso
            posizioneArmaInUso = cercaCartaMazzoPerNome(giocatore->carteGioco, armaInUso.nomeCarta);

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

            // sposto la vecchia arma nel mazzo di scarti se questa non era una colt. 45
            scriviCartaScartataSuLog(giocatore->nomeUtente, armaInUso);
            spostaCartaMazzo(&giocatore->carteGioco, mazzoScarti, posizioneArmaInUso);

            // ricalcolo della gittata del giocatore, dato che è stata cambiata arma
            giocatore->gittata = calcolaGittata(*giocatore);
        }
        // se l'arma è una colt, la sovrascrivo direttamente, non essendoci carte in gioco da scartare
        // e aggiungo la nuova carta
        aggiungiCartaInGioco(giocatore, giocatore, posizioneCarta);

        printf("\nHai equipaggiato una '%s'!", carta.nomeCarta);

        scriviCartaGiocataSuLog(giocatore->nomeUtente, carta);

    // QUARTA PARTE: carte in gioco
    } else if (carta.tipologiaCarta == EFFETTO) {
        // la carta prigione viene verificata come prima, perché le altre sono tutte carte che sono giocate e inserite
        // nel proprio mazzo di gioco
        if (strcmp(carta.nomeCarta, "Prigione") == 0) {
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

            aggiungiCartaInGioco(giocatore, giocatoreScelto, posizioneCarta);
            printf("\n%s ha messo in prigione %s!", giocatore->nomeUtente, giocatoreScelto->nomeUtente);
            scriviCartaGiocataAvversario(giocatore->nomeUtente, giocatoreScelto->nomeUtente, carta);
        } else {
            // verifico che la carta scelta non sia già in gioco
            if(!possiedeCartaInGioco(*giocatore, carta.nomeCarta)) {
                if (strcmp(carta.nomeCarta, "Barile") == 0) {
                    printf("\nLa carta 'Barile' ti consente, fintanto che è in gioco, di schivare una carta 'Bang!'.\n"
                           "Quando vieni colpito, puoi estrarre una carta: se questa è di Cuori, allora l'attacco è annullato.");
                    aggiungiCartaInGioco(giocatore, giocatore, posizioneCarta);
                    printf("\nHai giocato una carta 'Barile'!");
                    scriviCartaGiocataSuLog(giocatore->nomeUtente, carta);
                } else if (strcmp(carta.nomeCarta, "Mirino") == 0) {
                    printf("\nLa carta 'Mirino' ti consente, fintanto che è in gioco, di vedere tutti gli altri giocatori a una distanza diminuita di uno.");
                    aggiungiCartaInGioco(giocatore, giocatore, posizioneCarta);
                    printf("\nHai equipaggiato la carta 'Mirino'!");
                    scriviCartaGiocataSuLog(giocatore->nomeUtente, carta);
                } else if (strcmp(carta.nomeCarta, "Mustang") == 0) {
                    printf("\nLa carta 'Mustang' ti consente, fintanto che è in gioco, di essere visto a distanza aumentata di uno dagli altri giocatori.");
                    aggiungiCartaInGioco(giocatore, giocatore, posizioneCarta);
                    printf("\nHai equipaggiato la carta 'Mustang'!");
                    scriviCartaGiocataSuLog(giocatore->nomeUtente, carta);
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
                    scriviCartaGiocataSuLog(giocatore->nomeUtente, carta);
                }
            } else {
                printf("\nHai già una carta '%s' in gioco!", carta.nomeCarta);
                return false;
            }
        }
    }

    return true;
}

/**
 * Subroutine che rimuove una posizioneCarta dalla mano di un giocatore, per inserirla nel mazzo delle carte in gioco di
 * un altro giocatore.
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
    if(nCarte > mazzoPesca->numeroCarte) { // primo caso: il mazzo di pesca non contiene abbastanza carte, quindi bisogna scambiarlo con quello di scarti
        printf("\nIl mazzo di pesca è terminato! Lo scambio con quello degli scarti.");

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
 * @param giocatore Il giocatore che deve scartare.
 * @param mazzoScarti Il mazzo contenente le carte scartate.
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

        scriviCartaScartataSuLog(giocatore->nomeUtente, mazzoDaScartare->carte[cartaScelta - 1]);

        // scarto la carta
        spostaCartaMazzo(mazzoDaScartare, mazzoScarti, cartaScelta - 1);
    } else {
        printf("\nIl mazzo è vuoto!");
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

    // scorro tutte le carte in gioco, finché non trovo un'arma.
    for(i = 0; i < giocatore.carteGioco.numeroCarte; i++) {
        if(giocatore.carteGioco.carte[i].tipologiaCarta == ARMA)
            return giocatore.carteGioco.carte[i];
    }

    // se la funzione non è terminata, il giocatore non ha un'arma in gioco, quindi restituisco una COLT 45 appena generata
    return (Carta) {"Colt. 45", ARMA, (rand() % 12) + 1, rand() % 4};
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
    int gittata;
    // carta contenente l'arma del giocatore
    Carta armaInUso = prendiArmaGiocatore(giocatore);

    // calcolo la gittata in base all'arma usata
    if (strcmp(armaInUso.nomeCarta, "Carabina") == 0) {
        gittata = GITTATA_CARABINA;
    } else if (strcmp(armaInUso.nomeCarta, "Remington") == 0) {
        gittata = GITTATA_REMINGTON;
    } else if (strcmp(armaInUso.nomeCarta, "Schofield") == 0) {
        gittata = GITTATA_SCHOFIELD;
    } else if (strcmp(armaInUso.nomeCarta, "Winchester") == 0) {
        gittata = GITTATA_WINCHESTER;
    } else if (strcmp(armaInUso.nomeCarta, "Volcanic") == 0) {
        gittata = GITTATA_VOLCANIC;
    } else if (strcmp(armaInUso.nomeCarta, "Colt. 45") == 0) {
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
    // posizione della (se esistente) carta birra, che può essere usata per salvarsi in punto di morte
    int cartaBirra;
    // nome del ruolo dell'utente morto
    char nomeRuolo[NOME_RUOLO_LEN_MAX + 1];

    // logica di morte del giocatore
    if(puntiVitaRimossi >= giocatoreFerito->puntiVita) {
        cartaBirra = cercaCartaMazzoPerNome(giocatoreFerito->carteMano, "Birra");
        // il giocatore può usare una carta birra per salvarsi
        if(cartaBirra == -1) { // se la carta non è presente, il giocatore è morto
            printf("\n%s MORTE %s", MEZZO_SEPARATORE, MEZZO_SEPARATORE);
            printf("\nIl giocatore %s è stato ferito a morte!", giocatoreFerito->nomeUtente);
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
                        scartaCarta(giocatoreAttaccante, &giocatoreAttaccante->carteMano, mazzoScarti);
                    }
                // bonus: eliminazione di un fuorilegge
                } else if(giocatoreFerito->ruoloAssegnato == FUORILEGGE) {
                    printf("\nE' stato eliminato un FUORILEGGE! %s può pescare 3 carte!", giocatoreAttaccante->nomeUtente);
                    pescaCarte(mazzoPesca, mazzoScarti, giocatoreAttaccante, 3);
                }
                scriviPuntiVitaPersiSuLog(giocatoreFerito->nomeUtente, giocatoreAttaccante->nomeUtente, puntiVitaRimossi);
            } else {
                scriviPuntiVitaPersiSuLog(giocatoreFerito->nomeUtente, "", puntiVitaRimossi);
            }
            printf("\n%s MORTE %s\n", MEZZO_SEPARATORE, MEZZO_SEPARATORE);

            // spostamento delle carte del morto nel mazzo degli scarti
            for(int i = 0; i < giocatoreFerito->carteMano.numeroCarte; i++) {
                spostaCartaMazzo(&giocatoreFerito->carteMano, mazzoScarti, i);
            }

            scriviMorteGiocatoreSuLog(giocatoreFerito->nomeUtente);
        } else {
            printf("\n%s è stato ferito a morte, ma possiede una carta 'Birra'!\n"
                   "Usandola, si salva da morte certa, rimanendo però con un solo punto vita!", giocatoreFerito->nomeUtente);

            giocatoreFerito->puntiVita = 1; // imposto la vita a uno e termino la funzione
            spostaCartaMazzo(&giocatoreFerito->carteMano, mazzoScarti, cartaBirra);

            // scrittura su log
            if(giocatoreAttaccante == NULL) {
                scriviPuntiVitaPersiSuLog(giocatoreFerito->nomeUtente, "", puntiVitaRimossi);
            } else {
                scriviPuntiVitaPersiSuLog(giocatoreFerito->nomeUtente, giocatoreAttaccante->nomeUtente, puntiVitaRimossi);
            }

            return;
        }
    }

    // rimozione dei punti vita persi
    giocatoreFerito->puntiVita -= puntiVitaRimossi;

    // scrittura su log
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
 * Se non ci sono giocatori attaccabili ad una certa distanza,
 *
 * @param nGiocatori Numero totale di giocatori.
 * @param giocatori Array dei giocatori.
 * @param posizioneGiocatore Posizione del giocatore che sta scegliendo un altro giocatore.
 * @return Puntatore al giocatore scelto.
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
            giocatoreTrovato = false;
            // mostro la lista dei giocatori
            printf("\n\n%s GIOCATORI %s", MEZZO_SEPARATORE, MEZZO_SEPARATORE);
            for (i = 0; i < nGiocatori; i++) {
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
                    // un giocatore è stato trovato
                    if (strcmp(giocatori[i].nomeUtente, nomeGiocatoreScelto) == 0) {
                        giocatoreTrovato = true;
                        // se richiesto, considero anche la distanza nella scelta
                        if(calcolaDistanze && distanze[i] > giocatori[posizioneGiocatore].gittata) {
                            printf("\nIl giocatore scelto è troppo lontano!");
                            // il giocatore è morto
                        } else if (giocatori[i].puntiVita < 1) {
                            printf("\nIl giocatore scelto è morto!");
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
    free(distanzaGiocatori);
    return giocatoreTrovato;
}