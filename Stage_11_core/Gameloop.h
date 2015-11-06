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
		Gameloop(std::string& windowName, int xres, int yres, unsigned int threads = 1);

		~Gameloop();

		void start();

		void loop();

		void shutdown();

		void stop();

		void setActiveCamera(CameraComponent* cam);

		static Gameloop* getMainGameloop(){	return mainLoop;}
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