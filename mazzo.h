#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include "carta.h"
#include "utils.h"

#define MEZZO_SEPARATORE "-----------------"
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
void scambiaPescaScarti(Mazzo* mazzoPesca, Mazzo* mazzoScarti);
void mostraCarta(Carta carta);
void mostraCarteMazzo(Mazzo mazzo);
Carta estraiCarta(Mazzo *mazzoPesca, Mazzo *mazzoScarti);
void rimuoviCartaMazzo(Mazzo* mazzo, int posizioneCarta);
void aggiungiCartaMazzo(Mazzo* mazzo, Carta carta);
void spostaCartaMazzo(Mazzo* partenza, Mazzo* destinazione, int posizioneCarta);
int cercaCartaMazzoPerNome(Mazzo mazzo, char nomeCarta[]);