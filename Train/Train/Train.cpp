/**
 SECONDO ASSEGNAMENTO
 *@code Train.cpp
 *@author Carlotta Schiavo, matr: 1217866
 *@version 1.0
 */


#include "Train.h"

/*Definizione funzione classe base*/

	/*setTrack
	* @param t, track
	* @throw exception, se il valore del binario è fuori dal range [0,6]
	*/
	void Train::setTrack(int t) {
		if (t < 0 || t > 6) throw std::exception{ "ATTENZIONE: numero binario non conforme" };
		track = t;
	}

	/*setDelay
	*@param d, delay
	*@throw execption, se d<0;
	*/
	void Train::setDelay(int d) {
		if(d<0) throw std::exception{ "ATTENZIONE: il ritardo non puo' essere negativo" };
		delay = d;
	}

	/*setKm
	*@param kmm, km
	*@throw exception (se kmm<0)
	*/
	void Train::setKm(int kmm) {
		if (kmm < 0)throw std::exception{"ATTENZIONE: km possono essere solo positivi"};
		km = kmm;
	}

/*Definizione funzioni classi derivate */

/*classe Regional*/

	//costruttore
	Regional::Regional(const int id, int d, std::list<int>& time){
		ID = id;
		type = 0;
		track = -1;
		direction = d;
		speed = 0;
		km = -1;
		delay = 0;
		exists = true;
		timeTable = time;
	}

	//distruttore
	Regional::~Regional() {
		ID = -1;
		type = -1;
		track = -1;
		direction = -1;
		speed = 0;
		km = -1;
		delay = 0;
		exists = false;
	}

	//setSpeed
	void Regional::setSpeed(int s) {
		if (s < 0 || s > MAX_SPEED)throw std::exception{"Valore velocita' di crociera non conforme !"};
		speed = s;
	}

/*classe High_speed*/

	//costruttore
	High_speed::High_speed(int id, int d, std::list<int>& time){
		ID = id;
		type = 1;
		track = -1;
		direction = d;
		speed = 0;
		km = -1;
		delay = 0;
		exists = true;
		timeTable = time;
	}

	//distruttore
	High_speed::~High_speed() {
		ID = -1;
		type = -1;
		track = -1;
		direction = -1;
		speed = 0;
		km = -1;
		delay = 0;
		exists = false;
	}

	//setSpeed
	void High_speed::setSpeed(int s) {
		if (s < 0 || s > MAX_SPEED)throw std::exception{ "Valore velocita' di crociera non conforme !" };
		speed = s;
	}

/*classe High_speed_super*/

	//costruttore
	High_speed_super::High_speed_super(int id, int d, std::list<int>& time) {
		ID = id;
		type = 2;
		track = -1;
		direction = d;
		speed = 0;
		km = -1;
		delay = 0;
		exists = true;
		timeTable = time;
	}

	//distruttore
	High_speed_super::~High_speed_super() {
		ID = -1;
		type = -1;
		track = -1;
		direction = -1;
		speed = 0;
		km = -1;
		delay = 0;
		exists = false;
	}

	//setSpeed
	void High_speed_super::setSpeed(int s) {
		if (s < 0 || s > MAX_SPEED)throw std::exception{ "Valore velocita' di crociera non conforme !" };
		speed = s;
	}

	std::ostream& operator<<(std::ostream& os, const Train& t) {
		os << "ID:" << t.getID()<<"\n";
		os << "Type:" << t.getType() << "\n";
		os << "Track:" << t.getTrack() << "\n";
		os << "Direction:" << t.getDirection() << "\n";
		os << "Speed:" << t.getSpeed() << "\n";
		os << "Km (where is):" << t.getKm() << "\n";
		os << "Delay:" << t.getDelay() << "\n";
		os << "Exists ?:" << t.getExists() << "\n";
		return os;
	}