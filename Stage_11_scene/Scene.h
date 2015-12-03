#ifndef SCENE_H
#define SCENE_H

#include "stdafx.h"
#include <Task.h>
#include <list>
#include "GameObject.h"

namespace stage_11{
	/** Pelialueen mallintava luokka.*/
	class Scene{
	public:
		/** Luo työtehtävän, joka päivittää pelialueen peliolioiden tilan
		@param elapsedMS	Edellisestä ruudunpäivityksestä kulunut aika
		@returns			Työtehtäväolio, joka päivittää tämän pelialueen pelioliot
		*/
		Task* update(float elapsedMS){
			return new Update(this, elapsedMS);
		}
		/** Luo työtehtävän, joka piirtää pelialueen pelioliot
		@returns	Työtehtäväolio, joka piirtää tämän pelialueen pelioliot
		*/
		Task* render(){
			return new Render(this);
		}
		/** Luo pelialueelle uuden peliolion
		@returns	Viite luotuun peliolioon
		*/
		GameObject& createObject(){
			std::unique_lock<std::mutex>(objectListMutex);
			GameObject* obj = new GameObject();
			objects.push_back(obj);
			return *obj;
		}
		/** Luo uuden pelialueen*/
		Scene(){}
		/** Tuhoaa pelialueen ja sen sisältämät pelioliot*/
		~Scene(){
			std::unique_lock<std::mutex>(objectListMutex);
			std::for_each(objects.begin(), objects.end(), [](GameObject* go){
				delete go;
			});
		}
	private:
		/** Pelialueen sisältämät pelioliot*/
		std::list<GameObject*> objects;
		/** Pelioliolistaa suojaava lukko*/
		std::mutex objectListMutex;

		Scene(const Scene& other) = delete;
		Scene& operator= (const Scene& other) = delete;

		/** Työtehtäväolio, joka päivittää pelialueen sisältämien peliolioiden tilan*/
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
			/** Se pelialue, jonka oliot päivitetään*/
			Scene* sc;
			/** Edellisestä ruudunpäivityksestä kulunut aika*/
			float elapsedMS;
		};
		/** Työtehtäväolio, joka piirtää ruudulle pelialueen sisältämät pelioliot*/
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
			/** Se pelialue, jonka oliot piirretään*/
			Scene* sc;
		};
	};
}
#endif