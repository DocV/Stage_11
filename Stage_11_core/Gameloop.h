#ifndef GAMELOOP_H
#define GAMELOOP_H

#include "stdafx.h"
#include <iostream>
#include <thread>
#include <TaskPool.h>
#include <algorithm>
#include <TaskManager.h>
#include <SceneManager.h>

namespace stage_11{
	class Gameloop : public TaskManager, public SceneManager {
	public:
		Gameloop(unsigned int threads = 1) : TaskManager(threads), threadcount(threads){
		}

		void start(){
			std::cout << "starting engine" << std::endl;
			for (unsigned int i = 0; i < threadcount; i++){
				threadlist.push_back(new std::thread(&TaskPool::work, std::ref(tp)));
			}
			loop();
		}

		void loop(){
			while (true){
				tp.pushTask(activeScene->update());
				tp.waitForAllDone();
				tp.pushTask(activeScene->render());
				tp.waitForAllDone();
			}
			stop();
		}

		void stop(){
			tp.terminate();
			std::for_each(threadlist.begin(), threadlist.end(), [](std::thread* t){t->join(); });
			std::cout << "all done" << std::endl;
		}

	private:
		std::list<std::thread*> threadlist;
		unsigned int threadcount;

		class testFunc : public Task{
		public:
			virtual void operator()(){
				std::cout << "testing" << std::endl;
			}
		};
	};
}

#endif