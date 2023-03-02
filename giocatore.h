#include "carta.h"

/**
 * Intero contenente la lunghezza massima del nome di un giocatore ('\0' escluso)
 */
#define NOME_UTENTE_LEN 49

/**
 * Intero contenente il numero totale dei ruoli assegnabili.
 */
#define ROLES_NUMBER 4

/**
 * Lo sceriffo è l'unico ruolo ad avere più punti vita degli altri giocatori, le cui "pallottole" sono segnate dalla
 * macro "PUNTI_VITA_GENERICO".
 */
#define PUNTI_VITA_SCERIFFO 5
#define PUNTI_VITA_GENERICO 4

/**
 * Enumerazione contenente i vari ruoli che un giocatore
 * può assumere durante la partita. Il ruolo di un giocatore
 * è assegnato in base al numero dei partecipanti totali.
 */
typedef enum {SCERIFFO, VICESCERIFFO, FUORILEGGE, RINNEGATO} Ruoli;

/**
 * Struct 'Giocatore', che comprende
 * tutti i dati relativi a un partecipante:
 * * Una stringa contenente il nome del giocatore (di lunghezza max. pari a 'NOME_UTENTE_LEN')
 * * Il ruolo assegnato a un giocatore, ossia uno dei valori dell'enum 'Ruoli'
 * * Un intero, pari al numero dei punti vita (o 'pallottole') del giocatore
 * * Un puntatore a un array dinamico contenenti le carte in mano di un giocatore, del tipo 'Carta'
 * * Un puntatore a un array dinamico contenenti le carte in gioco di un giocatore, del tipo 'Carta'
 * * Un intero pari alla gittata attuale del giocatore, ossia la distanza massima a cui può sparare ad altri giocatori.
 */
typedef struct {
    char nomeUtente[NOME_UTENTE_LEN + 1];
    Ruoli ruoloAssegnato;
    int puntiVita;
    Carta* carteMano;
    Carta* carteGioco;
    int gittata;
} Giocatore;
