#ifndef SCENE_H
#define SCENE_H

#include "stdafx.h"
#include <Task.h>
#include <list>
#include "GameObject.h"

namespace stage_11{
	class Scene{
	public:
		Task* update(){
			return new Update(this);
		}
		Task* render(){
			return new Render(this);
		}
		GameObject& createObject(){
			std::unique_lock<std::mutex>(objectListMutex);
			GameObject* obj = new GameObject();
			objects.push_back(obj);
			return *obj;
		}
		Scene(){}
		~Scene(){
			std::unique_lock<std::mutex>(objectListMutex);
			std::for_each(objects.begin(), objects.end(), [](GameObject* go){
				delete go;
			});
		}
	private:
		std::list<GameObject*> objects;
		std::mutex objectListMutex;

		Scene(const Scene& other) = delete;
		Scene& operator= (const Scene& other) = delete;

		class Update : public Task{
		public:
			Update(Scene* sc) : sc(sc){}
			void operator()(){
				std::unique_lock<std::mutex>(sc->objectListMutex);
				std::for_each(sc->objects.begin(), sc->objects.end(), [](GameObject* go){
					TaskManager::pushTask(go->update());
				});
			}
		private:
			Scene* sc;
		};

		class Render : public Task{
		public:
			Render(Scene* sc) : sc(sc){}
			void operator()(){
				std::unique_lock<std::mutex>(sc->objectListMutex);
				std::for_each(sc->objects.begin(), sc->objects.end(), [](GameObject* go){
					TaskManager::pushTask(go->render());
				});
			}
		private:
			Scene* sc;
		};
	};
}

#endif