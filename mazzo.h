#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include "carta.h"
#include "utils.h"

/**
 * Enumerazione contenente le varie tipologie che possono essere assegnate a un mazzo.
 */
typedef enum {MAZZO_PESCA, MAZZO_SCARTO, GIOC_MANO, GIOC_GIOCO} tipologieMazzo;

/**
 * Struct 'Mazzo', che comprende le informazioni relative a un mazzo di carte:
 * * La tipologia del mazzo, uno dei valori dell'enum 'tipologieMazzo'
 * * Un intero che indica il numero di carte contenute nel mazzo
 * * Un puntatore a un array dinamico di carte, contenente le carte presenti nel mazzo
 */
typedef struct {
    tipologieMazzo tipologiaMazzo;
    int numeroCarte;
    Carta* carte;
} Mazzo;

Mazzo caricaMazzo();

void mischiaMazzo(Mazzo* mazzo);

void mostraCarta(Carta carta);
void mostraCarteMazzo(Mazzo mazzo);

Carta *pescaCimaMazzo(Mazzo *mazzoPesca, Mazzo *mazzoScarti, int numeroCarteDaPescare);
Carta *estraiCarte(Mazzo *mazzoPesca, Mazzo *mazzoScarti, int numeroCarteDaEstrarre);

void rimuoviCartaMazzo(Mazzo* mazzo, Carta carta);
void aggiungiCartaMazzo(Mazzo* mazzo, Carta carta);

int cercaCartaMazzoPerNome(Mazzo mazzo, char nomeCarta[NOME_CARTA_LEN + 1]);