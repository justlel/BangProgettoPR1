//
// Created by just on 24/03/23.
//
#include "giocatore.h"

// TODO: Aggiungere documentazione
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

// TODO: Aggiungere documentazione
// TODO: Modificarla per prendere in considerazione le carte che modificano la distanza
void mostraDistanze(int nGiocatori, int indiceGiocatore, Giocatore giocatori[]) {
    int i, distanza = 0;

    // calcolo dei giocatori andando verso destra
    for(i = indiceGiocatore + 1; i < nGiocatori; i++) {
        if(giocatori[i].puntiVita >= 0) {
            distanza++;
            if(giocatori[i].ruoloAssegnato == SCERIFFO) {
                printf("\nLo SCERIFFO, %s, si trova a %d posizioni di distanza.", giocatori[i].nomeUtente, distanza);
            } else {
                printf("\nIl giocatore %s si trova a %d posizioni di distanza.", giocatori[i].nomeUtente, distanza);
            }
        }
    }
    distanza = 0;
    // calcolo dei giocatori andando verso sinistra
    for(i = indiceGiocatore - 1; i >= 0; i--) {
        if(giocatori[i].puntiVita >= 0) {
            distanza--;
            if(giocatori[i].ruoloAssegnato == SCERIFFO) {
                printf("\nLo SCERIFFO, %s, si trova a %d posizioni di distanza.", giocatori[i].nomeUtente, distanza);
            } else {
                printf("\nIl giocatore %s si trova a %d posizioni di distanza.", giocatori[i].nomeUtente, distanza);
            }
        }
    }
}

int calcoloDistanza(Giocatore giocatori[], int posizioneGiocatore1, int posizioneGiocatore2) {
    int i, posizioneBase = abs(posizioneGiocatore1 - posizioneGiocatore2);

    if(posizioneGiocatore2 > posizioneGiocatore1) {
        // scorrimento a destra
        for(i = posizioneGiocatore1; i < posizioneGiocatore2; i++) {
            if(giocatori[i].puntiVita == 0)
                posizioneBase--;
        }
    } else {
        // scorrimento a sinistra
        for(i = posizioneGiocatore1; i > posizioneGiocatore2; i--) {
            if(giocatori[i].puntiVita == 0)
                posizioneBase--;
        }
    }
    // se il giocatore 1 ha un mirino, vede tutti a distanza diminuita di uno
    if(posizioneBase > 1 && possiedeCartaInGioco(giocatori[posizioneGiocatore1], "Mirino"))
        posizioneBase--;
    // se il giocatore 2 ha un mustang, tutti lo vedono a distanza aumentata di uno
    if(possiedeCartaInGioco(giocatori[posizioneGiocatore2], "Mustang"))
        posizioneBase++;
    return posizioneBase;
}

// TODO: Aggiungere documentazione
void mostraCarteInGiocoGiocatore(Giocatore giocatore) {
    int i;
    char tipologiaCarta[TIPOLOGIA_CARTA_LEN + 1], semeCarta[SEME_CARTA_LEN + 1];

    if(giocatore.carteGioco.numeroCarte == 0)
        printf("\nNessuna carta in gioco!");
    for(i = 0; i < giocatore.carteGioco.numeroCarte; i++) {
        prendiTipologiaCarta(giocatore.carteGioco.carte[i].tipologiaCarta, tipologiaCarta);
        prendiSemeCarta(giocatore.carteGioco.carte[i].semeCarta, semeCarta);
        printf("\n%d) %s\n"
               "Tipologia: %s\n"
               "Numero della carta: %d\n"
               "Seme della carta: %s",
               i+1, giocatore.carteGioco.carte[i].nomeCarta, tipologiaCarta, giocatore.carteGioco.carte[i].numeroCarta, semeCarta);
    }
}

// TODO: Aggiungere documentazione
// TODO: Forse si può modificare in modo da fornire, al posto del giocatore da escludere, il suo indice (?)
void mostraCarteInGiocoAltri(int nGiocatori, Giocatore giocatori[], Giocatore giocatore) {
    int i;

    for(i = 0; i < nGiocatori; i++) {
        if(strcmp(giocatori[i].nomeUtente, giocatore.nomeUtente) != 0) {
            printf("\n------ Carte in gioco di %s ------", giocatori[i].nomeUtente);
            mostraCarteInGiocoGiocatore(giocatore);
        }
    }
}

