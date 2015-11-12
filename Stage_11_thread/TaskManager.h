#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include "stdafx.h"
#include <iostream>
#include "TaskPool.h"
#include "LogManager.h"

/** Makro, jolla kirjoitetaan lokiin tavallinen lokiviesti
*/
#define LOGMSG(MSG) stage_11::TaskManager::getGlobalLogger().log(MSG);
/** Makro, jolla kirjoitetaan lokiin virheviesti
*/
#define LOGERR(MSG) stage_11::TaskManager::getGlobalLogger().logError(MSG);

namespace stage_11{
	/** Globaali singleton-olio, joka mahdollistaa työtehtävien lisäämisen säiealtaan suoritettavaksi.
	*/
	class TaskManager{
	public:
		/** Luo uuden työtehtävien hallintaolion
		@param threadcount	Säiealtaan työntekijäsäikeiden määrä
		*/
		TaskManager(unsigned int threadcount);
		/** Tuhoaa työntekijöiden hallintaolion
		*/
		~TaskManager();
		/** Lisää uuden työtehtävän säiealtaan suoritettavaksi
		@param t	Suoritettava työtehtävä
		*/
		static void pushTask(Task* t){
			singleton->tp.pushTask(t);
		}
		/** Lisää useita uusia työtehtäviä säiealtaan suoritettavaksi
		@param t	Suoritettavat työtehtävät
		*/
		static void pushTaskList(std::list<Task*>& t){
			singleton->tp.pushTaskList(t);
		}
		/** Hakee viitteen globaaliin työtehtävienhallintaolioon
		@returns	Viite globaaliin TaskManager-singletoniin
		*/
		static TaskManager& getSingleton(){
			return *singleton;
		}
		/** Hakee viitteen oletuslokiolioon
		@returns	Viite globaaliin lokiolioon
		*/
		static LogManager& getGlobalLogger(){
			return singleton->globalLogger;
		}
	private:
		/** Globaali TaskManager-singleton
		*/
		static TaskManager* singleton;
	protected:
		/** Säieallas
		*/
		TaskPool tp;
		/** Globaali lokiolio
		*/
		LogManager globalLogger;
	};
}

#endif