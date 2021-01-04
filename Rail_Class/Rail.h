/**
 SECONDO ASSEGNAMENTO
 *@code Rail.h
 *@author Carlotta Schiavo, matr: 1217866
 *@version 1.0
 */


#ifndef RAIL_H
#define RAIL_H

#include "Train.h"
#include "Station.h"
#include "RailLine.h"
#include <iostream>
#include <fstream>
#include <sstream>  
#include <vector>
#include <algorithm>
#include <string>
#include <exception>

class Rail {
private:
	RailLine line;
	std::list<Station>stationsList;
	std::list<Train*>trainsList;

	/*funzioni di utilita'*/

	/**
	 * calculateTime
	 * calcola i tempi esatti che dovrebbe impiegare il treno ad arrivare ad un certa stazione.
	 * @param train_t, tipo del treno
	 * @param station_t, tipo della stazione
	 * @param dist_km, distanza dell' n-esima stazione dalla stazione d'origine
	 * @param isTerminal, 0=origine o capolinea, 1=stazione in mezzo alla linea;
	 * @return il tempo calcolato
	 */
	int calculateTime(int train_t, int station_t, int dist_km, int isTerminal);

	/**
	 * getMaxSpeedFrom
	 * @param type, tipo di treno
	 * @return Velocita' massimo raggiungibile da treno
	 */
	double getMaxSpeedFromType(int type);

	/**
	 * createTrain
	 * funzione che crea un treno (a seconda del tipo indicato)
	 * @param type
	 * @param ID
	 * @param direc
	 * @param time
	 * @return Train*
	 */
	Train* createTrain(int type, int ID, int direc, std::list<int>& time);
public:

	/*classi di eccezione*/
	class EmptyStationListException {};
	class InvalidInputFile {};
	/**
	 *loadLine
	 *funzione membro che legge da file i dati sulle stazioni
	 *crea gli oggetti stazione e li carica in stationsList.
	 *Riempita la lista, invoca il metodo "generateLine" della classe
	 *RailLine che andrà a generare la linea ferroviaria.
	 *@param ld, il file "line_description.txt"
	 *@throw InvalidInputFile, se non trova il file di input
	 */
	void loadLine(std::string ld);


	/**
	 *loadLine
	 *funzione membro che legge da file i dati della TimeTable.
	 *crea gli oggetti Treni e li carica in trainsList.
	 *Prima di creare l'oggetto Treno, la funzione controlla che i dati del file
	 *siano conformi, se necessario li modifica.
	 *
	 *@param ld, il file "line_description.txt"
	 *
	 *@throw InvalidInputFile, se non trova il file di input
	 *@throw EmptyStationListException, se la lista delle stazioni è vuota. 
	 *(Prima di invocare tale metodo e' necessario invocare il metodo loadLine
	 * il quale andra' a riempire la lista delle stazioni)
	 */
	void loadTimeTable(std::string tt);


	/**
	 * getStationsList
	 * @return stationsList
	*/
	std::list<Station> getStationsList()const { return stationsList; }
	/**
	 * getTrainsList
	 * @return trainsList
	*/
	std::list<Train*> getTrainsList()const { return trainsList; }

	/**
	 * getLine
	 * @return line
	*/
	RailLine getLine() const { return line; }

};
//overload operator<<
std::ostream& operator<<(std::ostream& os, const Rail& _rail);

#endif // RAIL_H





