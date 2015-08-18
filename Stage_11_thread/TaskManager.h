#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include "stdafx.h"
#include <iostream>
#include "TaskPool.h"

namespace stage_11{
	class TaskManager{
	public:
		TaskManager(unsigned int threadcount) : tp(threadcount){
			if (singleton != nullptr){
				std::cerr << "Error: attempted to create a global Task Manager when one already exists" << std::endl;
				abort();
			}
			singleton = this;
		}
		~TaskManager(){
			singleton = nullptr;
		}
		static void pushTask(Task* t){
			singleton->tp.pushTask(t);
		}
	private:
		static TaskManager* singleton;
	protected:
		TaskPool tp;
	};
}

#endif