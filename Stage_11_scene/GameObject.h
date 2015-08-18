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
		Task* update(){
			return new Update(this);
		}
		Task* render(){
			return new Render(this);
		}
		void addComponent(Component* comp){
			std::unique_lock<std::mutex>(componentListMutex);
			components.push_back(comp);
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
				std::for_each(go->components.begin(), go->components.end(), [](Component* c){
					TaskManager::pushTask(c->update());
				});
			}
			Update(GameObject* go) :go(go){}
		private:
			GameObject* go;
		};
		class Render : public Task{
		public:
			TASK_EXECUTE{
				std::unique_lock<std::mutex>(go->componentListMutex);
				std::for_each(go->components.begin(), go->components.end(), [](Component* c){
					TaskManager::pushTask(c->render());
				});
			}
			Render(GameObject* go) :go(go){}
		private:
			GameObject* go;
		};
	};
}

#endif