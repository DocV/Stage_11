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

		PhysicsComponent(GameObject& owner, float radius, glm::vec3 initialV, float mass) :
			Component(owner), velocity(initialV), mass(mass){
			setup(owner);
			coll = new stage_common::SphereCollider(radius, transform->getPosition());

		}

		PhysicsComponent(GameObject& owner, glm::vec3 size, glm::vec3 initialV, float mass) :
			Component(owner), velocity(initialV), mass(mass){
			setup(owner);
			coll = new stage_common::AABBCollider(size, transform->getPosition());

		}

		void doUpdate(float elapsedMS){
			updatePosition(elapsedMS);
			getCollisionEventChannel().readLock();
			CollisionEvent e(*this, elapsedMS);
			getCollisionEventChannel().broadcastOthers(e, this);
			getCollisionEventChannel().readRelease();
		}

		void doRender(){
			updatedThisFrame = false;
		}

		void handleEvent(const Event& ev){
			//Käsitellään vain törmäysviestit
			if (ev.getEventType() != PHYSICS_COLLISION_EVENT_TYPE) return;
			const CollisionEvent& collEv = (const CollisionEvent&)ev;
			//Jos tilaa ei vielä ole päivitetty tämän ruudunpäivityksen aikana, päivitetään se nyt
			updatePosition(collEv.elapsedMS);

			std::unique_lock<std::mutex> ownlock(colliderMutex, std::defer_lock);
			std::unique_lock<std::mutex> otherlock(collEv.sender.colliderMutex, std::defer_lock);
			std::lock(ownlock, otherlock);

			if (!coll->checkCollision(*collEv.sender.coll)) return;
			stage_common::Collisions::collisionVelocityChange(velocity, mass, collEv.sender.velocity, collEv.sender.mass);
			stage_common::Collisions::backOff(*collEv.sender.coll, -1.0f * collEv.sender.velocity, *coll);
			collEv.sender.commitMovement();
		}

		stage_common::Collider* getCollider(){
			return coll;
		}

		glm::vec3 getVelocity(){
			return velocity;
		}
	private:
		stage_common::Collider* coll;
		Transform* transform;
		bool updatedThisFrame = false;
		glm::vec3 velocity;
		glm::vec3 oldPos;
		float mass;
		std::mutex colliderMutex;

		void setup(GameObject& owner){
			transform = (Transform*)(owner.getComponentByID(TRANSFORM_ID));
			getCollisionEventChannel().writeLock();
			getCollisionEventChannel().registerRecipient(this);
			getCollisionEventChannel().writeRelease();
		}

		void updatePosition(float elapsedMS){
			std::unique_lock<std::mutex> lock(colliderMutex);
			if (updatedThisFrame) return;
			oldPos = transform->getPosition();
			glm::vec3 translation = velocity * elapsedMS;
			coll->center = oldPos + translation;
			transform->translate(translation);
			oldPos = coll->center;
			updatedThisFrame = true;
		}

		void commitMovement(){
			glm::vec3 translation = coll->center - oldPos;
			transform->translate(translation);
			oldPos = coll->center;
		}
	};
}

#endif