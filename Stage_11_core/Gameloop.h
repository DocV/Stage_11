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
#include <Input.h>

namespace stage_11{
	class Gameloop : public TaskManager, public SceneManager {
	public:
		Gameloop(std::string& windowName, int xres, int yres, unsigned int threads = 1) 
			: TaskManager(threads), threadcount(threads), gc(windowName, xres, yres){
			if (mainLoop != nullptr){
				LOGERR("Game loop already exists, terminating");
				abort();
			}
			mainLoop = this;
		}

		~Gameloop(){
			std::for_each(threadlist.begin(), threadlist.end(), [](std::thread* t){delete t; });
			mainLoop = nullptr;
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
				tp.pushTask(activeScene->update(looptimer.lastTickTime()));
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
				stage_common::Input::getSingleton().update(false);
				std::unique_lock<std::mutex> lock(stopMutex);
				if (gc.getController().shouldStop()) terminated = true;
				maintenancetimer.stop();
				looptimer.stop();
			}
			shutdown();
		}

		void shutdown(){
			std::unique_lock<std::mutex> lock(stopMutex);
			terminated = true;
			tp.terminate();
			std::for_each(threadlist.begin(), threadlist.end(), [](std::thread* t){t->join(); });

			std::cout << "Total runtime: " << looptimer.totalTime() << std::endl;
			std::cout << "Total frames: " << looptimer.totalTicks() << std::endl;
			std::cout << "Average loop time: " << looptimer.averageTime() << std::endl;
			std::cout << "Average fps: " << std::to_string(1000 / looptimer.averageTime()) << std::endl;
			std::cout << "Average update time: " << updatetimer.averageTime() << std::endl;
			std::cout << "Average render time: " << rendertimer.averageTime() << std::endl;
			std::cout << "Average maintenance time: " << maintenancetimer.averageTime() << std::endl;
		}

		void stop(){
			//std::unique_lock<std::mutex> lock(stopMutex);
			terminated = true;
		}

		void setActiveCamera(CameraComponent* cam){
			activeCam = cam;
		}

		static Gameloop* getMainGameloop(){
			return mainLoop;
		}
	private:
		static Gameloop* mainLoop;

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

		std::mutex stopMutex;
	};
}

#endif