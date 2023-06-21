#include <string.h>


/**
 * Intero contenente la lunghezza massima del nome di una carta ('\0' escluso)
 */
#define NOME_CARTA_LEN 14
/**
 * Intero contenente la lunghezza massima della tipologia di una carta ('\0' escluso)
 */
#define TIPOLOGIA_CARTA_LEN 18
/**
 * Intero contenente la lunghezza massima del seme di una carta ('\0' escluso)
 */
#define SEME_CARTA_LEN 6

/**
 * Interi contenenti le gittate di tutte le armi
 */
#define GITTATA_CARABINA 4
#define GITTATA_REMINGTON 3
#define GITTATA_SCHOFIELD 2
#define GITTATA_WINCHESTER 5
#define GITTATA_VOLCANIC 1
#define GITTATA_COLT 1

/**
 * Enumerazione contenente i tipi che possono essere assegnati ad una carta.
 */
typedef enum {ISTANTANEA=1, ISTANTANEA_SPECIAL, ARMA, EFFETTO} tipologieCarta;

/**
 * Enumerazione contenente i possibili semi che possono essere assegnati a una carta.
 */
typedef enum {CUORI, QUADRI, FIORI, PICCHE} semiCarta;

/**
 * Struct 'Carta', che comprende tutte le informazioni relative
 * a una carta utilizzabile in gioco:
 * * Una stringa contenente il nome di una carta (di lunghezza max. pari a 'NOME_CARTA_LEN')
 * * La tipologia della carta, ossia uno dei valori dell'enum 'tipologieCarta'
 * * Un intero che indica il numero della carta, compreso tra i numeri 1 e 13
 * * Il seme della carta, ossia uno dei valori dell'enum 'semiCarta'
 */
typedef struct {
    char nomeCarta[NOME_CARTA_LEN+1];
    tipologieCarta tipologiaCarta;
    int numeroCarta;
    semiCarta semeCarta;
} Carta;

void prendiTipologiaCarta(tipologieCarta tipologiaCarta, char nomeTipologia[TIPOLOGIA_CARTA_LEN + 1]);
void prendiSemeCarta(semiCarta semeCarta, char nomeSeme[SEME_CARTA_LEN + 1]);