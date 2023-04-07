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
void giocaCarta(int nGiocatori, Giocatore giocatori[], int posizioneGiocatore, Carta carta) {
    int i;
    Giocatore giocatore = giocatori[posizioneGiocatore];
    char confermaAzione;

    // PRIMA PARTE: carte istantanee
    if(carta.tipologiaCarta == ISTANTANEA) {
        if(strcmp(carta.nomeCarta, "Bang!") == 0) {
            bool mirino = false, confermaSparo = true;
            int gittata = 0, distanza = 0, destinazioneSparo;

            int* distanze = calloc(nGiocatori, sizeof(int));
            if(distanze == NULL) {
                printf("\nImpossibile allocare dinamicamente memoria.");
                exit(-1);
            }

            printf("\nLa carta 'Bang!' ti permette di infliggere danni a un giocatore.");
            // calcolo della gittata
            printf("\nAl momento, la tua gittata totale è data da questi fattori:");
            for(int i = 0; i < giocatore.carteGioco.numeroCarte; i++) {
                // calcolo gittata data dall'arma
                if(giocatore.carteGioco.carte[i].tipologiaCarta == ARMA) {
                    if (strcmp(giocatore.carteGioco.carte[i].nomeCarta, "Carabina") == 0) {
                        printf("\nArma: CARABINA (+%d gittata)", GITTATA_CARABINA);
                        gittata += GITTATA_CARABINA;
                    } else if (strcmp(giocatore.carteGioco.carte[i].nomeCarta, "Remington") == 0) {
                        printf("\nArma: REMINGTON (+%d gittata)", GITTATA_REMINGTON);
                        gittata += GITTATA_REMINGTON;
                    } else if (strcmp(giocatore.carteGioco.carte[i].nomeCarta, "Schofield") == 0) {
                        printf("\nArma: SCHOFIELD (+%d gittata)", GITTATA_SCHOFIELD);
                        gittata += GITTATA_SCHOFIELD;
                    } else if (strcmp(giocatore.carteGioco.carte[i].nomeCarta, "Winchester") == 0) {
                        printf("\nArma: WINCHESTER (+%d gittata)", GITTATA_WINCHESTER);
                        gittata += GITTATA_WINCHESTER;
                    } else if (strcmp(giocatore.carteGioco.carte[i].nomeCarta, "Volcanic") == 0) {
                        printf("\nArma: VOLCANIC (+%d gittata)", GITTATA_VOLCANIC);
                        gittata += GITTATA_VOLCANIC;
                    }
                // calcolo gittata data da altre carte speciali
                } else if(giocatore.carteGioco.carte[i].tipologiaCarta == EFFETTO) {
                    if(strcmp(carta.nomeCarta, "Mirino") == 0) {
                        printf("\nCarta speciale: MIRINO (-1 distanza altri giocatori)");
                        mirino = true;
                    }
                }
            }
            // popolo l'array distanze con le distanze di ogni giocatore, così da non dover effettuare troppe volte il calcolo
            printf("\nConsiderate le carte a disposizione, questi sono i giocatori a cui puoi infliggere danno:");
            for(i = 0; i < nGiocatori; i++) {
                if(strcmp(giocatori[i].nomeUtente, giocatore.nomeUtente) == 0)
                    continue;
                distanze[i] = calcoloDistanza(giocatori, posizioneGiocatore, i);
                if(distanze[i] <= gittata) {
                    printf("\nGiocatore n° %d: %s. Distanza: %d", i+1, giocatori[i].nomeUtente, distanza);
                }
            }
            // chiedo all'utente a quale giocatore vorrebbe sparare, finché non conferma la sua scelta
            do {
                printf("\nInserisci la posizione del giocatore a cui vorresti sparare: ");
                do {
                    scanf("%d", &destinazioneSparo);
                    // il numero fornito deve essere valido
                    if(destinazioneSparo - 1 < 0 || destinazioneSparo - 1 >= nGiocatori) {
                        printf("\nInserisci un numero compreso tra 1 e %d: ", nGiocatori);
                    } else if(distanze[destinazioneSparo - 1] > gittata) {
                        printf("\nHey! Non puoi sparare a quel giocatore! Inserisci un'altra posizione:");
                    } else if(destinazioneSparo - 1 == posizioneGiocatore) {
                        printf("\nOvviamente non puoi sparare a te stesso... Inserisci un'altra posizione: ");
                    }
                } while (destinazioneSparo - 1 < 0 || destinazioneSparo - 1 >= nGiocatori ||
                        distanze[destinazioneSparo - 1] > gittata || destinazioneSparo - 1 == posizioneGiocatore);
                printf("\nStai per sparare a %s! Confermi la tua scelta?\n"
                       "%c/%c) ", giocatori[destinazioneSparo - 1].nomeUtente, PROMPT_CONFERMA, PROMPT_RIFIUTA);
                confermaAzione = getchar();
                confermaSparo = confermaAzione != PROMPT_RIFIUTA;
                if(!confermaSparo) {
                    printf("\nDesideri cambiare giocatore? Inserendo no, tornerai al menu principale."
                           "%c/%c) ", PROMPT_CONFERMA, PROMPT_RIFIUTA);
                    confermaAzione = getchar();
                    if(confermaAzione == PROMPT_RIFIUTA) {
                        printf("\nTorno al menu principale!");
                        free(distanze);
                        return;
                    }
                }
            } while(!confermaSparo);
            // rimozione del punto vita e fine della funzione
            printf("\n%s ha sparato a %s, che perde quindi un punto vita!");
            rimuoviPuntoVita(&giocatori[destinazioneSparo - 1]);
            free(distanze);
            return;
        } else if(strcmp(carta.nomeCarta, "Birra") == 0) {
            if((giocatore.ruoloAssegnato == SCERIFFO && giocatore.puntiVita == PUNTI_VITA_SCERIFFO) ||
               (giocatore.ruoloAssegnato != SCERIFFO && giocatore.puntiVita == PUNTI_VITA_GENERICO)
               ) {
                printf("\nHai già il massimo della vita!");
            } else {
                printf("\n%s ha utilizzato una birra e guadagna un punto vita!");
                giocatori[posizioneGiocatore].puntiVita++;
            }
            return;
        } else if(strcmp(carta.nomeCarta, "Diligenza") == 0) {
            printf("\n%s pesca 2 carte!");
            pescaCarte(&giocatori[posizioneGiocatore], 2);
        } else if(strcmp(carta.nomeCarta, "Panico!") == 0) {

        } else if(strcmp(carta.nomeCarta, "CatBalou!") == 0) {

        } else if(strcmp(carta.nomeCarta, "Mancato!") == 0) {

        } else if(strcmp(carta.nomeCarta, "Gatling") == 0) {

        } else if(strcmp(carta.nomeCarta, "Saloon") == 0) {

        } else if(strcmp(carta.nomeCarta, "WellsFargo") == 0) {

        }
    // SECONDA PARTE: carte istantanee speciali
    } else if(carta.tipologiaCarta == ISTANTANEA_SPECIAL) {

    // TERZA PARTE: armi
    } else if(carta.tipologiaCarta == ARMA) {

    // QUARTA PARTE: carte in gioco
    } else if(carta.tipologiaCarta == EFFETTO) {

    }
}

bool possiedeCartaInGioco(Giocatore giocatore, char nomeCarta[NOME_CARTA_LEN + 1]) {
    for(int i = 0; i < giocatore.carteGioco.numeroCarte; i++) {
        if(strcmp(nomeCarta, giocatore.carteGioco.carte[i].nomeCarta) == 0)
            return true;
    }
    return false;
}