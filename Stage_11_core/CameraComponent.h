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
	class CameraComponent : public Component{
	public:
		CameraComponent(GameObject& owner) : Component(owner){
			position = (Transform*)owner.getComponentByID(TRANSFORM_ID);
			if (position == nullptr){
				LOGERR(std::string("Error: parent game object does not have a transform, can't initialize camera component"));
				abort();
			}
		}
		CameraComponent(GameObject& owner, glm::mat4& initialProjection, glm::mat4& initialView) :
			Component(owner), cam(initialProjection, initialView){
			position = (Transform*)owner.getComponentByID(TRANSFORM_ID);
			if (position == nullptr){
				LOGERR(std::string("Error: parent game object does not have a transform, can't initialize camera component"));
				abort();
			}
		}

		virtual int id(){
			return CAMERACOMPONENT_ID;
		}

		void doRender(){
			cam.setViewMatrix(position->getMatrix());
		}

		stage_common::Camera* getRawCamera(){ return &cam; }
	private:
		Transform* position;
		stage_common::Camera cam;
	};
}

#endif