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
	class SceneManager{
	public:
		SceneManager(){}
		~SceneManager(){
			std::unique_lock<std::mutex> lock(sceneListMutex);
			std::for_each(scenes.begin(), scenes.end(), [](Scene* sc){delete sc; });
		}

		Scene& createScene(){
			std::unique_lock<std::mutex> lock(sceneListMutex);
			Scene* sc = new Scene();
			scenes.push_back(sc);
			return *sc;
		}

		void setActiveScene(unsigned int sc){
			std::unique_lock<std::mutex> lock(sceneListMutex);
			if (sc < 0 || sc > scenes.size()){
				std::cerr << "Attempted to set invalid active scene; " << std::to_string(sc) << std::endl;
				abort();
			}
			activeScene = scenes[sc];
		}

	protected:
		std::vector<Scene*> scenes;
		Scene* activeScene;
		std::mutex sceneListMutex;
	};
}

#endif