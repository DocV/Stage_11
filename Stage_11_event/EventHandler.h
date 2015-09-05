#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include "stdafx.h"
#include "Event.h"

namespace stage_11{
	/** Abstrakti rajapintaluokka, jonka toteuttavat oliot voivat rekisteröityä ottamaan vastaan viestejä tapahtumakanavilta
	*/
	class EventHandler{
	public:
		/** Käsittelee tapahtumajärjestelmän kautta saapuneen viestin
		@param e	Käsiteltävä viesti
		*/
		virtual void handleEvent(const Event& e) = 0;
	};
}

#endif