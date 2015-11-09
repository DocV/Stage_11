#ifndef GRAPHICSCONTROLWRAPPER_H
#define GRAPHICSCONTROLWRAPPER_H

#include "stdafx.h"
#include <mutex>
#include <glm\glm.hpp>
#include <TaskManager.h>
#include <GraphicsController.h>

namespace stage_11{
	/** Luokka, joka tarjoaa säieturvallisen rajapinnan grafiikkamoottorin hallintaluokkaan
	*/
	class GraphicsControlWrapper{
		friend class Gameloop;
	public:
		/** Luo uuden grafiikkamoottorin
		@param windowName	Peli-ikkunan nimi
		@param xres			Peli-ikkunan vaakaresoluutio
		@param yres			Peli-ikkunan pystyresoluutio
		*/
		GraphicsControlWrapper(std::string& windowName, int xres, int yres);
		/** Tuhoaa grafiikkamoottorin
		*/
		~GraphicsControlWrapper(){
			globalController = nullptr;
		}
		/** Lisää 3D-mallin seuraavan ruudunpäivityksen aikana piirrettävien mallien listalle
		@param model	Osoite piirrettävään malliolioon
		@param position	Sijainti, johon malli piirretään
		*/
		void queue(const stage_common::Model* model, const glm::mat4& position);

		/** Hakee osoittimen globaaliin grafiikkamoottorisingletoniin
		@returns	Osoitin globaaliin grafiikkamoottoriolioon
		*/
		static GraphicsControlWrapper& getGlobalController(){ return *globalController;	}
	private:
		GraphicsControlWrapper(const GraphicsControlWrapper& other) = delete;
		GraphicsControlWrapper& operator= (const GraphicsControlWrapper& other) = delete;

		/** Grafiikkamoottoriolio
		*/
		stage_common::GraphicsController gc;
		/** Lukko, jolla estetään useampaa säiettä muuttamasta piirrettävien mallien listaa samanaikaisesti
		*/
		std::mutex gcqueuemutex;

		/** Globaali grafikkamoottorisingleton
		*/
		static GraphicsControlWrapper* globalController;

		/** Hakee viitteen tämän olion sisältämään grafiikkamoottoriolioon
		@returns	Viite yksisäikeiseen grafiikkamoottoriolioon
		*/
		stage_common::GraphicsController& getController(){return gc;}
	};
}


#endif