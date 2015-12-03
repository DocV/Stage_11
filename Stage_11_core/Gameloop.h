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
	/** Pelisilmukan toteuttava luokka.
	Luo peli-ikkunan ja käynnistää pelin.*/
	class Gameloop : public TaskManager, public SceneManager {
	public:
		/** Luo uuden pelisilmukan
		@param windowName	Peli-ikkunan nimi
		@param xres			Peli-ikkunan sivusuuntainen resoluutio
		@param yres			Peli-ikkunan pystysuuntainen resoluutio
		@param threads		Pelimoottorin työntekijäsäikeiden määrä
		*/
		Gameloop(std::string& windowName, int xres, int yres, unsigned int threads = 1);
		/** Tuhoaa pelisilmukkaolion ja kaikki pelin pelialueet*/
		~Gameloop();
		/** Käynnistää pelisilmukan suorituksen*/
		void start();
		/** Suorittaa pelisilmukkaa, kunnes stop()-metodia kutsutaan*/
		void loop();
		/** Lopettaa pelin*/
		void shutdown();
		/** Pysäyttää pelin suorituksen nykyisen ruudunpäivityksen laskennan päätyttyä*/
		void stop();
		/** Asettaa sen kameraolion, jonka näkökulmasta pelimaailma kuvataan
		@param cam	Osoitin kamerakomponenttiolioon, jonka näkokulmaa käytetään
		*/
		void setActiveCamera(CameraComponent* cam);
		/** Hakee Gameloop-singletonin osoitteen
		@returns	Osoite globaaliin Gameloop-olioon
		*/
		static Gameloop* getMainGameloop(){	return mainLoop;}
	private:
		Gameloop(const Gameloop& other) = delete;
		Gameloop& operator= (const Gameloop& other) = delete;

		/**Globaali Gameloop-singleton*/
		static Gameloop* mainLoop;
		/** Pelimoottorin työntekijäsäikeet*/
		std::list<std::thread*> threadlist;
		/** Työntekijäsäikeiden määrä*/
		unsigned int threadcount;
		/** Onko pelin suoritus lopetettu?*/
		bool terminated = false;
		/**Grafiikkamoottoria hallinnoiva olio*/
		GraphicsControlWrapper gc;
		/**Se kameraolio, jonka näkökulmasta peli kuvataan*/
		CameraComponent* activeCam;
		/**Ajastin, joka mittaa pelisilmukan suorittamiseen kuluvaa aikaa*/
		stage_common::Timer looptimer;
		/**Ajastin, joka mittaa pelisilmukan päivitysvaiheeseen kuluvaa aikaa*/
		stage_common::Timer updatetimer;
		/**Ajastin, joka mittaa pelisilmukan piirtovaiheeseen kuluvaa aikaa*/
		stage_common::Timer rendertimer;
		/**Ajastin, joka mittaa pelisilmukan ylläpitovaiheeseen kuluvaa aikaa*/
		stage_common::Timer maintenancetimer;
		/**Lukko, jolla suojataan terminated-muuttujaa*/
		std::mutex stopMutex;
	};
}
#endif