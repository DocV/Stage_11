#ifndef TASKPOOL_H
#define TASKPOOL_H

#include "stdafx.h"
#include "Task.h"
#include <list>
#include <mutex>
#include <condition_variable>
#include <algorithm>

namespace stage_11{	
	class TaskPool{
	public:
		TaskPool& operator=(const TaskPool& other) = delete;
		TaskPool(const TaskPool& other) = delete;
		
		Task* pullTask();

		void pushTask(Task* task);

		void pushTaskList(std::list<Task*>& taskList);

		void waitForAllDone();

		void work();

		void terminate();

		TaskPool(unsigned int threadCount) : threadCount(threadCount){}

		~TaskPool(){
			std::for_each(tasks.begin(), tasks.end(), [](Task* t){delete t; });
		}
	private:
		std::list<Task*> tasks;
		std::condition_variable hasTasks;
		std::condition_variable allDone;
		std::mutex taskListMutex;
		
		bool terminated = false;
		unsigned int waitingThreads = 0;
		unsigned int threadCount;
	};
}

#endif