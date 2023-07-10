#include "utils.h"


/**
 * Subroutine che verifica che un puntatore ad una porzione di memoria allocata non sia NULL.
 * Se il puntatore è NULL, il programma termina.
 *
 * @param puntatore Il puntatore da verificare.
 * @param messaggioErrore Il messaggio di errore da stampare a schermo se il puntatore è null.
 */
void assertPuntatoreNonNull(void* puntatore, char* messaggioErrore) {
    if (puntatore == NULL) {
        printf("\n\n%s ERRORE %s", MEZZO_SEPARATORE, MEZZO_SEPARATORE);
        printf("%s", messaggioErrore);
        printf("\n%s ERRORE %s", MEZZO_SEPARATORE, MEZZO_SEPARATORE);
        exit(-1);
    }
}

/**
 * Subroutine per svuotare lo schermo. Tiene conto del sistema operativo in utilizzo.
 */
void svuotaSchermo() {
    #ifdef _WIN64
        system("cls"); // pulizia su windows
    #elif __APPLE__
        system("clear"); // pulizia su macOS
    #elif __linux__
        system("clear"); // stessa pulizia, per linux
    #endif
}

/**
 * Subroutine per svuotare il buffer.
 */
void svuotaBuffer() {
    while (getchar() != '\n')
        continue;
}