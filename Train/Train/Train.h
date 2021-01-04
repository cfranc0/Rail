/**
 SECONDO ASSEGNAMENTO
 *@code Train.h
 *@author Carlotta Schiavo, matr: 1217866
 *@version 1.0
 */
#ifndef TRAIN_H
#define TRAIN_H
#include <iostream>
#include <List>

 /*Classe di base*/
class Train {
protected:
	int ID; //numero identificativo del treno
	int type;//tipo di treno: 0=Regional, 1=High_speed, 2= High_speed_super
	int track;//binario sul quale viaggia
	int direction;//direzione verso cui viaggia (0=partenza da origine, 1=partenza da capolinea)
	int speed;//velocità di crociera
	int km;//km della linea al quale si trova
	int delay;//ritardo
	bool exists;//(true=il treno sta ancora viaggiando, false=il treno arrivato a destinazione)
	std::list<int>timeTable;//lista che contiene gli orari di arrivo nelle varie stazioni

public:

	/*Eccezioni*/
	class InvalidTrackException {};
	class InvalidValueOfDelayEception {};
	class InvalidValueOfSpeedException {};
	class InvalidValueOfKmException {};
	/*metodi setter e getter che serviranno per la simulazione*/

	/**
	 * getID
	 * @return ID
	 */
	int getID()const { return ID; }
	/**
	 * getType
	 * @return type
	 */
	int getType()const { return type; }
	/**
	* getTrack
	* @return track
	*/
	int getTrack()const { return track; }

	/**
	 * getDirection
	 * @return direction
	 */

	int getDirection()const { return direction; }
	/**
	 * getSpeed
	 * @return speed
	 */
	int getSpeed()const { return speed; }
	/**
	* getKm
	* @return km
	*/
	int getKm()const { return km; }
	/**
	* getDelay
	* @return delay
	*/
	int getDelay()const { return delay; }
	/**
	* getExists
	* @return exists
	*/
	bool getExists()const { return exists; }

	/**
	* getTimeTable
	* @return timeTable
	*/
	std::list<int> getTimeTable()const { return timeTable; }

	/*toggleExists
	* nega lo stato del membro "exists"
	*/
	void toggleExists() { !exists; }

	/*setTrack
	* @param t, track
	* @throw InvalidTrackException, se il valore del binario è fuori dal range [0,6]
	*/
	void setTrack(int t);

	/*setDelay
	* @param d, delay
	* @throw InvalidValueOfDelayEception, se d < 0;
	*/
	void setDelay(int d);


	/*setKm
	* @param kmm, km
	* @throw InvalidValueOfKmException (se kmm<0)
	*/
	void setKm(int kmm);

	/**
	 * getMaxSpeed()
	 * @return la massima velocità che il treno può raggiungere
	 */
	virtual int getMaxSpeed()const = 0;

	/*setSpeed
	* @param s, speed
	* @throw InvalidValueOfSpeedException, se la velocità di crociera
	* e' fuori dal range specifico del tipo di treno
	*/
	virtual void setSpeed(int s) = 0;


};

/*Classi derivate*/

class Regional : public Train {
public:
	const int MAX_SPEED = 160;
	/**
	 * costruttore
	 * @param idd, id
	 * @param d, direction
	 * @param time, timeTable list
	 */
	Regional(const int id, int d, std::list<int>& time);

	/**
	 *distruttore
	 */
	~Regional();

	/**
	* getMaxSpeed()
	* @return la massima velocità che il treno può raggiungere
	*/
	int getMaxSpeed()const { return MAX_SPEED; }

	/*setSpeed
	*@param s, speed
	*@throw InvalidValueOfSpeedException (se s>160 || s<0)
	*/
	void setSpeed(int s);

};

class High_speed : public Train {
public:
	const int MAX_SPEED = 240;
	/**
	 * costruttore
	 * @param idd, id
	 * @param d, direction
	 * @param time, timeTable list
	 */
	High_speed(int id, int d, std::list<int>& time);

	/**
	 *distruttore
	 */
	~High_speed();

	/**
	* getMaxSpeed()
	* @return la massima velocità che il treno può raggiungere
	*/
	int getMaxSpeed()const { return MAX_SPEED; }

	/*setSpeed
	*@param s, speed
	*@throw InvalidValueOfSpeedException (se s>240 || s<0)
	*/
	void setSpeed(int s);
};

class High_speed_super : public Train {
public:
	const int MAX_SPEED = 300;
	/**
	 * costruttore
	 * @param idd, id
	 * @param d, direction
	 * @param time, timeTable list
	 */
	High_speed_super(int id, int d, std::list<int>& time);

	/**
	 *distruttore
	 */
	~High_speed_super();

	/**
	* getMaxSpeed()
	* @return la massima velocità che il treno può raggiungere
	*/
	int getMaxSpeed()const { return MAX_SPEED; }

	/*setSpeed
	*@param s, speed
	*@throw InvalidValueOfSpeedException (se s>300 || s<0)
	*/
	void setSpeed(int s);
};
std::ostream& operator<<(std::ostream& os, const Train& _line); //Mette a video un Treno
#endif //!TRAIN_H