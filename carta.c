#include "carta.h"


/**
 * Subroutine che converte la tipologia di una carta nella stringa corrispondente al suo nome.
 *
 * @param tipologiaCarta La tipologia della carta.
 * @param nomeTipologia Un puntatore a una stringa in cui inserire il nome della tipologia della carta.
 */
void prendiTipologiaCarta(tipologieCarta tipologiaCarta, char nomeTipologia[TIPOLOGIA_CARTA_LEN + 1]) {
    switch (tipologiaCarta) {
        case ISTANTANEA:
            strcpy(nomeTipologia, "Istantanea");
            break;
        case ISTANTANEA_SPECIAL:
            strcpy(nomeTipologia, "Istantanea Special");
            break;
        case ARMA:
            strcpy(nomeTipologia, "Arma");
            break;
        case EFFETTO:
            strcpy(nomeTipologia, "In gioco");
            break;
    }
}

/**
 * Subroutine che converte il seme di una carta nella stringa corrispondente al suo nome.
 *
 * @param semeCarta Il seme della carta.
 * @param nomeSeme Un puntatore a una stringa in cui inserire il nome del seme della carta.
 */
void prendiSemeCarta(semiCarta semeCarta, char nomeSeme[SEME_CARTA_LEN + 1]) {
    switch (semeCarta) {
        case PICCHE:
            strcpy(nomeSeme, "Picche");
            break;
        case CUORI:
            strcpy(nomeSeme, "Cuori");
            break;
        case QUADRI:
            strcpy(nomeSeme, "Quadri");
            break;
        case FIORI:
            strcpy(nomeSeme, "Fiori");
            break;
    }
}