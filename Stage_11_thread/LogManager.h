#ifndef LOGMANAGER_H
#define LOGMANAGER_H

#include "stdafx.h"
#include <mutex>
#include <Logger.h>

namespace stage_11{
	class LogManager{
	public:
		LogManager(std::ostream& standard, std::ostream& error) : logger(standard, error){
		}
		void log(std::string msg){
			std::unique_lock<std::mutex>(logmutex);
			logger.Log(msg);
		}
		void logError(std::string msg){
			std::unique_lock<std::mutex>(logmutex);
			logger.LogError(msg);
		}
	private:
		LogManager(const LogManager& other) = delete;
		LogManager* operator= (const LogManager& other) = delete;

		std::mutex logmutex;
		stage_common::Logger logger;
	};
}

#endif