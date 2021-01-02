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
		//DA TERMINARE
		/*funzioni di utilita'*/
		int calculateTime(int train_t, int station_t, int arr_t, int dur_h, int dist_km,int isTerminal);
		int getMaxSpeedFromType(int type);
		Train* createTrain(int type,int ID,int direc,std::list<int>& time);
	public:
		/**
		 *loadLine
		 *funzione membro che legge da file i dati sulle stazioni
		 *crea gli oggetti stazione e li carica in stationsList.
		 *Riempita la lista, invoca il metodo "generateLine" della classe
		 *RailLine che andrà a generare la linea ferroviaria.
		 * 
		 *@param ld, il file "line_description.txt"
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
		 */
		void loadTimeTable(std::string tt);

		//void simulate();

		/*FUNZIONI CHE NON SO SE POSSONO SERVIRE MA CHE INTANTO USO PER IL TEST*/
		std::list<Station> getStationsList() { return stationsList; }
		std::list<Train*> getTrainsList() { return trainsList; }

};

#endif // RAIL_H





