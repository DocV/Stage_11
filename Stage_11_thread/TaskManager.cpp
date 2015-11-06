#include "stdafx.h"
#include "TaskManager.h"

using namespace stage_11;

TaskManager* TaskManager::singleton = nullptr;

TaskManager::TaskManager(unsigned int threadcount) : tp(threadcount), globalLogger(std::cout, std::cerr){
	if (singleton != nullptr){
		globalLogger.logError(std::string("Error: attempted to create a global Task Manager when one already exists"));
		abort();
	}
	singleton = this;
}

TaskManager::~TaskManager(){
	singleton = nullptr;
}