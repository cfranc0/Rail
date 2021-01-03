/**
 SECONDO ASSEGNAMENTO
 *@code Rail.cpp
 *@author Carlotta Schiavo, matr: 1217866
 *@version 1.0
 */
#include "Rail.h"
using namespace std;

void Rail::loadLine(string ld) {
	//Apertura file
	string fileName = ld;
	ifstream ist{ fileName };
	if (!ist) throw exception{ "File non trovato" };

	string s = " ";//linea appena letta
	string temp = "";//varibile di supporto
	int tmp = 0;//varibile di supporto

	//inserimento stazione d'origine
	string nameOrig = "";//nome della stazione d'origine
	getline(ist, nameOrig);
	Main_station mainStat(nameOrig, 0);
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
		if (num[1] == 0) {
			Main_station m(statName, num[0]);
			stationsList.push_back(m);
		}
		else {
			Local_station l(statName, num[0]);
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
	int token_vec_size = stationsList.size() + 3;
	while (getline(ist, s)) {
		istringstream istd(s);
		vector<int>token(token_vec_size);//in questo modo ciascun elemento del vettore si inizializza a zero
		int count = 0;

		//riempimento vettore con token linea appena letta
		while (istd >> tmp && count < token_vec_size) {
			token[count] = tmp;
			count++;
		}

		//inizializzazione variabili che serviranno per il calcolo dell'orario
		int ID = token[0];
		int direc = token[1];
		int train_type = token[2];
		int departure_t = token[3];//orario di partenza da stazione d'origine

		list<Station>::iterator it;//iteratore per la lista di stazioni
		int i = 0;

		/*count_stat--> tiene conto della stazione della quale stiamo verificando l'orario d'arrivo
		  parte da 3 perchè gli orari di arrivo partono da terzo elemento del vector
		  che o riempito con gli elementi letti da ciuscuna riga del file*/
		int count_stat = 3;
		list<int>timeTablelist;//lista degli orari (una per ciascun treno)

		int count_not_hour = 0;//varibile che serve a verificare se la timetable ha degli orari mancanti;
		int arrival_t;
		int duration_min;
		int duration_hour;
		int distance_km;
		int time;

		for (it = stationsList.begin(); it != stationsList.end() && count_stat < token_vec_size; ++it) {
			arrival_t = token[count_stat];
			//cout << arrival_t<<" ";
			duration_min = arrival_t - departure_t;

			//ogni volta che trova un orario mancante, incrementa la variabile che conta gli orari mancanti
			if (arrival_t == 0) {
				count_not_hour++;
			}

			distance_km = it->getKm();

			/*controlla se la stazione sia "terminale" o che si trovi lungo la linea*/
			if (count_stat == 3 || count_stat == token_vec_size - 1) {
				time = calculateTime(train_type, it->getType(), distance_km, 0);
			}
			else {
				time = calculateTime(train_type, it->getType(), distance_km, 1);
			}

			/*controllo della validità dell'orario di arrivo*/
			if (duration_min < time || arrival_t == 0) {
				time = time + departure_t;
			}
			else {
				time = duration_min + departure_t;
			}
			/*se ci sono stati orari mancanti, (quindi count_not_hour>0), vengono
			aggiunti 10min all'orario di arrivo al "capolinea" )*/
			if (count_stat == token_vec_size - 1 && count_not_hour > 0) {
				time += 10;
			}
			timeTablelist.push_back(time);
			count_stat++;
		}//for

		Train* t = createTrain(train_type, ID, direc, timeTablelist);
		trainsList.push_back(t);

	}//while

	cout << trainsList.size();
}


int Rail::calculateTime(int train_t, int station_t, int dist_km, int isTerminal) {
	double time = 0;
	const double STOP_DUR = 5 / 60; //5min/60
	double max_sp = getMaxSpeedFromType(train_t);

	if ((train_t == 1 || ((train_t == 2 || train_t == 3) && station_t == 0)) && isTerminal == 1) {
		time = ((10 / 80) + ((dist_km - 10) / max_sp) + (STOP_DUR)) * 60;
	}
	else {
		time = ((10 / 80) + ((dist_km - 10) / max_sp)) * 60;
	}

	return int(time);
}

double Rail::getMaxSpeedFromType(int train_t) {
	if (train_t == 1) {
		return 160;
	}
	else if (train_t == 2) {
		return 240;
	}
	return 300;
}

Train* Rail::createTrain(int type, int ID, int direc, list<int>& time) {
	if (type == 1) {
		Regional t(ID, direc, time);
		Train* tr = &t;
		return tr;
	}
	else if (type == 2) {
		High_speed t(ID, direc, time);
		Train* tr = &t;
		return tr;
	}
	High_speed_super t(ID, direc, time);
	Train* tr = &t;
	return tr;
}