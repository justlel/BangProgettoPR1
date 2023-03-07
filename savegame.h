#include <stdbool.h>
#include "mazzo.h"

// -------- MACRO GENERALI --------
/**
 * Se definita, questa macro precisa che i file di salvataggio sono da ricercarsi
 * nella directory fornita.
 */
#define SAVEGAME_DIR "savegames"
/**
 * Lunghezza massima del nome di un file di salvataggio ('\0' escluso).
 */
#define SAVEGAME_NAME_LEN 16

/**
 * Struttura contenente le informazioni fondamentali relative a una partita, memorizzate in un file di salvataggio.
 * I campi di questa struttura vengono utilizzati per creare i file di salvataggio.
 */
typedef struct {
    char nomeSalvataggio[SAVEGAME_NAME_LEN + 1];
    Giocatore* giocatori;
    Mazzo mazzoPesca;
    Mazzo mazzoScarti;
    int prossimoGiocatore;
} Salvataggio;

Salvataggio caricaSalvataggio(char nomeSalvataggio[]);