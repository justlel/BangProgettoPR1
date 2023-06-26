#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include "savegame.h"
#include "utils.h"


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
/**
 * Caratteri che l'utente può inserire in un prompt per confermare/rifiutare una richiesta del programma.
 */
#define PROMPT_CONFERMA 'y'
#define PROMPT_RIFIUTA 'n'
/**
 *
 */
#define PROMPT_TURNO_GIOCA_CARTA 1
#define PROMPT_TURNO_VEDI_CARTE_MANO 2
#define PROMPT_TURNO_VEDI_CARTE_GIOCO 3
#define PROMPT_TURNO_VEDI_DISTANZE 4
#define PROMPT_TURNO_VEDI_CARTE_GIOCO_ALTRI 5
#define PROMPT_TURNO_PASSA_TURNO 6
#define PROMPT_TURNO_ESCI 7

void avvioGioco();

Salvataggio caricaPartita();

Salvataggio creaPartita();

void assegnaRuoli(Giocatore* giocatori, int nGiocatori);

// TODO: spostarlo
void distribuisciCartePartenza(Mazzo *mazzoPesca, Mazzo *mazzoScarti, Giocatore *giocatori, int nGiocatori);

bool verificaCarteInGioco(Mazzo* mazzoPesca, Mazzo* mazzoScarti, int posizioneGiocatore, Giocatore* giocatori, int nGiocatori);

void avviaPartita(Salvataggio partita);
bool partitaTerminata(Salvataggio partita, Ruoli* ruoloVincitore);
void chiudiPartita(Ruoli ruoloVincitore);

void chiudiGioco();