#ifndef GAMEOBJECTFACTORY_H
#define GAMEOBJECTFACTORY_H

#include "stdafx.h"
#include "Plane.h"
#include "Sphere.h"
#include <Scene.h>
#include <glm\glm.hpp>
#include <PhysicsComponent.h>
#include <StaticGeometryComponent.h>
#include <ModelComponent.h>
#include <SimpleShader.h>

namespace stage_11{
	/** Luokka, jota käytetään demo-ohjelman peliolioiden luomiseen
	*/
	class GameObjectFactory{
	public:
		/** Luo satunnaiseen paikkaan pallon, joka liikkuu satunnaiseen suuntaan
		@param sc				Pelialue, jolle pallo luodaan
		@param maxCoordinates	Luotavan pallon maksimietäisyys origosta
		@returns				Viite luotuun peliolioon
		*/
		static GameObject& constructRandomSphere(Scene* sc, glm::vec3 maxCoordinates){
			//Luodaan peliolio
			GameObject& obj = sc->createObject();
			//Arvotaan peliolion alkusijainti
			glm::vec3 translation(randomFloat(-maxCoordinates.x, maxCoordinates.x),
				randomFloat(-maxCoordinates.y, maxCoordinates.y),
				randomFloat(-maxCoordinates.z, maxCoordinates.z));
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), translation);
			//Luodaan pelioliolle sijaintikomponentti
			Transform* tf = new Transform(obj, transform);
			//Kiinnitetään peliolioon 3D-malli
			ModelComponent* mod = new ModelComponent(obj, &(getSingleton().mod_sphere));
			//Arvotaan peliolion alkunopeus
			glm::vec3 velocity(randomFloat(-0.01f, 0.01f), randomFloat(-0.01f, 0.01f), randomFloat(-0.01f, 0.01f));
			//Luodaan pelioliolle fysiikkakomponentti
			PhysicsComponent* pc = new PhysicsComponent(obj, 1.0f, velocity, 1.0f);
			return obj;
		}

		/** Luo pelimaailmaan seinän eli litteän, staattisen törmäyspinnan
		@param sc			Pelialue, johon seinä luodaan
		@param transform	Seinän keskipisteen 3D-sijainti
		@param size			Seinän koko
		@returns				Viite luotuun peliolioon
		*/
		static GameObject& constructWall(Scene* sc, const glm::mat4& transform, glm::vec3 size){
			//Luodaan uusi peliolio
			GameObject& obj = sc->createObject();
			//Luodaan pelioliolle sijaintikomponentti
			Transform* tf = new Transform(obj, transform);
			//Kiinnitetään peliolioon 3D-malli
			ModelComponent* mod = new ModelComponent(obj, &(getSingleton().mod_plane));
			//Luodaan pelioliolle liikkumaton törmäyshahmo
			StaticGeometryComponent* sgc = new StaticGeometryComponent(obj, size);
			return obj;
		}
	private:
		/** Peliolioiden käyttämä sävytinohjelma
		*/
		stage_common::SimpleShader ss;

		/** Pallon 3D-malli
		*/
		stage_common::Model mod_sphere;

		/** Seinän 3D-malli
		*/
		stage_common::Model mod_plane;

		/**Hakee viitteen globaaliin singleton-olioon, joka luo 3D-mallit ja niiden sävytinohjelman sekä pitää ne muistissa
		@returns	Viite singleton-olioon
		*/
		static GameObjectFactory& getSingleton(){
			//Luodaan singleton ensimmäisellä suorituskerralla
			static GameObjectFactory gof;
			return gof;
		}

		/**Arpoo liukuluvun kahden liukuluvun väliltä
		@param start	Arvottavan luvun alaraja
		@param end		Arvottavan luvun yläraja
		@returns		Satunnainen luku ala- ja ylärajojen väliltä
		*/
		static float randomFloat(float start, float end){
			float random = ((float)rand()) / (float)RAND_MAX;
			return start + (end - start) * random;
		}

		/** Luo uuden GameObjectFactory-olion, eli käytännössä lataa muistiin peliolioiden luomisessa tarvittavat resurssit
		*/
		GameObjectFactory() : mod_sphere(generate_sphere_vertices(), generate_sphere_colors(), &ss),
			mod_plane(generate_plane_vertices(), generate_plane_colors(), &ss){
		}
	};
}

#endif