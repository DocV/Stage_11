#ifndef GAMELOOP_H
#define GAMELOOP_H

#include "stdafx.h"
#include <iostream>
#include <thread>
#include <TaskPool.h>
#include <algorithm>
#include <TaskManager.h>
#include <SceneManager.h>
#include "GraphicsControlWrapper.h"
#include "CameraComponent.h"

namespace stage_11{
	class Gameloop : public TaskManager, public SceneManager {
	public:
		Gameloop(std::string& windowName, int xres, int yres, unsigned int threads = 1) 
			: TaskManager(threads), threadcount(threads), gc(windowName, xres, yres){
		}

		~Gameloop(){
			std::for_each(threadlist.begin(), threadlist.end(), [](std::thread* t){delete t; });
		}

		void start(){
			LOGMSG("Starting Engine");
			for (unsigned int i = 0; i < threadcount; i++){
				threadlist.push_back(new std::thread(&TaskPool::work, std::ref(tp)));
			}
			loop();
		}

		void loop(){
			while (!terminated){
				tp.pushTask(activeScene->update());
				tp.waitForAllDone();
				tp.pushTask(activeScene->render());
				tp.waitForAllDone();
				gc.getController().draw(*(activeCam->getRawCamera()));
				if (gc.getController().shouldStop) terminated = true;
			}
			stop();
		}

		void stop(){
			terminated = true;
			tp.terminate();
			std::for_each(threadlist.begin(), threadlist.end(), [](std::thread* t){t->join(); });
			std::cout << "all done" << std::endl;
		}

	private:
		Gameloop(const Gameloop& other) = delete;
		Gameloop& operator= (const Gameloop& other) = delete;

		std::list<std::thread*> threadlist;
		unsigned int threadcount;
		bool terminated = false;

		GraphicsControlWrapper gc;
		CameraComponent* activeCam;
	};
}

#endif