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

// gittata delle armi
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

// Macro contenenti i nomi delle carte presenti nel mazzo da caricare
#define CARTA_BANG "Bang!"
#define CARTA_BIRRA "Birra"
#define CARTA_CATBALOU "CatBalou"
#define CARTA_DILIGENZA "Diligenza"
#define CARTA_GATLING "Gatling"
#define CARTA_MANCATO "Mancato"
#define CARTA_PANICO "Panico!"
#define CARTA_SALOON "Saloon"
#define CARTA_WELLSFARGO "WellsFargo"
#define CARTA_INDIANI "Indiani"
#define CARTA_DUELLO "Duello"
#define CARTA_EMPORIO "Emporio"
#define CARTA_REMINGTON "Remington"
#define CARTA_CARABINA "Carabina"
#define CARTA_SCHOFIELD "Schofield"
#define CARTA_WINCHESTER "Winchester"
#define CARTA_VOLCANIC "Volcanic"
#define CARTA_COLT45 "Colt. 45"
#define CARTA_BARILE "Barile"
#define CARTA_DINAMITE "Dinamite"
#define CARTA_MIRINO "Mirino"
#define CARTA_MUSTANG "Mustang"
#define CARTA_PRIGIONE "Prigione"


/**
 * Struct 'Carta', che comprende tutte le informazioni relative
 * a una carta utilizzabile in gioco:
 * Una stringa contenente il nome di una carta (di lunghezza max. pari a 'NOME_CARTA_LEN')
 * La tipologia della carta, ossia uno dei valori dell'enum 'tipologieCarta'
 * Un intero che indica il numero della carta, compreso tra i numeri 1 e 13
 * Il seme della carta, ossia uno dei valori dell'enum 'semiCarta'
 */
typedef struct {
    char nomeCarta[NOME_CARTA_LEN + 1];
    tipologieCarta tipologiaCarta;
    int numeroCarta;
    semiCarta semeCarta;
} Carta;

void prendiTipologiaCarta(tipologieCarta tipologiaCarta, char nomeTipologia[]);
void prendiSemeCarta(semiCarta semeCarta, char nomeSeme[]);
void svuotaSchermo();