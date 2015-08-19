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
		
		Task* pullTask(){
			std::unique_lock<std::mutex> lock(taskListMutex);
			
			if (tasks.size() < 1 && !terminated){
				waitingThreads++;
				if (waitingThreads >= threadCount) allDone.notify_all();
				hasTasks.wait(lock, [this]{return tasks.size() > 0 || terminated; });
				waitingThreads--;
			}
			if (terminated) return nullptr;
			Task* ret = tasks.front();
			tasks.pop_front();
			return ret;
		}
		void pushTask(Task* task){
			std::unique_lock<std::mutex> lock(taskListMutex);
			tasks.push_back(task);
			lock.unlock();
			hasTasks.notify_one();
		}
		void waitForAllDone(){
			std::unique_lock<std::mutex> lock(taskListMutex);
			if (waitingThreads < threadCount) allDone.wait(lock);
		}
		void work(){
			while (true){
				Task* task = pullTask();
				if (task == nullptr) return;
				(*task)();
				delete task;
			}
			
		}
		void terminate(){
			std::unique_lock<std::mutex> lock(taskListMutex);
			terminated = true;
			lock.unlock();
			hasTasks.notify_all();
		}

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