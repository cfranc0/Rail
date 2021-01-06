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
	if (!ist) throw InvalidInputFile{};
	
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
	/*copio la lista dentro una lista di supporto per
	evitare di modificare la lista principale*/
	list<Station>myListOfStations = stationsList;

	//controllo che stationList sia piena
	if (stationsList.size() == 0) {
		throw EmptyStationListException{};
	}
	//Apertura file
	string fileName = tt;
	ifstream ist{ fileName };
	if (!ist) throw InvalidInputFile{};

	string s = " ";//linea appena letta
	int tmp = 0;//varibile di supporto
	int token_vec_size = myListOfStations.size() + 3;
	vector<int>whereMainStat = whereIsMainStation();

	/*ciclo che legge il file linea per linea*/
	while (getline(ist, s)) {
		istringstream istd(s);
		vector<int>token(token_vec_size);//in questo modo ciascun elemento del vettore si inizializza a zero
		int count = 0;

		//riempimento vettore con token linea appena letta
		while (istd >> tmp && count < 4) {
			token[count] = tmp;
			count++;
		}
		int ID = token[0];
		int direc = token[1];
		int train_type = token[2];
		int departure_t = token[3];
		
		//rovescia l'ordine della lista delle stazioni se la direzione del treno 
		//e' da capolinea a origine
		if (direc == 1) {
			myListOfStations=reverseStationList(myListOfStations);
			reverse(whereMainStat.begin(),whereMainStat.end());
		}
		vector<int>times(countMainStation());
		if (train_type == 2 || train_type == 3) {

			times[0] = tmp;
			int cnt = 1;
			while (istd >> tmp && cnt < times.size()) {
				times[cnt] = tmp;
				cnt++;
			}
			int k = 1;
			int j = 0;
			while (count < token_vec_size && j < times.size()) {
				if (whereMainStat[k] == 1) {
					token[count] = -1;
				}
				else {
					token[count] = times[j];
					j++;
				}
				count++;
				k++;
			}
		}
		else {
			token[count] = tmp;
			count++;
			while (istd >> tmp) {
				token[count] = tmp;
				count++;
			}

		}

		list<Station>::iterator it;//iteratore per la lista di stazioni

		/*count_stat--> tiene conto della stazione della quale stiamo verificando l'orario d'arrivo
		 parte da 3 perchè gli orari di arrivo partono da terzo elemento del vector
		che ho riempito con gli elementi letti da ciuscuna riga del file*/
		int count_stat = 3;
		list<int>timeTablelist;//lista degli orari (una per ciascun treno)
		int count_not_hour = 0;//varibile che serve a verificare se la linea analizzata della timetable ha degli orari mancanti;
		int arrival_t;
		int duration_min;
		int distance_km;
		int time;

		for (it = myListOfStations.begin(); it != myListOfStations.end() && count_stat < token_vec_size; ++it) {

			arrival_t = token[count_stat];
			duration_min = arrival_t - departure_t;

			if (arrival_t == -1) {

				time = 0;
				timeTablelist.push_back(time);
			}
			else {

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
					std::cout << ID << " Arriving time for station " << (count_stat - 3) << " change to: " << time << "\n";
				}
				else {
					time = duration_min + departure_t;
				}
				/*se ci sono stati orari mancanti, (quindi count_not_hour>0), vengono
				aggiunti 10min all'orario di arrivo al "capolinea" )*/
				if (count_stat == token_vec_size - 1 && count_not_hour > 0) {
					std::cout << "(..Addition of 10 min to the arrival time of the final station...)\n";
					time += 10;
				}

				timeTablelist.push_back(time);
			}
			count_stat++;
		}//for

		/*Creazione del treno ed inserimento in trainsList */

		Train* t;
		switch (train_type) {
		case 1:
			t = new Regional(ID, direc, timeTablelist);
			break;
		case 2:
			t = new High_speed(ID, direc, timeTablelist);
			break;
		case 3:
			t = new High_speed_super(ID, direc, timeTablelist);
			break;
		}

		trainsList.push_back(t);

		//DA ELIMINARE
		list<int>::iterator it_p;//iteratore per la lista di stazioni
		for (it_p = timeTablelist.begin(); it_p != timeTablelist.end(); ++it_p) {
			cout << *it_p<<" ";
		}
		cout << "\n\n";

	}//while
}


int Rail::calculateTime(int train_t, int station_t, int dist_km,int isTerminal){
	double time = 0;
	const double STOP_DUR = 5 / 60; //5min/60
	double max_sp = getMaxSpeedFromType(train_t);

	if ((train_t == 1 || ((train_t == 2 || train_t == 3) && station_t==0)) && isTerminal==1) {
		time = ((10 / 80) + ((dist_km - 10) / max_sp) + (STOP_DUR))*60;
	}
	else{
		time = ((10 / 80) + ((dist_km - 10) / max_sp))*60;
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

int Rail::countMainStation() {
	int count = 0;
	list<Station>::iterator it;
	for (it = stationsList.begin(); it != stationsList.end(); ++it) {
		if (it->getType() == 0)
			count++;
	}
	return count;
}

list<Station> Rail::getMainStationsList() {
	list<Station>mainStatList;
	list<Station>::iterator it;
	for (it = stationsList.begin(); it != stationsList.end(); ++it) {
		if (it->getType() == 0)
			mainStatList.push_back(*it);
	}
	return mainStatList;
}

Station Rail::nextStop(Train* t) {
	int km = t->getKm();
	list<Station> myListOfStation = stationsList;
	list<Station>::iterator it;

	if (t->getDirection() < 0)
		myListOfStation.reverse();

	it = myListOfStation.begin();

	while (it != myListOfStation.end() && ((it->getKm() <= km && t->getDirection() > 0) || (it->getKm() >= km && t->getDirection() < 0))) {
		++it;
	}
	return *it;
}
std::vector<int> Rail::whereIsMainStation() {
	vector<int>stat(stationsList.size());
	list<Station>::iterator it;
	int i = 0;
	for (it = stationsList.begin(); it != stationsList.end(); ++it) {
		if (it->getType() == 1)
			stat[i] = 1;
		i++;
	}
	return stat;
}

std::list<Station> Rail::reverseStationList(const list<Station>& l) {
	std::list<Station> myList = stationsList;
	myList.reverse();
	list<Station>::iterator i = myList.begin();
	int kmfromEndStat = i->getKm();
	list<Station>::iterator it;
	
	for (it = myList.begin(); it != myList.end(); ++it) {
		it->setKm(kmfromEndStat - (it->getKm()));
	}
	return myList;
}

//DA ELIMINARE
std::list<Station> Rail::provaReverse(const std::list<Station>& l) {
	return reverseStationList(l);
}