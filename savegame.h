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
    char nomeSalvataggio[SAVEGAME_NAME_LEN + SAVEGAME_EXT_LEN + 1];
} Salvataggio;

Salvataggio caricaSalvataggio(char nomeSalvataggio[SAVEGAME_NAME_LEN + 1]);
void scriviSalvataggio(Salvataggio salvataggio, char nomeSalvataggio[SAVEGAME_NAME_LEN + 1]);

void aggiungiEstensioneSalvataggio(char nomeSalvataggio[SAVEGAME_NAME_LEN + 1], char nomeCompleto[SAVEGAME_NAME_LEN + SAVEGAME_EXT_LEN + 1]);

bool salvataggioEsistente(char nomeSalvataggio[SAVEGAME_NAME_LEN + 1]);