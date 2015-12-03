#include "Gameloop.h"

using namespace stage_11;

Gameloop* Gameloop::mainLoop = nullptr;

Gameloop::Gameloop(std::string& windowName, int xres, int yres, unsigned int threads)
	: TaskManager(threads), threadcount(threads), gc(windowName, xres, yres){
	//Sallitaan vain yksi globaali Gameloop
	if (mainLoop != nullptr){
		LOGERR("Game loop already exists, terminating");
		abort();
	}
	mainLoop = this;
}
Gameloop::~Gameloop(){
	//Tuhotaan työntekijäsäikeet
	std::for_each(threadlist.begin(), threadlist.end(), [](std::thread* t){delete t; });
	//Poistetaan singleton-osoite
	mainLoop = nullptr;
}
void Gameloop::start(){
	LOGMSG("Starting Engine");
	//Luodaan työntekijäsäikeet
	for (unsigned int i = 0; i < threadcount; i++){
		threadlist.push_back(new std::thread(&TaskPool::work, std::ref(tp)));
	}
	loop();
}
void Gameloop::loop(){
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
void Gameloop::shutdown(){
	std::unique_lock<std::mutex> lock(stopMutex);
	terminated = true;
	//Pysäytetään säieallas
	tp.terminate();
	//Odotetaan, että työntekijäsäikeet pysähtyvät
	std::for_each(threadlist.begin(), threadlist.end(), [](std::thread* t){t->join(); });

	std::cout << "Total runtime: " << looptimer.totalTime() << std::endl;
	std::cout << "Total frames: " << looptimer.totalTicks() << std::endl;
	std::cout << "Average loop time: " << looptimer.averageTime() << std::endl;
	std::cout << "Average fps: " << std::to_string(1000 / looptimer.averageTime()) << std::endl;
	std::cout << "Average update time: " << updatetimer.averageTime() << std::endl;
	std::cout << "Average render time: " << rendertimer.averageTime() << std::endl;
	std::cout << "Average maintenance time: " << maintenancetimer.averageTime() << std::endl;
}
void Gameloop::stop(){
	std::unique_lock<std::mutex> lock(stopMutex);
	terminated = true;
}
void Gameloop::setActiveCamera(CameraComponent* cam){
	activeCam = cam;
}