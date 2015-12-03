#pragma once

#include "stdafx.h"
#ifndef CAMERACONTROLCOMPONENT_H
#define CAMERACONTROLCOMPONENT_H

#define CAMERACONTROLCOMPONENT_ID 6
#define CAMERASPEED 0.025f

#include <Component.h>
#include <Transform.h>
#include <Input.h>
#include <GLFW\glfw3.h>
#include <SceneManager.h>
#include <glm\gtx\quaternion.hpp>
#include <Gameloop.h>

namespace stage_11{
	/** Peliolioon liitettävä komponentti, joka mahdollistaa peliolion liikuttamisen näppäimistöllä.
	Suunniteltu kameran liikuttamiseen*/
	class CameraControlComponent : public Component{
	public:
		/** Luo uuden ohjauskomponentin
		@param owner	Komponentin omistava olio
		*/
		CameraControlComponent(GameObject& owner) : Component(owner){
			tr = (Transform*)(owner.getComponentByID(TRANSFORM_ID));
			//Rekisteröidään tarkkailtavat näppäimet
			stage_common::Input& in = stage_common::Input::getSingleton();
			in.registerKey(GLFW_KEY_W);
			in.registerKey(GLFW_KEY_S);
			in.registerKey(GLFW_KEY_A);
			in.registerKey(GLFW_KEY_D);
			in.registerKey(GLFW_KEY_R);
			in.registerKey(GLFW_KEY_F);
			in.registerKey(GLFW_KEY_ESCAPE);
		}
		/** Päivittää komponentin tilan
		@param elapsedMS	Edellisestä ruudunpäivityksestä kulunut aika
		*/
		void doUpdate(float elapsedMS){
			stage_common::Input& in = stage_common::Input::getSingleton();
			//Suunta, johon liikutaan
			glm::vec3 movement;
			//Tarkistetaan mitä näppäimiä on painettu
			if (in.getKeyDown(GLFW_KEY_W)) movement.z += CAMERASPEED * elapsedMS;
			if (in.getKeyDown(GLFW_KEY_S)) movement.z -= CAMERASPEED  * elapsedMS;
			if (in.getKeyDown(GLFW_KEY_A)) movement.x += CAMERASPEED  * elapsedMS;
			if (in.getKeyDown(GLFW_KEY_D)) movement.x -= CAMERASPEED  * elapsedMS;
			if (in.getKeyDown(GLFW_KEY_F)) movement.y += CAMERASPEED  * elapsedMS;
			if (in.getKeyDown(GLFW_KEY_R)) movement.y -= CAMERASPEED  * elapsedMS;
			//Siirretään isäntäoliota
			tr->translate(movement);
			//Suljetaan peli, jos painetaan escapea
			if (in.getKeyDown(GLFW_KEY_ESCAPE)) Gameloop::getMainGameloop()->stop();
		}
		/** Hakee tämän komponentin komponenttitunnuksen
		@returns	Komponentin tunnus
		*/
		int id(){
			return CAMERACONTROLCOMPONENT_ID;
		}
	private:
		/** Isäntäolion sijainnista kirjaa pitävä olio*/
		Transform* tr;
	};
}
#endif