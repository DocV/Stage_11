#ifndef SCENE_H
#define SCENE_H

#include "stdafx.h"
#include <Task.h>
#include <list>
#include "GameObject.h"

namespace stage_11{
	class Scene{
	public:
		Task* update(float elapsedMS){
			return new Update(this, elapsedMS);
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
			Update(Scene* sc, float elapsedMS) : sc(sc), elapsedMS(elapsedMS){}
			void operator()(){
				std::unique_lock<std::mutex>(sc->objectListMutex);
				std::list<Task*> tasks;
				std::for_each(sc->objects.begin(), sc->objects.end(), [this, &tasks](GameObject* go){
					tasks.push_back(go->update(elapsedMS));
				});
				TaskManager::pushTaskList(tasks);
			}
		private:
			Scene* sc;
			float elapsedMS;
		};

		class Render : public Task{
		public:
			Render(Scene* sc) : sc(sc){}
			void operator()(){
				std::unique_lock<std::mutex>(sc->objectListMutex);
				std::list<Task*> tasks;
				std::for_each(sc->objects.begin(), sc->objects.end(), [&tasks](GameObject* go){
					tasks.push_back(go->render());
				});
				TaskManager::pushTaskList(tasks);
			}
		private:
			Scene* sc;
		};
	};
}

#endif