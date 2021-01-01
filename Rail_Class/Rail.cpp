/**
 SECONDO ASSEGNAMENTO
 *@code Rail.cpp
 *@author Carlotta Schiavo, matr: 1217866
 *@version 1.0
 */
#include "Rail.h"
using namespace std;

void Rail::loadLine(std::string ld){
	//Apertura file
	string fileName = ld;
	ifstream ist{ fileName };
	if (!ist) throw exception{"File non trovato"};
	
	string s = " ";//linea appena letta
	string temp = "";//varibile di supporto
	int tmp = 0;//varibile di supporto
	
	//inserimento stazione d'origine
	string nameOrig = "";//nome della stazione d'origine
	getline(ist, nameOrig);
	Main_station mainStat(nameOrig,0);
	stationsList.push_back(mainStat);
	
	while (getline(ist, s)) {
		
		istringstream istd(s);
		vector<string>pieceOfReadLine;
		vector<int>num;

		while (istd >> temp) {
			pieceOfReadLine.push_back(temp);
		}

		string statName = "";//nome della stazione
		for (int i = 0; i < (pieceOfReadLine.size() - 2); i++) {
			statName += pieceOfReadLine[i] + " ";
		}
		for (int i = pieceOfReadLine.size() - 2; i < pieceOfReadLine.size(); i++) {
			num.push_back(stoi(pieceOfReadLine[i]));
		}

		//creazione oggetti Station e inserimento in lista
		if(num[0]==0){
			Main_station m(statName,num[0]);
			stationsList.push_back(m);
		}
		else{
			Local_station l(statName,num[0]);
			stationsList.push_back(l);
		}
	}//while
	ist.close();
	line.generateLine(stationsList);
}