#ifndef STATICGEOMETRYCOMPONENT_H
#define STATICGEOMETRYCOMPONENT_H

#include "stdafx.h"
#include "PhysicsComponent.h"

#define STATICGEOMETRYCOMPONENT_ID 5

namespace stage_11{
	class StaticGeometryComponent : public Component, public EventHandler{
	public:
		StaticGeometryComponent(GameObject& owner, float radius);

		StaticGeometryComponent(GameObject& owner, glm::vec3 size);

		virtual int id(){
			return STATICGEOMETRYCOMPONENT_ID;
		}

		virtual void doUpdate();

		virtual void handleEvent(const Event& ev);
	private:
		stage_common::Collider* coll;
		Transform* transform;
		std::mutex updatemutex;

		void setup(GameObject& owner);

	};
}

#endif