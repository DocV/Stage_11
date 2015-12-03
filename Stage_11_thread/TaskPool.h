#ifndef TASKPOOL_H
#define TASKPOOL_H

#include "stdafx.h"
#include "Task.h"
#include <list>
#include <mutex>
#include <condition_variable>
#include <algorithm>

namespace stage_11{
	/** Pelimoottorin pohjana toimiva säieallas*/
	class TaskPool{
	public:
		TaskPool& operator=(const TaskPool& other) = delete;
		TaskPool(const TaskPool& other) = delete;
		
		/** Hakee seuraavaksi suoritettavan työtehtävän.
		Jos suoritettavia työtehtäviä ei ole, jää odottamaan niiden saapumista.
		@returns	Osoitin suorittamattomaan työtehtävään. Tuhottava suorittamisen jälkeen deletellä.
		*/
		Task* pullTask();
		/** Lisää uuden työtehtävän suoritettavaksi.
		@param task	Osoitin suoritettavaan työtehtävään tai nullptr, jos säieallas on pysäytetty
		*/
		void pushTask(Task* task);
		/** Lisää uusia työtehtäviä suoritettavaksi.
		@param task	Lista, joka sisältää osoittimia suoritettaviin työtehtäviin
		*/
		void pushTaskList(std::list<Task*>& taskList);
		/** Odottaa, kunnes kaikki työtehtävät on suoritettu*/
		void waitForAllDone();
		/** Suorittaa jatkuvasti työtehtäviä, kunnes säieallas pysäytetään*/
		void work();
		/** Pysäyttää säiealtaan*/
		void terminate();
		/** Luo uuden säiealtaan
		@param threadCount	Työntekijäsäikeiden määrä
		*/
		TaskPool(unsigned int threadCount) : threadCount(threadCount){}
		/** Tuhoaa säiealtaan
		Kutsu aina terminate()-metodia ja odota säikeiden suorituksen loppumista ennen tuhoamista*/
		~TaskPool(){
			//Tuhotaan suorittamattomat työtehtävät
			std::for_each(tasks.begin(), tasks.end(), [](Task* t){delete t; });
		}
	private:
		/** Lista suorittamattomista työtehtävistä*/
		std::list<Task*> tasks;
		/** Ilmoittaa, että työtehtäviä on saatavillla*/
		std::condition_variable hasTasks;
		/** Ilmoittaa, että kaikki työtehtävät on suoritettu*/
		std::condition_variable allDone;
		/** Työtehtävälistaa suojaava lukko*/
		std::mutex taskListMutex;
		
		/** Onko säiealtaan suoritus pysäytetty*/
		bool terminated = false;
		/** Niiden työntekijäsäikeiden määrä, joilla ei ole mitään tekemistä*/
		unsigned int waitingThreads = 0;
		/** Työntekijäsäikeiden kokonaismäärä*/
		unsigned int threadCount;
	};
}
#endif