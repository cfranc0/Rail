/**
 SECONDO ASSEGNAMENTO
 *@code Train.cpp
 *@author Carlotta Schiavo, matr: 1217866
 *@version 1.0
 */

#ifndef TRAIN_H
#define TRAIN_H
#include "Train.h"

/*Definizione funzione classe base*/

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
		if (s < 0 || s>160)throw std::exception{"Valore velocita' di crociera non conforme !"};
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
		if (s < 0 || s>240)throw std::exception{ "Valore velocita' di crociera non conforme !" };
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
		if (s < 0 || s>300)throw std::exception{ "Valore velocita' di crociera non conforme !" };
		speed = s;
	}

#endif //!TRAIN_H