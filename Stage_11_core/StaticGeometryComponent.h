#ifndef STATICGEOMETRYCOMPONENT_H
#define STATICGEOMETRYCOMPONENT_H

#include "stdafx.h"
#include "PhysicsComponent.h"

#define STATICGEOMETRYCOMPONENT_ID 5

namespace stage_11{
	/** Pelioliokomponentti, joka mallintaa staattista, liikkumatonta törmäyshahmoa, kuten pelimaailman maastoa
	tai seiniä.
	*/
	class StaticGeometryComponent : public Component, public EventHandler{
	public:
		/** Luo uuden staattisen törmäyshahmon, joka käyttää pallotörmäyshahmoa
		@param owner	Komponentin omistava peliolio
		@param radius	Törmäyshahmon säde
		*/
		StaticGeometryComponent(GameObject& owner, float radius);

		/** Luo uuden staattisen törmäyshahmon, joka käyttää AABB-törmäyshahmoa
		@param owner	Komponentin omistava peliolio
		@param radius	Törmäyshahmon koko
		*/
		StaticGeometryComponent(GameObject& owner, glm::vec3 size);

		/** Palauttaa staattisen törmäyshahmokomponentin komponenttitunnuksen
		@returns	Komponentin komponenttitunnus
		*/
		virtual int id(){
			return STATICGEOMETRYCOMPONENT_ID;
		}

		/** Päivittää komponentin tilan
		@param elapsedMS	Edellisestä ruudunpäivityksestä kulunut aika
		*/
		virtual void doUpdate();

		/** Käsittelee olion vastaanottamat tapahtumaviestit
		@param ev	Käsiteltävä viesti
		*/
		virtual void handleEvent(const Event& ev);
	private:
		/** Komponentin törmäyshahmo
		*/
		stage_common::Collider* coll;
		/** Komponentin omistavan peliolion sijaintikomponentti
		*/
		Transform* transform;
		/** Komponentin sisäistä tilaa suojaava lukko
		*/
		std::mutex updatemutex;
		/** Konstruktoreille yhteinen alustusfunktio
		@param owner	Viite tämän komponentin omistamaan peliolioon
		*/
		void setup(GameObject& owner);
	};
}

#endif