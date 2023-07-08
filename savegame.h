#include <stdbool.h>
#include "giocatore.h"

// -------- MACRO GENERALI --------
/**
 * Se definita, questa macro indica il nome del file di testo in cui sono contenuti, uno per riga,
 * i nomi dei file di salvataggio precedentemente creati. La path completa è "SAVEGAME_DIR/SAVEGAME_LIST_FILE",
 */
#define SAVEGAME_LIST_FILE "saves.txt"
/**
 * Lunghezza massima del nome di un file di salvataggio ('\0' e '.sav' escluso).
 */
#define SAVEGAME_NAME_LEN 16
/**
 * Nome e lunghezza dell'estensione dei file di salvataggio
 */
#define SAVEGAME_EXT ".sav"
#define SAVEGAME_EXT_LEN 4

/**
 * Struttura contenente le informazioni fondamentali relative a una partita, memorizzate in un file di salvataggio.
 * I campi di questa struttura vengono utilizzati per creare i file di salvataggio.
 */
typedef struct {
    Giocatore* giocatori;
    int nGiocatori;
    Mazzo mazzoPesca;
    Mazzo mazzoScarti;
    int prossimoGiocatore;
    char nomeSalvataggio[SAVEGAME_NAME_LEN + 1];
} Salvataggio;

Salvataggio caricaSalvataggio(char nomeSalvataggio[]);
void scriviSalvataggio(Salvataggio salvataggio, char nomeSalvataggio[]);

void aggiungiEstensioneSalvataggio(char nomeSalvataggio[], char nomeCompleto[]);

bool isSalvataggioInLista(char nomeSalvataggio[]);
void stampaSalvataggiInLista();
void aggiungiSalvataggioLista(char nomeSalvataggio[]);
bool salvataggioEsistente(char nomeSalvataggio[]);

bool fileEsistente(char* nomeFile);