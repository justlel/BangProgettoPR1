#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include "savegame.h"
#include "utils.h"


/**
 * Numero minimo di giocatori per partita.
 */
#define MIN_PLAYERS 4
/**
 * Numero massimo di giocatori per partita.
 */
#define MAX_PLAYERS 7

/**
 * Nei prompt in cui vengono presentate delle scelte al giocatore, l'inserimento di questo carattere (dove possibile)
 * comporta l'uscita dal gioco, oppure dall'azione correntemente in esecuzione.
 */
#define PROMPT_EXIT 'E'
/**
 * Carattere da inserire nel prompt di avvio per scegliere di caricare un salvataggio
 */
#define PROMPT_CARICA_PARTITA 'C'
/**
 * Carattere da inserire nel prompt di avvio per scegliere di creare un salvataggio
 */
#define PROMPT_NUOVA_PARTITA 'N'


// Numeri rappresentanti una scelta all'interno del prompt del turno per un'azione da fare
#define PROMPT_TURNO_GIOCA_CARTA 1
#define PROMPT_TURNO_VEDI_CARTE_MANO 2
#define PROMPT_TURNO_VEDI_CARTE_GIOCO 3
#define PROMPT_TURNO_VEDI_DISTANZE 4
#define PROMPT_TURNO_VEDI_CARTE_GIOCO_ALTRI 5
#define PROMPT_VEDI_VITA_ALTRI 6
#define PROMPT_TURNO_PASSA_TURNO 7
#define PROMPT_TURNO_ESCI 8


void avvioGioco();
Salvataggio caricaPartita();
Salvataggio creaPartita();

void assegnaRuoli(Giocatore* giocatori, int nGiocatori);
void distribuisciCartePartenza(Mazzo *mazzoPesca, Mazzo *mazzoScarti, Giocatore *giocatori, int nGiocatori);

void avviaPartita(Salvataggio partita);
bool verificaCarteInGioco(Mazzo* mazzoPesca, Mazzo* mazzoScarti, int posizioneGiocatore, Giocatore* giocatori, int nGiocatori);
bool partitaTerminata(Salvataggio partita, Ruoli* ruoloVincitore);
void chiudiPartita(Ruoli ruoloVincitore);
void chiudiGioco(Salvataggio* partita);
void liberaStrutture(Salvataggio* partita);