#include "mazzo.h"

#define LOG_FILE_NAME "log.txt"

void scriviCartePescateSuLog(char nomeGiocatore[], Mazzo mazzoMano, int nCarte);
void scriviCartaEstrattaSuLog(char nomeGiocatore[], Carta carta);

void scriviDinamiteSuLog(char nomeGiocatore[], Carta cartaEstratta);
void scriviPrigioneSuLog(char nomeGiocatore[], Carta cartaEstratta);

void scriviCartaGiocataAvversario(char nomeGiocatore[], char nomeAvversario[], Carta carta);
void scriviCartaGiocataSuLog(char nomeGiocatore[], Carta carta);

void scriviBarileSuLog(char nomeGiocatore[], Carta cartaEstratta);
void scriviMancatoSuLog(char nomeGiocatore[]);

void scriviPuntiVitaPersiSuLog(char nomeGiocatoreAttaccato[], char nomeGiocatoreAttaccante[], int puntiVitaPersi);
void scriviMorteGiocatoreSuLog(char nomeGiocatoreMorto[]);

void scriviCartaScartataSuLog(char nomeGiocatore[], Carta carta);

void scriviCartaRubataPanico(char nomeGiocatore[], char nomeAttaccato[], Carta cartaRubata);

void scriviInizioTurno(int turno);
void pulisciLog();