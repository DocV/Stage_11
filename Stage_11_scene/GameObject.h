#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "stdafx.h"
#include "Component.h"
#include <list>
#include <TaskManager.h>
#include <algorithm>
#include <mutex>

namespace stage_11{
	/** Peliolion mallintava olio. Peliolio liittää eri komponentteja yhteen yhdeksi kokonaisuudeksi
	*/
	class GameObject{
	public:
		/** Luo uuden työtehtäväolion, joka luo päivitystyötehtävät kaikille tämän peliolion komponenteille
		@param	elapsedMS	Edellisestä ruudunpäivityksestä kulunut aika
		@returns			Työtehtäväolio, joka päivittää tämän peliolion komponentit
		*/
		Task* update(float elapsedMS){
			return new Update(this, elapsedMS);
		}
		/** Luo uuden työtehtäväolion, joka luo piirtotyötehtävät kaikille tämän peliolion komponenteille
		@returns	Työtehtäväolio, joka piirtää tämän peliolion komponentit
		*/
		Task* render(){
			return new Render(this);
		}
		/** Liittää tähän peliolioon uuden komponentin.
		Liittämisen jälkeen peliolio hallinnoi komponentin elinkaarta, joten komponentti on aina luotava new:lla.
		@param comp	Osoitin liitettävään komponenttiin
		*/
		void addComponent(Component* comp){
			std::unique_lock<std::mutex>(componentListMutex);
			components.push_back(comp);
		}
		/** Hakee osoittimen tämän peliolion omistamaan tietyntyyppiseen komponenttiin
		@param id	Etsittävän komponentin tyyppitunnus
		@returns	Osoitin etsittyyn komponentiin tai nullptr, jos etsityn tyyppistä komponenttia ei ole
		*/
		Component* getComponentByID(int id){
			for (std::list<Component*>::iterator it = components.begin(); it != components.end(); it++){
				if ((*it)->id() == id) return *it;
			}
			return nullptr;
		}

		/** Luo uuden peliolion
		*/
		GameObject(){}
		/** Tuhoaa peliolion ja sen komponentit
		*/
		~GameObject(){
			std::unique_lock<std::mutex>(componentListMutex);
			std::for_each(components.begin(), components.end(), [](Component* c){
				delete c;
			});
		}
	private:
		/** Tämän peliolion omistamat komponentit
		*/
		std::list<Component*> components;
		/** Komponenttilistaa suojaava lukko
		*/
		std::mutex componentListMutex;

		GameObject(const GameObject& other) = delete;
		GameObject& operator= (const GameObject& other) = delete;

		/** Työtehtäväolio, joka päivittää peliolion komponenttien tilan
		*/
		class Update : public Task{
		public:
			TASK_EXECUTE{
				std::unique_lock<std::mutex>(go->componentListMutex);
				std::list<Task*> tasks;
				std::for_each(go->components.begin(), go->components.end(), [this, &tasks](Component* c){
					tasks.push_back(c->update(elapsedMS));
				});
				//Lisätään kaikki komponenttipäivitykset kerralla
				TaskManager::pushTaskList(tasks);
			};
			Update(GameObject* go, float elapsedMS) :go(go), elapsedMS(elapsedMS){}
		private:
			/** Se peliolio, jonka komponentit päivitetään
			*/
			GameObject* go;
			/** Edellisestä ruudunpäivityksestä kulunut aika
			*/
			float elapsedMS;
		};
		/** Työtehtäväolio, joka päivittää valmistelee peliolion komponentit piirtoa varten
		*/
		class Render : public Task{
		public:
			TASK_EXECUTE{
				std::unique_lock<std::mutex>(go->componentListMutex);
				std::list<Task*> tasks;
				std::for_each(go->components.begin(), go->components.end(), [&tasks](Component* c){
					tasks.push_back(c->render());
				});
				//Lisätään kaikki työtehtävät kerralla
				TaskManager::pushTaskList(tasks);
			}
			Render(GameObject* go) :go(go){}
		private:
			/** Se peliolio, jonka komponentit piirretään
			*/
			GameObject* go;
		};
	};
}

#endif