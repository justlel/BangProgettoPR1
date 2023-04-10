#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "carta.h"

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

Carta* scartaCimaMazzo(Mazzo* mazzo, int daScartare);