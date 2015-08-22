#ifndef GRAPHICSCONTROLWRAPPER_H
#define GRAPHICSCONTROLWRAPPER_H

#include "stdafx.h"
#include <mutex>
#include <glm\glm.hpp>
#include <TaskManager.h>
#include <GraphicsController.h>

namespace stage_11{
	class GraphicsControlWrapper{
		friend class Gameloop;
	public:
		GraphicsControlWrapper(std::string& windowName, int xres, int yres) : gc(windowName, xres, yres){
			if (globalController != nullptr){
				LOGMSG("Global graphics control threadsafety wrapper already set, aborting");
				abort();
			}
			globalController = this;
		}
		~GraphicsControlWrapper(){
			globalController = nullptr;
		}
		void queue(const stage_common::Model* model, const glm::mat4& position){
			std::unique_lock<std::mutex> lock(gcqueuemutex);
			gc.queue(model, position);
		}
		GraphicsControlWrapper& getGlobalController(){
			return *globalController;
		}
	private:
		GraphicsControlWrapper(const GraphicsControlWrapper& other) = delete;
		GraphicsControlWrapper& operator= (const GraphicsControlWrapper& other) = delete;

		stage_common::GraphicsController gc;
		std::mutex gcqueuemutex;

		static GraphicsControlWrapper* globalController;

		stage_common::GraphicsController& getController(){
			return gc;
		}
	};
}


#endif