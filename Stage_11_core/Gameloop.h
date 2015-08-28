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
#include <Timer.h>

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
				looptimer.start();

				//Päivitysvaihe
				updatetimer.start();
				tp.pushTask(activeScene->update());
				tp.waitForAllDone();
				updatetimer.stop();

				//Piirtovaihe
				rendertimer.start();
				tp.pushTask(activeScene->render());
				tp.waitForAllDone();
				gc.getController().draw(*(activeCam->getRawCamera()));
				rendertimer.stop();

				//Ylläpitovaihe
				maintenancetimer.start();
				if (gc.getController().shouldStop()) terminated = true;
				maintenancetimer.stop();
				looptimer.stop();
			}
			shutdown();
		}

		void shutdown(){
			terminated = true;
			tp.terminate();
			std::for_each(threadlist.begin(), threadlist.end(), [](std::thread* t){t->join(); });

			std::cout << "Total runtime: " << looptimer.totalTime() << std::endl;
			std::cout << "Total frames: " << looptimer.totalTicks() << std::endl;
			std::cout << "Average loop time: " << looptimer.averageTime() << std::endl;
			std::cout << "Average update time: " << updatetimer.averageTime() << std::endl;
			std::cout << "Average render time: " << rendertimer.averageTime() << std::endl;
			std::cout << "Average maintenance time: " << maintenancetimer.averageTime() << std::endl;
		}

		void stop(){
			terminated = true;
		}

		void setActiveCamera(CameraComponent* cam){
			activeCam = cam;
		}

	private:
		Gameloop(const Gameloop& other) = delete;
		Gameloop& operator= (const Gameloop& other) = delete;

		std::list<std::thread*> threadlist;
		unsigned int threadcount;
		bool terminated = false;

		GraphicsControlWrapper gc;
		CameraComponent* activeCam;

		stage_common::Timer looptimer;
		stage_common::Timer updatetimer;
		stage_common::Timer rendertimer;
		stage_common::Timer maintenancetimer;
	};
}

#endif