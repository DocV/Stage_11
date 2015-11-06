#include "stdafx.h"
#include "TaskPool.h"

using namespace stage_11;

Task* TaskPool::pullTask(){
	std::unique_lock<std::mutex> lock(taskListMutex);

	if (tasks.size() < 1 && !terminated){
		waitingThreads++;
		if (waitingThreads >= threadCount) allDone.notify_one();
		hasTasks.wait(lock, [this]{return tasks.size() > 0 || terminated; });
		waitingThreads--;
	}
	if (terminated) return nullptr;
	Task* ret = tasks.front();
	tasks.pop_front();
	return ret;
}

void TaskPool::pushTask(Task* task){
	std::unique_lock<std::mutex> lock(taskListMutex);
	tasks.push_back(task);
	lock.unlock();
	hasTasks.notify_one();
}

void TaskPool::pushTaskList(std::list<Task*>& taskList){
	std::unique_lock<std::mutex> lock(taskListMutex);
	for (std::list<Task*>::iterator it = taskList.begin(); it != taskList.end(); it++){
		tasks.push_back(*it);
	}
	lock.unlock();
	hasTasks.notify_all();
}

void TaskPool::waitForAllDone(){
	std::unique_lock<std::mutex> lock(taskListMutex);
	if (waitingThreads < threadCount || tasks.size() > 0) allDone.wait(lock, [this]{
		return (waitingThreads == threadCount && tasks.size() < 1);
	});
}

void TaskPool::work(){
	while (true){
		Task* task = pullTask();
		if (task == nullptr) return;
		(*task)();
		delete task;
	}
}

void TaskPool::terminate(){
	std::unique_lock<std::mutex> lock(taskListMutex);
	terminated = true;
	lock.unlock();
	hasTasks.notify_all();
}