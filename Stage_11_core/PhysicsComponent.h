#ifndef PHYSICSCOMPONENT_H
#define PHYSICSCOMPONENT_H

#include "stdafx.h"
#include <Component.h>
#include <EventChannel.h>
#include <Collisions.h>
#include <SphereCollider.h>
#include <AABBCollider.h>
#include <glm\glm.hpp>
#include <Transform.h>
#include <GameObject.h>
#include <algorithm>

#define PHYSICSCOMPONENT_ID 4
#define PHYSICS_COLLISION_EVENT_TYPE 99001

namespace stage_11{
	/** Yksinkertainen fysiikkakomponentti, joka liittää peliolioon törmäyshahmon,
	antaa sille liikesuunnan ja -nopeuden ja kimmottaa sen muista törmäyshahmoista
	*/
	class PhysicsComponent : public Component, public EventHandler{
		friend class StaticGeometryComponent;
	public:
		/** Hakee viitteen törmäystapahtumien tapahtumakanavaan
		@returns	Viite tapahtumakanavaan
		*/
		static EventChannel& getCollisionEventChannel(){
			//Palautetaan aina sama staattinen olio
			static EventChannel channel;
			return channel;
		}

		/** Törmäyshahmon siirtymisestä ilmoittava viestitietue
		*/
		struct CollisionEvent : public Event{
			virtual unsigned int getEventType() const {
				return PHYSICS_COLLISION_EVENT_TYPE;
			};
			/** Viite lähettäjäolioon
			*/
			PhysicsComponent& sender;
			/** Edellisestä ruudunpäivityksestä kulunut aika
			*/
			float elapsedMS;
			CollisionEvent(PhysicsComponent& sender, float elapsedMS)
				: sender(sender), elapsedMS(elapsedMS){}
		};

		/** Luo uuden fysiikkakomponentin pallotörmäyshahmolla. Katso oikea käyttö yliluokasta
		@param owner	Tämän komponentin omistava peliolio
		@param radius	Törmäyshahmon säde
		@param initialV	Fysiikkaolion liikesuunta ja -nopeus simulaation alussa
		@param mass		Fysiikkaolion massa
		*/
		PhysicsComponent(GameObject& owner, float radius, glm::vec3 initialV, float mass);

		/** Luo uuden fysiikkakomponentin AABB-törmäyshahmolla. Katso oikea käyttö yliluokasta
		@param owner	Tämän komponentin omistava peliolio
		@param siz		Törmäyshahmon koko
		@param initialV	Fysiikkaolion liikesuunta ja -nopeus simulaation alussa
		@param mass		Fysiikkaolion massa
		*/
		PhysicsComponent(GameObject& owner, glm::vec3 size, glm::vec3 initialV, float mass);

		/** Tuhoaa törmäyshahmon
		*/
		~PhysicsComponent();

		/** Päivittää fysiikkakomponentin tilan
		*/
		void doUpdate(float elapsedMS);

		/** Suorittaa fysiikkaolion piirtovaiheen laskennan
		*/
		void doRender();

		/** Käsittelee fysiikkaolion vastaanottamat viestit
		@param ev	Vastaanotettu viesti
		*/
		void handleEvent(const Event& ev);

		/** Hakee osoittimen tämän peliolion törmäyshahmoon
		@returns	Osoitin tämän peliolion törmäyshahmoon
		*/
		stage_common::Collider* getCollider(){
			return coll;
		}

		/** Hakee tämän peliolion nopeusvektorin
		@returns	Tämän peliolion nykyinen nopeusvektori
		*/
		glm::vec3 getVelocity(){
			return velocity;
		}

		/** Palauttaa fysiikkakomponentin komponenttitunnuksen
		@returns	Fysiikkkomponentin komponenttitunnus
		*/
		virtual int id(){
			return PHYSICSCOMPONENT_ID;
		}
	private:
		/** Tämän fysiikkaolion törmäyshahmo
		*/
		stage_common::Collider* coll;
		/** Tämän komponentin omistavan peliolion sijaintikomponentti
		*/
		Transform* transform;
		/** Onko tämän komponentin tila jo päivitetty tämän ruudunpäivityksen aikana
		*/
		bool updatedThisFrame = false;
		/** Tämän fysiikkolion nykyinen liikevektori
		*/
		glm::vec3 velocity;
		/** Tämän fysiikkolion liikevektori edellisen sijaintipäivityksen jälkeen
		*/
		glm::vec3 oldPos;
		/** Tämän fysiikkolion massa
		*/
		float mass;
		/** Fysiikkolion sisäistä tilaa suojaava lukko
		*/
		std::mutex colliderMutex;

		/** Konstruktoreille yhteinen alustusfunktio
		@param owner	Viite tämän komponentin omistamaan peliolioon
		*/
		void setup(GameObject& owner);

		/** Päivittää fysiikkaolion sijainnin nykyisen nopeusvektorin perusteella
		@param elapsedMS	Edellisestä ruudunpäivityksestä kulunut aika millisekunteina
		*/
		void updatePosition(float elapsedMS);

		/** Synkronoi fysiikkakomponentin liikkeet sen omistavan peliolion sijaintikomponentin kanssa
		*/
		void commitMovement();
	};
}

#endif