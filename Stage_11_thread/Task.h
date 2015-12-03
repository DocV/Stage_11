#ifndef TASK_H
#define TASK_H

#include "stdafx.h"

#define TASK_EXECUTE virtual void operator() ()

namespace stage_11{
	/**Olio, joka määrittelee säiealtaan työtehtävän.
	Pelimoottorin toimiessa säiealtaan työntekijäsäikeet hakevat työtehtäviä yksi kerrallaan ja suorittavat ne.*/
	class Task{
	public:
		/** Metodi, joka suorittaa työtehtävän laskennan*/
		TASK_EXECUTE = 0;
	};
}
#endif