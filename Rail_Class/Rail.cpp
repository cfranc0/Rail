/**
 SECONDO ASSEGNAMENTO
 *@code Rail.cpp
 *@author Carlotta Schiavo, matr: 1217866
 *@version 1.0
 */
#include "Rail.h"
using namespace std;

void Rail::loadLine(string ld){
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
		if(num[1]==0){
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
}//loadLine

void Rail::loadTimeTable(string tt) {
	//Apertura file
	string fileName = tt;
	ifstream ist{ fileName };
	if (!ist) throw exception{ "File non trovato" };

	string s = " ";//linea appena letta
	int tmp = 0;//varibile di supporto

	while (getline(ist, s)) {
		istringstream istd(s);
		vector<int>token(stationsList.size());//in questo modo ciascun elemento del vettore si inizializza a zero
		int count = 0;
		//riempimento vettore con token linea appena letta
		while (istd >> tmp) {
			token[count] = tmp;
			count++;
		}

		int ID = token[0];
		int direc = token[1];
		int train_type = token[2];
		int departure_t = token[3];

		//stationsList
		list<Station>::iterator it;
		int i = 0;
		int count_stat = 3;
		list<int>timeTablelist;

		//controllo stazione d'origine
		/*it = stationsList.begin();
		int arrival_t = token[count_stat];
		int duration_min = departure_t - arrival_t;
		int duration_hour = duration_min/ 60;
		int distance_km = it->getKm();
		int time = calculateTime(train_type, it->getType(), arrival_t, duration_hour, distance_km, 0);
		if (time < duration_min) {
			timeTablelist.push_back(time);
		}
		else {
			timeTablelist.push_back(duration_min);
		}
		next(it);
		while (it != prev(stationsList.end())) {
			arrival_t = token[count_stat];
			duration_min = departure_t - arrival_t;
			duration_hour = duration_hour / 60;
			distance_km = it->getKm();

			time = calculateTime(train_type, it->getType(), arrival_t, duration_hour, distance_km, 1);
			if (time < duration_min) {
				timeTablelist.push_back(time);
			}
			else {
				timeTablelist.push_back(duration_min);
			}
			next(it);
			count_stat++;
		}
		//stazione arrivo
		arrival_t = token[count_stat];
		duration_min = departure_t - arrival_t;
		duration_hour = duration_hour / 60;
		distance_km = it->getKm();
		time = calculateTime(train_type, it->getType(), arrival_t, duration_hour, distance_km, 0);
		if (time < duration_min) {
			timeTablelist.push_back(time);
		}
		else {
			timeTablelist.push_back(duration_min);
		}
		Train* t = createTrain(train_type, ID, direc, timeTablelist);
		trainsList.push_back(t);*/
	}//while
}

/*int Rail::calculateTime(int train_t, int station_t, int arr_t, int dur_h, int dist_km,int isTerminal){
	int time = 0;
	const int STOP_DUR = 5 / 60; //5min/60
	int max_sp = getMaxSpeedFromType(train_t);

	if ((train_t == 0 || ((train_t == 2 || train_t == 3) && station_t==0)) && isTerminal==1) {
		time = ((10 / 80) + ((dist_km - 10) / max_sp) + (STOP_DUR))*60;
	}
	else{
		time = ((10 / 80) + ((dist_km - 10) / max_sp));
	}

	return time;
}*/

/*int Rail::getMaxSpeedFromType(int train_t) {
	if (train_t == 0) {
		return 160;
	}
	else if (train_t == 1) {
		 return 240;
	}
	return 300;
}*/

/*Train* Rail::createTrain(int type, int ID, int direc, list<int>& time) {
	if (type == 1) {
		Regional t(ID,direc,time);
		Train* tr = &t;
		return tr;
	}
	else if (type == 2) {
		High_speed t(ID,direc,time);
		Train* tr = &t;
		return tr;
	}
	High_speed_super t(ID, direc,time);
	Train* tr = &t;
	return tr;
	
}*/