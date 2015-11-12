#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "stdafx.h"
#include <vector>
#include "Scene.h"
#include <mutex>
#include <algorithm>
#include <iostream>
#include <string>

namespace stage_11{
	/** Pelialueita hallinnoiva olio
	*/
	class SceneManager{
	public:
		/** Luo uuden pelialueita hallinnoivan olion
		*/
		SceneManager(){}
		/** Tuhoaa pelialueita hallinnoivan olion ja sen sisältämät pelialueet
		*/
		~SceneManager(){
			std::unique_lock<std::mutex> lock(sceneListMutex);
			std::for_each(scenes.begin(), scenes.end(), [](Scene* sc){delete sc; });
		}
		/** Luo uuden pelialueen
		@returns	Viite luotuun pelialueeseen
		*/
		Scene& createScene(){
			std::unique_lock<std::mutex> lock(sceneListMutex);
			Scene* sc = new Scene();
			scenes.push_back(sc);
			return *sc;
		}

		/** Asettaa aktiivisen pelialueen, eli määrittelee minkä pelialueen tila päivitetään pelisilmukkaa suoritettaessa
		@param sc	Uuden aktiivisen pelialueen indeksi pelialuelistassa
		*/
		void setActiveScene(unsigned int sc){
			std::unique_lock<std::mutex> lock(sceneListMutex);
			if (sc < 0 || sc > scenes.size()){
				std::cerr << "Attempted to set invalid active scene; " << std::to_string(sc) << std::endl;
				abort();
			}
			activeScene = scenes[sc];
		}

	protected:
		/** Lista pelin pelialueista
		*/
		std::vector<Scene*> scenes;
		/** Nykyinen aktiivinen pelialue
		*/
		Scene* activeScene;
		/** Pelialuelistaa suojaava lukko
		*/
		std::mutex sceneListMutex;
	};
}

#endif