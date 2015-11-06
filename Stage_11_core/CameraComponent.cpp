#include "CameraComponent.h"

using namespace stage_11;

CameraComponent::CameraComponent(GameObject& owner) : Component(owner){
	position = (Transform*)owner.getComponentByID(TRANSFORM_ID);
	if (position == nullptr){
		LOGERR(std::string("Error: parent game object does not have a transform, can't initialize camera component"));
		abort();
	}
}

CameraComponent::CameraComponent(GameObject& owner, glm::mat4& initialProjection, glm::mat4& initialView) :
Component(owner), cam(initialProjection, initialView){
	position = (Transform*)owner.getComponentByID(TRANSFORM_ID);
	if (position == nullptr){
		LOGERR(std::string("Error: parent game object does not have a transform, can't initialize camera component"));
		abort();
	}
}