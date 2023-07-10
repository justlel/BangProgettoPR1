#include "utils.h"


void assertPuntatoreNonNull(void* puntatore, char* messaggioErrore) {
    if (puntatore == NULL) {
        printf("\n\n%s ERRORE %s", MEZZO_SEPARATORE, MEZZO_SEPARATORE);
        printf("%s", messaggioErrore);
        printf("\n%s ERRORE %s", MEZZO_SEPARATORE, MEZZO_SEPARATORE);
        exit(-1);
    }
}

void svuotaSchermo() {
    #ifdef _WIN64
        system("cls"); // pulizia su windows
    #elif __APPLE__
        system("clear"); // pulizia su macOS
    #elif __linux__
        system("clear"); // stessa pulizia, per linux
    #endif
}