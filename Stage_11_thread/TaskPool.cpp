#include "stdafx.h"
#include "TaskPool.h"

using namespace stage_11;

Task* TaskPool::pullTask(){
	std::unique_lock<std::mutex> lock(taskListMutex);
	//Jos työtehtäviä ei ole, jäädään odottamaan
	if (tasks.size() < 1 && !terminated){
		waitingThreads++;
		//Jos kaikki säikeet odottavat, herätetään pelisilmukka
		if (waitingThreads >= threadCount) allDone.notify_one();
		hasTasks.wait(lock, [this]{return tasks.size() > 0 || terminated; });
		waitingThreads--;
	}
	//Säieallas pysäytetty, lopetetaan suoritus
	if (terminated) return nullptr;
	//Palautetaan ensimmäinen työtehtävä
	Task* ret = tasks.front();
	tasks.pop_front();
	return ret;
}

void TaskPool::pushTask(Task* task){
	std::unique_lock<std::mutex> lock(taskListMutex);
	tasks.push_back(task);
	lock.unlock();
	//Herätetään työtehtäviä odottava säie
	hasTasks.notify_one();
}

void TaskPool::pushTaskList(std::list<Task*>& taskList){
	std::unique_lock<std::mutex> lock(taskListMutex);
	for (std::list<Task*>::iterator it = taskList.begin(); it != taskList.end(); it++){
		tasks.push_back(*it);
	}
	lock.unlock();
	//Herätetään työtehtäviä odottavat säikeet
	hasTasks.notify_all();
}

void TaskPool::waitForAllDone(){
	std::unique_lock<std::mutex> lock(taskListMutex);
	//Odotetaan, kunnes kaikki säikeet ovat odotustilassa ja tehtävälista on tyhjä
	if (waitingThreads < threadCount || tasks.size() > 0) allDone.wait(lock, [this]{
		return (waitingThreads == threadCount && tasks.size() < 1);
	});
}

void TaskPool::work(){
	while (true){
		//Haetaan uusi työtehtävä
		Task* task = pullTask();
		//Lopetetaan suoritus, kun tehtäviä ei ole
		if (task == nullptr) return;
		//Suoritetaan haettu tehtävä
		(*task)();
		//Tuhotaan suoritettu tehtävä
		delete task;
	}
}

void TaskPool::terminate(){
	std::unique_lock<std::mutex> lock(taskListMutex);
	terminated = true;
	lock.unlock();
	//Herätetään työntekijäsäikeet, jotta niiden suoritus voidaan lopettaa
	hasTasks.notify_all();
}