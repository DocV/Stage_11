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
		CameraComponent(GameObject& owner);

		CameraComponent(GameObject& owner, glm::mat4& initialProjection, glm::mat4& initialView);

		virtual int id(){return CAMERACOMPONENT_ID;}

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