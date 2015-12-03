#ifndef LOGMANAGER_H
#define LOGMANAGER_H

#include "stdafx.h"
#include <mutex>
#include <Logger.h>

namespace stage_11{
	/** Olio, joka hallinnoi säieturvallista kirjoitusta lokiin*/
	class LogManager{
	public:
		/** Luo uuden lokinhallintaolion
		@param standard	Kanava, johon kirjoitetaan tavalliset lokiviestit
		@param error	Kanava, johon kirjoitetaan virheilmoitukset
		*/
		LogManager(std::ostream& standard, std::ostream& error) : logger(standard, error){
		}
		/** Kirjoittaa lokiin tavallisen viestin
		@param msg	Kirjoitettava viesti
		*/
		void log(std::string msg){
			std::unique_lock<std::mutex> lock(logmutex);
			logger.Log(msg);
		}
		/** Kirjoittaa lokiin virheilmoituksen
		@param msg	Kirjoitettava viesti
		*/
		void logError(std::string msg){
			std::unique_lock<std::mutex> lock(logmutex);
			logger.LogError(msg);
		}
	private:
		LogManager(const LogManager& other) = delete;
		LogManager* operator= (const LogManager& other) = delete;

		/** Pääsyä lokiin hallinnoiva lukko*/
		std::mutex logmutex;
		/** Lokiolio*/
		stage_common::Logger logger;
	};
}
#endif