// TODO: Aggiungere documentazione
void giocaCarta(int nGiocatori, Giocatore giocatori[], int posizioneGiocatore, Carta carta, Mazzo* mazzoCarte) {
    int i;
    Giocatore* giocatore = &giocatori[posizioneGiocatore];
    bool confermaCarta;
    char confermaAzione;

    // PRIMA PARTE: carte istantanee
    if(carta.tipologiaCarta == ISTANTANEA) {
        // TODO: bisogna trovare un modo per limitare l'utilizzo di bang a uno per turno
        if(strcmp(carta.nomeCarta, "Bang!") == 0) {
            int gittata = 0, distanza = 0, nGiocatoriRaggiungibili = 0, destinazioneSparo;
            Carta armaInUso;

            // inizializzazione dell'array a 0, poi la dimensione sarà riaggiustata al calcolo delle distanze
            int* giocatoriRaggiungibili = calloc(0, sizeof(int));
            if(giocatoriRaggiungibili == NULL) {
                printf("\nImpossibile allocare dinamicamente memoria.");
                exit(-1);
            }

            printf("\nLa carta 'Bang!' ti permette di infliggere danni a un giocatore.");
            // calcolo della gittata
            printf("\nAl momento, la tua gittata totale è data da questi fattori:");

            armaInUso = prendiArmaGiocatore(*giocatore);
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

            // popolo l'array giocatoriRaggiungibili con le posizioni di ogni giocatore raggiungibile, così da non dover effettuare troppe volte il calcolo
            printf("\nConsiderate le carte a disposizione, questi sono i giocatori a cui puoi infliggere danno:");
            for(i = 0; i < nGiocatori; i++) {
                if(strcmp(giocatori[i].nomeUtente, giocatore->nomeUtente) == 0 || giocatori[i].puntiVita == 0)
                    continue;
                distanza = calcoloDistanza(giocatori, posizioneGiocatore, i);
                if(distanza <= gittata) {
                    nGiocatoriRaggiungibili++;
                    // inserisco la posizione dell'utente nell'array delle giocatoriRaggiungibili dei giocatori raggiungibili
                    giocatoriRaggiungibili = (int*) realloc(giocatoriRaggiungibili, nGiocatoriRaggiungibili);
                    if(giocatoriRaggiungibili == NULL) {
                        printf("Errore: impossibile allocare dinamicamente"); // TODO: 'utils'
                        exit(-1);
                    }
                    giocatoriRaggiungibili[nGiocatoriRaggiungibili] = i;
                    printf("\n%d) %s. Distanza: %d", i+1, giocatori[i].nomeUtente, distanza);
                }
            }

            // chiedo all'utente a quale giocatore vorrebbe sparare, finché non conferma la sua scelta
            do {
                printf("\nInserisci la posizione del giocatore a cui vorresti sparare: ");
                do {
                    scanf("%d", &destinazioneSparo);
                    // il numero fornito deve essere valido
                    if(destinazioneSparo <= 0 || destinazioneSparo > nGiocatoriRaggiungibili)
                        printf("\nInserisci un numero tra 1 e %d: \n", nGiocatoriRaggiungibili);
                } while (destinazioneSparo <= 0 || destinazioneSparo > nGiocatoriRaggiungibili);
                printf("\nStai per sparare a %s! Confermi la tua scelta?\n"
                       "%c/%c) ", giocatori[destinazioneSparo - 1].nomeUtente, PROMPT_CONFERMA, PROMPT_RIFIUTA);
                confermaAzione = getchar();
                confermaCarta = confermaAzione != PROMPT_RIFIUTA;
                if(!confermaCarta) {
                    printf("\nDesideri cambiare giocatore? Inserendo no, tornerai al menu principale."
                           "%c/%c) ", PROMPT_CONFERMA, PROMPT_RIFIUTA);
                    confermaAzione = getchar();
                    if(confermaAzione == PROMPT_RIFIUTA) {
                        printf("\nTorno al menu principale!");
                        free(giocatoriRaggiungibili);
                        return;
                    }
                }
            } while(!confermaCarta);
            // rimozione del punto vita e fine della funzione
            printf("\n%s ha sparato a %s, che perde quindi un punto vita!", giocatore->nomeUtente, giocatori[destinazioneSparo - 1].nomeUtente);
            rimuoviPuntiVita(&giocatori[destinazioneSparo - 1], 1);
            free(giocatoriRaggiungibili);
            return;
        } else if(strcmp(carta.nomeCarta, "Birra") == 0) {
            if((giocatore->ruoloAssegnato == SCERIFFO && giocatore->puntiVita == PUNTI_VITA_SCERIFFO) ||
               (giocatore->ruoloAssegnato != SCERIFFO && giocatore->puntiVita == PUNTI_VITA_GENERICO)
               ) {
                printf("\nHai già il massimo della vita!");
            } else {
                printf("\n%s ha utilizzato una birra e guadagna un punto vita!", giocatore->nomeUtente);
                giocatori[posizioneGiocatore].puntiVita++;
            }
            return;
        } else if(strcmp(carta.nomeCarta, "Diligenza") == 0) {
            printf("\n%s pesca 2 carte!", giocatore->nomeUtente);
            pescaCarte(mazzoCarte, &giocatori[posizioneGiocatore], 2);
        } else if(strcmp(carta.nomeCarta, "Panico!") == 0) {

        } else if(strcmp(carta.nomeCarta, "CatBalou!") == 0) {
            int j, indiceGiocatoreScelto;
            char giocatoreScelto[NOME_UTENTE_LEN + 1], confermaScelta;
            bool ripetizioneCiclo = true;

            do {
                printf("\nScegli il giocatore a cui far scartare una carta!");
                for(j = 0; j < nGiocatori; j++) {
                    if(strcmp(giocatori[j].nomeUtente, giocatore->nomeUtente) == 0 || giocatori[j].puntiVita == 0)
                        continue;
                    printf("*) %s", giocatori[j].nomeUtente);
                }

                printf("?) ");
                do {
                    scanf(" %49s", giocatoreScelto);
                    if(strcmp(giocatoreScelto, giocatore->nomeUtente) != 0) {
                        for (j = 0; ripetizioneCiclo || j < nGiocatori; j++) {
                            if (strcmp(giocatori[j].nomeUtente, giocatoreScelto) == 0) {
                                indiceGiocatoreScelto = j;
                                ripetizioneCiclo = false;
                            }
                        }
                    }
                    ripetizioneCiclo = ripetizioneCiclo && giocatori[indiceGiocatoreScelto].puntiVita > 0;
                    if(ripetizioneCiclo)
                        printf("\nIl nome inserito non è valido! Inserisci un altro utente: ");
                } while(ripetizioneCiclo);

                printf("\nConfermi la tua scelta di sparare a %s?"
                       "%c/%c) ", giocatori[indiceGiocatoreScelto].nomeUtente, PROMPT_CONFERMA, PROMPT_RIFIUTA);
                confermaAzione = getchar();
                confermaCarta = confermaAzione != PROMPT_RIFIUTA;
                if(!confermaCarta) {
                    printf("\nDesideri scegliere un altro giocatore? Inserendo no, tornerai al menu principale\n"
                           "%c/%c) ", PROMPT_CONFERMA, PROMPT_RIFIUTA);
                    confermaAzione = getchar();
                    if(confermaAzione == PROMPT_RIFIUTA) {
                        printf("\nTorno al menu principale!");
                        return;
                    }
                }
            } while(!confermaCarta);

            printf("\n%s ha giocato un Cat Balou contro %s, che deve scartare una carta!", giocatore->nomeUtente, giocatoreScelto);
            printf("\nPassa lo schermo a %s per permettergli di scegliere quale carta scartare.", giocatoreScelto);
            scartaCarta(&giocatori[indiceGiocatoreScelto]);
        } else if(strcmp(carta.nomeCarta, "Mancato!") == 0) {
            printf("\nNon puoi giocare questa carta direttamente, ma solo quando stai subendo uno sparo!");
        } else if(strcmp(carta.nomeCarta, "Gatling") == 0) {
            int j;

            printf("\n%s ha giocato un Gatling: tutti i giocatori perdono un punto vita!", giocatore->nomeUtente);
            for(j = 0; j < nGiocatori; j++) {
                if(j != posizioneGiocatore && giocatori[j].puntiVita > 0) {
                    rimuoviPuntiVita(&giocatori[j], 1);
                }
            }
        } else if(strcmp(carta.nomeCarta, "Saloon") == 0) {
            int j;

            printf("\n%s ha giocato un Saloon: tutti i giocatori guadagnano un punto vita!", giocatore.nomeUtente);
            for(j = 0; j < nGiocatori; j++) {
                if(j != posizioneGiocatore && giocatori[j].puntiVita > 0) {
                    giocatori[j].puntiVita++;
                }
            }
        } else if(strcmp(carta.nomeCarta, "WellsFargo") == 0) {
            printf("\n%s pesca 3 carte!", giocatore->nomeUtente);
            pescaCarte(mazzoCarte, &giocatori[posizioneGiocatore], 3);
        }
    // SECONDA PARTE: carte istantanee speciali
    } else if(carta.tipologiaCarta == ISTANTANEA_SPECIAL) {
        if(strcmp(carta.nomeCarta, "Duello") == 0) {
            int j, indiceGiocatoreScelto, turnoDuello = 0, cartaBangDaScartare;
            char giocatoreScelto[NOME_UTENTE_LEN + 1];
            bool ripetizioneCiclo, vittoriaSfidante = false, vittoriaSfidato = false;
            Giocatore* vincitoreDuello;

            printf("\nInserisci il nome del giocatore da sfidare a duello!");
            for(j = 0; j < nGiocatori; j++) {
                if(strcmp(giocatori[j].nomeUtente, giocatore->nomeUtente) == 0 || giocatori[j].puntiVita == 0)
                    continue;
                printf("*) %s", giocatori[j].nomeUtente);
            }

            printf("?) ");
            do  {
                do {
                    scanf(" %49s", giocatoreScelto);
                    if(strcmp(giocatoreScelto, giocatore->nomeUtente) != 0) {
                        for (j = 0; ripetizioneCiclo || j < nGiocatori; j++) {
                            if (strcmp(giocatori[j].nomeUtente, giocatoreScelto) == 0) {
                                indiceGiocatoreScelto = j;
                                ripetizioneCiclo = false;
                            }
                        }
                    }
                    ripetizioneCiclo = ripetizioneCiclo && giocatori[indiceGiocatoreScelto].puntiVita > 0;
                    if(ripetizioneCiclo)
                        printf("\nIl nome inserito non è valido! Inserisci un altro utente: ");
                } while(ripetizioneCiclo);

                printf("\nSei sicuro di voler sfidare %s a duello?\n"
                       "%c/%c) ", giocatori[indiceGiocatoreScelto].nomeUtente, PROMPT_CONFERMA, PROMPT_RIFIUTA);
                confermaAzione = getchar();
                confermaCarta = confermaAzione != PROMPT_RIFIUTA;
                if(!confermaCarta) {
                    printf("\nDesideri scegliere un altro giocatore? Inserendo no, tornerai al menu principale\n"
                           "%c/%c) ", PROMPT_CONFERMA, PROMPT_RIFIUTA);
                    confermaAzione = getchar();
                    if(confermaAzione == PROMPT_RIFIUTA) {
                        printf("\nTorno al menu principale!");
                        return;
                    }
                }
            } while (!confermaCarta);

            printf("\nSi dia inizio al duello tra %s e %s!", giocatore->nomeUtente, giocatori[indiceGiocatoreScelto].nomeUtente);
            printf("\nEstrarrò una carta 'Bang!' una volta a testa per ciascun giocatore. Il primo a finirle perderà un punto vita!");

            do {
                // è il turno del giocatore sfidato
                if(turnoDuello % 2 == 0) {
                    cartaBangDaScartare = cercaCartaMazzoPerNome(giocatori[indiceGiocatoreScelto].carteMano, "Bang!");
                    vittoriaSfidante = cartaBangDaScartare == -1;
                    if(vittoriaSfidante) {
                        printf("\n%s scarta una carta Bang! Ora tocca allo sfidante.", giocatori[indiceGiocatoreScelto].nomeUtente);
                        rimuoviCartaMazzo(&giocatori[indiceGiocatoreScelto].carteMano,
                                          giocatori[indiceGiocatoreScelto].carteMano.carte[cartaBangDaScartare]);
                    }
                // è il turno dello sfidante
                } else {
                    cartaBangDaScartare = cercaCartaMazzoPerNome(giocatore->carteMano, "Bang!");
                    vittoriaSfidato = cartaBangDaScartare == -1;
                    if(vittoriaSfidato) {
                        printf("\n%s scarta una carta Bang! Ora tocca al giocatore sfidato.", giocatore->nomeUtente);
                        rimuoviCartaMazzo(&giocatori[indiceGiocatoreScelto].carteMano,
                                          giocatori[indiceGiocatoreScelto].carteMano.carte[cartaBangDaScartare]);
                    }
                }
                turnoDuello++;
            } while(!vittoriaSfidante && !vittoriaSfidato);

            if(vittoriaSfidante) {
                vincitoreDuello = giocatore;
            } else {
                vincitoreDuello = &giocatori[indiceGiocatoreScelto];
            }
            printf("%s ha esaurito le carte Bang!, e pertanto perde il duello!", vincitoreDuello->nomeUtente);
            rimuoviPuntiVita(vincitoreDuello, 3);
        } else if(strcmp(carta.nomeCarta, "Emporio") == 0) {
            printf("\nLa carta 'Emporio' estrae dal mazzo di pesca un numero di carte pari a quello dei giocatori in vita,"
                   "per poi permettere ad ogni utente (a partire da chi ha giocato la carta) di pescare una carta da questo mazzo.");
            int j, k, giocatoriInVita, cartaScelta, nCarteEstratte = 0;
            Carta* carteEstratte = NULL; // array dinamico contenente le carte estratte dal mazzo di pesca
            Carta* carteEstratteTmp = NULL; // array utilizzato quando viene rimossa una carta dal mazzo di sopra

            // calcolo dei giocatori in vita
            for(j = 0; j < nGiocatori; j++) {
                if(giocatori[j].puntiVita > 0)
                    giocatoriInVita++;
            }
            printf("\nEssendoci %d giocatori in vita, saranno pescate %d carte.", giocatoriInVita, giocatoriInVita);

            // creazione dell'array dinamico contenente tante carte estratte quanti sono i giocatori in vita
            carteEstratte = (Carta*) calloc(giocatoriInVita, sizeof(Carta));
            if(carteEstratte == NULL) {
                printf("\nErrore: impossibile allocare dinamicamente"); // TODO: 'utils'
                exit(-1);
            }
            // e allocazione della memoria sufficiente per l'array in cui inserire temporaneamente le carte che non sono state selezionate
            // di dimensione giocatoriInVita - 1, dato che si può estrarre solo una carta alla volta per giocatore
            carteEstratteTmp = (Carta*) calloc(giocatoriInVita - 1, sizeof(Carta));
            if(carteEstratteTmp == NULL) {
                printf("\nErrore: impossibile allocare dinamicamente"); // TODO: 'utils'
                exit(-1);
            }

            // popolazione dell'array dinamico appena dichiarato
            carteEstratte = scartaCimaMazzo(mazzoCarte, giocatoriInVita);

            // a partire dal giocatore che ha estratto la carta, chiedo di pescarne una da quelle appena ottenute
            j = posizioneGiocatore;
            while(nCarteEstratte < giocatoriInVita) {
                printf("\n%s, è il tuo turno di pescare!", giocatori[j].nomeUtente);
                if(giocatori[j].carteMano.numeroCarte == giocatori[j].puntiVita) {
                    printf("\nMa prima devi scegliere una carta da scartare.");
                    scartaCarta(&giocatori[j]);
                }

                printf("\nScegli una carta da pescare:");
                for(k = 0; k < giocatoriInVita - nCarteEstratte; k++) {
                    printf("%d) %s", k+1, carteEstratte[k].nomeCarta);
                }
                do {
                    printf("\nInserisci il numero della carta da pescare: \n"
                           "?) ");
                    scanf("%d", &cartaScelta);
                    if(cartaScelta <= 0 || cartaScelta > giocatoriInVita)
                        printf("\nIl valore inserito non è valido!");
                } while(cartaScelta <= 0 || cartaScelta > giocatoriInVita);
                printf("\nBene! Hai scelto di pescare una carta '%s'.", carteEstratte[cartaScelta - 1].nomeCarta);

                // rimozione della carta dal mazzetto pescato e aggiunta alla mano del giocatore
                for(k = 0; k < giocatoriInVita - nCarteEstratte; k++) {
                    if(k != cartaScelta - 1)
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
                carteEstratteTmp = (Carta*) calloc(giocatoriInVita - nCarteEstratte, sizeof(Carta));
                // uso il modulo per tornare all'inizio nel caso in cui dovessi aver raggiunto la fine dell'array giocatori
                j = (j + 1) % nGiocatori;
            }
            free(carteEstratte);
            free(carteEstratteTmp);
            printf("\nEstrazione terminata!");
        } else if(strcmp(carta.nomeCarta, "Indiani") == 0) {
            int j, posizioneCartaBang;

            printf("\nAttacco di indiani in arrivo! Ogni giocatore (eccetto chi ha giocato la carta) deve giocare"
                   "una carta 'Bang!', oppure perdere un punto vita!");
            for(j = 0; j < nGiocatori; j++) {
                if(j != posizioneGiocatore && giocatori[j].puntiVita > 0) {
                    posizioneCartaBang = cercaCartaMazzoPerNome(giocatori[j].carteMano, "Bang!");
                    if(posizioneCartaBang == -1) {
                        printf("\n%s non possiede una carta 'Bang!', e perde quindi un punto vita!", giocatori[j].nomeUtente);
                        rimuoviPuntiVita(&giocatori[j], 1);
                    } else {
                        printf("\n%s scarta una carta 'Bang!' ed evita l'attacco degli indiani!", giocatori[j].nomeUtente);
                        rimuoviCartaMazzo(&giocatori[j].carteMano, giocatori[j].carteMano.carte[posizioneCartaBang]);
                    }
                }
            }
        }
    // TERZA PARTE: armi
    } else if(carta.tipologiaCarta == ARMA) {

    // QUARTA PARTE: carte in gioco
    } else if(carta.tipologiaCarta == EFFETTO) {

    }
}

void pescaCarte(Mazzo* mazzoCarte, Giocatore* giocatore, int nCarte) {
    int i;
    char tipologiaCartaPescata[TIPOLOGIA_CARTA_LEN], semeCartaPescata[SEME_CARTA_LEN];
    Carta cartaPescata;

    for(i = 0; i < nCarte; i++) {
        if(giocatore->carteMano.numeroCarte == giocatore->puntiVita) {
            printf("\nHai già %d carte! Scegli una carta da scartare.", giocatore->puntiVita);
            scartaCarta(giocatore);
        }
        cartaPescata = scartaCimaMazzo(mazzoCarte, 1)[0];
        prendiTipologiaCarta(cartaPescata.tipologiaCarta, tipologiaCartaPescata);
        prendiSemeCarta(cartaPescata.semeCarta, semeCartaPescata);
        printf("\nHai pescato una carta %s: '%s'! (%d di %s)", tipologiaCartaPescata, cartaPescata.nomeCarta, cartaPescata.numeroCarta, semeCartaPescata);
    }
}

void scartaCarta(Giocatore* giocatore) {
    int i, cartaScelta;
    bool ripetiCiclo = false;

    printf("\nScegli una carta da scartare:");
    for(i = 0; i < giocatore->carteMano.numeroCarte; i++) {
        printf("%d) %s", i+1, giocatore->carteMano.carte[i].nomeCarta);
    }
    do {
        printf("?) ");
        scanf("%d", &cartaScelta);
        if(ripetiCiclo)
            printf("\nInserisci un numero tra %d e %d!\n", 1, giocatore->carteMano.numeroCarte);
        ripetiCiclo = cartaScelta <= 0 || cartaScelta > giocatore->carteMano.numeroCarte;
    } while(ripetiCiclo);

    // rimuovo la carta dalla mano del giocatore
    rimuoviCartaMazzo(&giocatore->carteMano, giocatore->carteMano.carte[cartaScelta - 1]);
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