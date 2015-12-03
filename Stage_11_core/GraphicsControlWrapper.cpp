#include "stdafx.h"
#include "GraphicsControlWrapper.h"

using namespace stage_11;

GraphicsControlWrapper* GraphicsControlWrapper::globalController = nullptr;

GraphicsControlWrapper::GraphicsControlWrapper(std::string& windowName, int xres, int yres) : gc(windowName, xres, yres){
	//Sallitaan vain yksi globaali GraphicsControlWrapper
	if (globalController != nullptr){
		LOGMSG("Global graphics control threadsafety wrapper already set, aborting");
		abort();
	}
	globalController = this;
}
void GraphicsControlWrapper::queue(const stage_common::Model* model, const glm::mat4& position){
	std::unique_lock<std::mutex> lock(gcqueuemutex);
	gc.queue(model, position);
}