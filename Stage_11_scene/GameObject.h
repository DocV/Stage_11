#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "stdafx.h"
#include "Component.h"
#include <list>
#include <TaskManager.h>
#include <algorithm>
#include <mutex>

namespace stage_11{
	class GameObject{
	public:
		Task* update(float elapsedMS){
			return new Update(this, elapsedMS);
		}
		Task* render(){
			return new Render(this);
		}
		void addComponent(Component* comp){
			std::unique_lock<std::mutex>(componentListMutex);
			components.push_back(comp);
		}

		Component* getComponentByID(int id){
			for (std::list<Component*>::iterator it = components.begin(); it != components.end(); it++){
				if ((*it)->id() == id) return *it;
			}
			return nullptr;
		}

		GameObject(){}
		~GameObject(){
			std::unique_lock<std::mutex>(componentListMutex);
			std::for_each(components.begin(), components.end(), [](Component* c){
				delete c;
			});
		}
	private:
		std::list<Component*> components;
		std::mutex componentListMutex;

		GameObject(const GameObject& other) = delete;
		GameObject& operator= (const GameObject& other) = delete;

		class Update : public Task{
		public:
			TASK_EXECUTE{
				std::unique_lock<std::mutex>(go->componentListMutex);
				std::list<Task*> tasks;
				std::for_each(go->components.begin(), go->components.end(), [this, &tasks](Component* c){
					tasks.push_back(c->update(elapsedMS));
				});
				TaskManager::pushTaskList(tasks);
			};
			Update(GameObject* go, float elapsedMS) :go(go), elapsedMS(elapsedMS){}
		private:
			GameObject* go;
			float elapsedMS;
		};
		class Render : public Task{
		public:
			TASK_EXECUTE{
				std::unique_lock<std::mutex>(go->componentListMutex);
				std::list<Task*> tasks;
				std::for_each(go->components.begin(), go->components.end(), [&tasks](Component* c){
					tasks.push_back(c->render());
				});
				TaskManager::pushTaskList(tasks);
			}
			Render(GameObject* go) :go(go){}
		private:
			GameObject* go;
		};
	};
}

#endif