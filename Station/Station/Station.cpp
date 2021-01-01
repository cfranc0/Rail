//DA_ELIMINARE ECCEZIONI IN CASO 


/**
 SECONDO ASSEGNAMENTO
 *@code Station.cpp
 *@author Carlotta Schiavo, matr: 1217866
 *@version 1.0
 */

#include "Station.h"
#include <exception> 
 
/*Definizione funzioni membro classe Main_station*/

//costruttore
Main_station::Main_station(std::string n, int kmm) {
	if (kmm < 0) throw std::exception{ "Il numero di km deve essere strettamente positivo !" };
	name = n;
	km = kmm;
	type = 0; //tipo 0 --> Stazione Principale
}

//costruttore di copia
Main_station::Main_station(const Main_station& s) {
	name = s.name;
	km = s.km;
	type = s.type;
}

//distruttore
Main_station::~Main_station() {
	name = "";
	km = -1;
	type = -1;
}



/*Definizione funzioni membro classe Local_station*/

//costruttore
Local_station::Local_station(std::string n, int kmm) 
{
	if (kmm < 0) throw std::exception{ "Il numero di km deve essere strettamente positivo !" };
	name = n;
	km = kmm;
	type = 1; //tipo 1 --> Stazione Locale
}

//costruttore di copia
Local_station::Local_station(const Local_station& s) {
	name = s.name;
	km = s.km;
	type = s.type;
}

//distruttore
Local_station::~Local_station() {
	name = "";
	km = -1;
	type = -1;
}

std::ostream& operator<<(std::ostream& os, const Station& s) {
	os << "Nome: " << s.getName()<<"\n";
	os << "Distanza da origine: " << s.getKm() << "\n";
	os << "Tipo: " << s.getType() << "\n";
	
	return os;
}