#ifndef COMPONENT_H
#define COMPONENT_H

#include "stdafx.h"
#include <Task.h>

namespace stage_11{
	class GameObject;
	/** Yliluokka pelioliokomponenteille. Komponentit liitetään peliolioihin ja ne suorittavat laskentaa pelimoottorin päivitys- ja piirtovaiheissa.*/
	class Component{
	public:
		/** Suorittaa tämän komponentin päivitysvaiheen laskennan.
		@param elapsedMS	Edellisestä ruudunpäivityksestä kulunut aika
		*/
		virtual void doUpdate(float elapsedMS){}
		/** Suorittaa tämän komponentin piirtovaiheen laskennan.
		*/
		virtual void doRender(){}
		/** Palauttaa tunnusluvun, jonka avulla tunnistetaan tämän komponentin tyyppi
		@returns	Tämän komponentin tyyppitunnus
		*/
		virtual int id() = 0;
		/** Luo uuden komponentin. Luonnin jälkeen tämän komponentin omistava olio hallinnoi sen elinkaarta,
		joten komponentti on aina luotava new:lla.
		@param owner	Tämän komponentin omistava peliolio
		*/
		Component(GameObject& owner);
		/** Luo uuden työtehtävän, joka kutsuu tämän komponentin päivitysmetodia.
		@returns	Tämän komponentin päivitystyötehtävä. Tuhottava suorituksen jälkeen manuaalisesti deletellä.
		*/
		Task* update(float elapsedMS){
			return new Update(this, elapsedMS);
		}
		/** Luo uuden työtehtävän, joka kutsuu tämän komponentin piirtometodia.
		@returns	Tämän komponentin piirtotyötehtävä. Tuhottava suorituksen jälkeen manuaalisesti deletellä.
		*/
		Task* render(){
			return new Render(this);
		}
	protected:
		/** Työtehtäväolio, joka kutsuu komponenttiolion päivitysmetodia*/
		class Update : public Task{
		public:
			TASK_EXECUTE {
				c->doUpdate(elapsedMS);
			};
			Update(Component*c, float elapsedMS) :c(c), elapsedMS(elapsedMS){}
		private:
			/** Se komponentti, jonka päivitysmetodia kutsutaan.*/
			Component* c;
			/** Edellisestä ruudunpäivityksestä kulunut aika.*/
			float elapsedMS;
		};
		/** Työtehtäväolio, joka kutsuu komponenttiolion piirtometodia*/
		class Render : public Task{
		public:
			TASK_EXECUTE{
				c->doRender();
			}
			Render(Component*c) :c(c){}
		private:
			/** Se komponentti, jonka piirtometodia kutsutaan.*/
			Component* c;
		};
	private:
		Component(const Component& other) = delete;
		Component& operator= (const Component& other) = delete;
	};

}
#endif