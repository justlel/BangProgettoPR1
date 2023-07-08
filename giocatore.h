#include "log.h"
#include "string.h"
#include "utils.h"

/**
 * Caratteri che l'utente può inserire in un prompt per confermare/rifiutare una richiesta del programma.
 */
#define PROMPT_CONFERMA 'y'
#define PROMPT_RIFIUTA 'n'

#define PROMPT_MAZZO_MANO 'M'
#define PROMPT_MAZZO_GIOCO 'G'

/**
 * Intero contenente la lunghezza massima del nome di un giocatore ('\0' escluso)
 */
#define NOME_UTENTE_LEN 49

/**
 * Intero contenente il numero totale dei ruoli assegnabili.
 */
#define ROLES_NUMBER 4

/**
 * Interi contenenti i numeri di punti vita dello sceriffo e degli altri ruoli.
 * Lo sceriffo è l'unico ruolo ad avere più punti vita degli altri giocatori, le cui "pallottole" sono segnate dalla
 * macro "PUNTI_VITA_GENERICO".
 */
#define PUNTI_VITA_SCERIFFO 5
#define PUNTI_VITA_GENERICO 4

/**
 * Lunghezza massima del nome di un ruolo per un giocatore ('\0' escluso)
 */
#define NOME_RUOLO_LEN_MAX 12

/**
 * Enumerazione contenente i vari ruoli che un giocatore
 * può assumere durante la partita. Il ruolo di un giocatore
 * è assegnato in base al numero dei partecipanti totali
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
    Mazzo carteMano;
    Mazzo carteGioco;
    int gittata;
} Giocatore;


// FUNZIONI SULLE DISTANZE TRA GIOCATORI
int* calcolaDistanzeGiocatori(int nGiocatori, Giocatore giocatori[nGiocatori], int posizioneGiocatore);
int calcolaDistanzaGiocatore(int nGiocatori, Giocatore giocatori[], int posizioneGiocatore1, int posizioneGiocatore2);
void mostraDistanze(int nGiocatori, Giocatore giocatori[nGiocatori], int posizioneGiocatore);

// FUNZIONI SULLE CARTE DEI GIOCATORI
// SUBROUTINE VISIVE
void mostraCarteInGiocoAltri(int nGiocatori, Giocatore giocatori[nGiocatori], int posizioneGiocatore);
// FUNZIONI
bool possiedeCartaInGioco(Giocatore giocatore, char nomeCarta[]);

bool giocaCarta(Mazzo *mazzoPesca, Mazzo *mazzoScarti, int nGiocatori, Giocatore giocatori[nGiocatori], int posizioneGiocatore, int posizioneCarta);

void pescaCarte(Mazzo *mazzoPesca, Mazzo *mazzoScarti, Giocatore *giocatore, int nCarte);
void scartaCarta(Giocatore *giocatore, Mazzo *mazzoDaScartare, Mazzo *mazzoScarti);

void aggiungiCartaInGioco(Giocatore* giocatoreCarta, Giocatore* giocatoreBersaglio, int posizioneCarta);
Carta prendiArmaGiocatore(Giocatore giocatore);

// FUNZIONI SUI GIOCATORI
void
rimuoviPuntiVita(Giocatore *giocatoreFerito, Giocatore *giocatoreAttaccante, int puntiVitaRimossi, Mazzo *mazzoPesca,
                 Mazzo *mazzoScarti);
void prendiNomeRuolo(Ruoli ruoli, char nomeRuolo[]);
int prendiVitaMassima(Giocatore giocatore);
int calcolaGittata(Giocatore giocatore);
void mostraVitaGiocatori(int nGiocatori, Giocatore giocatori[nGiocatori]);

bool esisteGiocatoreVicino(int nGiocatori, Giocatore giocatori[nGiocatori], int posizioneGiocatore, int distanza);

Giocatore *selezionaGiocatore(int nGiocatori, Giocatore giocatori[nGiocatori], int posizioneGiocatore, bool mostraDistanze, bool calcolaDistanze);
