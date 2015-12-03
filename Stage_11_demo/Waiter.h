#ifndef WAITER_H
#define WAITER_H

#define WAITER_ID 7

#include "stdafx.h"
#include <Component.h>

namespace stage_11{
	/** Pelioliokomponentti, joka simuloi raskasta, hyvin rinnakkaistuvaa laskentaa etsimällä alkulukuja*/
	class Waiter : public Component{
	public:
		/** Luo uuden Waiter-komponentin. Katso oikea käyttö yliluokasta.
		@param owner	Viite tämän komponentin omistavaan peliolioon
		@param limit	Se luku, jota pienemmät alkukuvut etsitään joka ruudunpäivityksen aikana
		*/
		Waiter(GameObject& owner, int limit): Component(owner), limit(limit){}
		/** Laskee alkulukuja tiettyyn raja-arvoon asti
		@param elapsedMS	Edellisestä ruudunpäivityksestä kulunut aika
		*/
		virtual void doUpdate(float elapsedMS){
			for (int i = 3; i <= limit; i++){
				for (int j = 2; j < i; j++){
					if (i % j == 0) break; //alkuluku löydetty
				}
			}
		}
		/** Palauttaa Waiter-komponentin komponenttitunnuksen
		@returns	Waiter-komponentin tunnusluku
		*/
		virtual int id(){ return WAITER_ID; }
	private:
		/** Se luku, jota pienemmät alkuluvut etsitään jokaisen ruudunpäivityksen aikana
		*/
		int limit;
	};
}
#endif