#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include "mazzo.h"
#include "savegame.h"


// -------- MACRO RELATIVE ALLE IMPOSTAZIONI DELLA PARTITA --------
/**
 * Numero minimo e massimo di giocatori per partita.
 */
#define MIN_PLAYERS 4
#define MAX_PLAYERS 7

// -------- MACRO RELATIVE ALLE OPZIONI DEI PROMPT PER I GIOCATORI --------
/**
 * Nei prompt in cui vengono presentate delle scelte al giocatore, l'inserimento di questo carattere (dove possibile)
 * comporta l'uscita dal gioco, oppure dall'azione correntemente in esecuzione.
 */
#define PROMPT_EXIT 'E'
/**
 * All'avvio del gioco, il giocatore inserisce uno di questi due caratteri per scegliere se caricare un salvataggio
 * o creare una nuova partita.
 */
#define PROMPT_CARICA_PARTITA 'C'
#define PROMPT_NUOVA_PARTITA 'N'


void avvioGioco();

Salvataggio caricaPartita();

Salvataggio creaPartita();

void assegnaRuoli(Giocatore* giocatori, int nGiocatori);

void avviaPartita(Salvataggio partita);

void chiudiGioco();