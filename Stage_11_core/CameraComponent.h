#ifndef CAMERACOMPONENT_H
#define CAMERACOMPONENT_H

#include "stdafx.h"
#include <Transform.h>
#include <Camera.h>
#include <glm\glm.hpp>
#include <GameObject.h>
#include <TaskManager.h>

#define CAMERACOMPONENT_ID 2

namespace stage_11{
	/**Komponentti, joka liittää kameran peliolioon
	*/
	class CameraComponent : public Component{
	public:
		/**Luo uuden kamerakomponentin oletusparametreilla
		(45 asteen fov, kuvasuhde 4:3, piirtoetäisyys 0.1-100, suunnattu origoon)
		Katso oikea käyttö yliluokasta
		@param owner	Viite tämän komponentin omistavaan peliolioon
		*/
		CameraComponent(GameObject& owner);

		/**Luo uuden kamerakomponentin. Katso oikea käyttö yliluokasta.
		@param owner				Viite tämän komponentin omistavaan peliolioon
		@param initialProjection	Kameran projektiomatriisi simulaation alussa
		@param initialView			Kameran näkymämatriisi simulaation alussa
		*/
		CameraComponent(GameObject& owner, glm::mat4& initialProjection, glm::mat4& initialView);

		/** Palauttaa kamerakomponentin komponenttitunnuksen
		@returns	Kamerakomponentin komponenttitunnus
		*/
		virtual int id(){return CAMERACOMPONENT_ID;}

		/** Valmistelee kameran ruudun piirtämistä varten
		*/
		void doRender(){
			cam.setViewMatrix(position->getMatrix());
		}

		/** Hakee osoittimen kamerakomponentin kameraolioon
		@returns	osoitin kameraolioon
		*/
		stage_common::Camera* getRawCamera(){ return &cam; }
	private:
		/** Osoitin kamerakomponentin omistavan peliolion sijaintia ylläpitävään olioon
		*/
		Transform* position;

		/** Kameraolio
		*/
		stage_common::Camera cam;
	};
}

#endif