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
	class PhysicsComponent : public Component, public EventHandler{
		friend class StaticGeometryComponent;
	public:
		static EventChannel& getCollisionEventChannel(){
			static EventChannel channel;
			return channel;
		}

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

		PhysicsComponent(GameObject& owner, float radius, glm::vec3 initialV, float mass);

		PhysicsComponent(GameObject& owner, glm::vec3 size, glm::vec3 initialV, float mass);

		~PhysicsComponent();

		void doUpdate(float elapsedMS);

		void doRender();

		void handleEvent(const Event& ev);

		stage_common::Collider* getCollider(){
			return coll;
		}

		glm::vec3 getVelocity(){
			return velocity;
		}

		virtual int id(){
			return PHYSICSCOMPONENT_ID;
		}
	private:
		stage_common::Collider* coll;
		Transform* transform;
		bool updatedThisFrame = false;
		glm::vec3 velocity;
		glm::vec3 oldPos;
		float mass;
		std::mutex colliderMutex;

		void setup(GameObject& owner);

		void updatePosition(float elapsedMS);

		void commitMovement();
	};
}

#endif