/**
 SECONDO ASSEGNAMENTO
 *@code Station.h
 *@author Carlotta Schiavo, matr: 1217866
 *@version 1.0
 */

#include <iostream>

 /*INVARIANTI:
 * - type: può assumere solamente i valori:
 *		- 0: stazione principale (Main_station)
 *		- 1: stazione locale (Local_station)
 */

/*Classe di base*/
class Station {
	protected:
		std::string name; //nome della stazione
		int km;//kilomentri che separano la stazione dalla stazione d'origine
		int type;//tipo della stazione

	public:
		/**
		* getName
		* @return nome stazione
		*/
		std::string getName()const{ return name; }

		/**
		* getKm
		* @return km che separano la stazione dalla stazione d'origine
		*/
		int getKm()const{ return km; }

		/**
		 * getType
		 * @return tipo della stazione
		 */
		int getType()const{ return type; }

		/**
		* setName
		* @param nome stazione
		*/
		void setName(std::string n){ name = n; }

		/**
		* setKm
		* @param km che separano la stazione dalla stazione d'origine
		* (Suppongo che una stazione può cambiare il num di km che la 
		* separano dalla stazione d'origine se un giorno di decidesse di spostarla)
		*/
		void setKm(int kmm) { km = kmm; }

		/**
		* setType
		* @param tipo della stazione
		* (suppongo che una stazione possa diventare da locale a principale o vicerversa)
		*/
		void setType(int t) { type = t; }
	
		
};//Station

/*Classi derivate */
class Main_station: public Station {
	public:
		/**
		 * costruttore
		 * @param n, nome della stazione;
		 * @param kmm, km che separano la stazione dalla stazione d'origine
		 */
		Main_station(std::string n,int kmm);

		/**
		 * costruttore di copia
		 * @param s, riferimento a oggetto Station
		 */
		Main_station(const Main_station& s);
		/**
		 * distruttore
		 * Priva delle sue funzionalità l'oggetto Main_station
		 */
		~Main_station();
		
};

class Local_station : public Station {
	public:
		/**
		 * costruttore
		 * @param n, nome della stazione;
		 * @param kmm, km che separano la stazione dalla stazione d'origine
		 */
		Local_station(std::string n, int kmm);

		/**
		 * costruttore di copia
		 * @param s, riferimento a oggetto Station
		 */
		Local_station(const Local_station& s);
		/**
		 * distruttore
		 * Priva delle sue funzionalità l'oggetto Main_station
		 */
		~Local_station();

};



