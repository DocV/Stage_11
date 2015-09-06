#ifndef STATICGEOMETRYCOMPONENT_H
#define STATICGEOMETRYCOMPONENT_H

#include "stdafx.h"
#include "PhysicsComponent.h"

#define STATICGEOMETRYCOMPONENT_ID 5

namespace stage_11{
	class StaticGeometryComponent : public Component, public EventHandler{
	public:
		StaticGeometryComponent(GameObject& owner, float radius) :
			Component(owner){
			setup(owner);
			coll = new stage_common::SphereCollider(radius, transform->getPosition());

		}

		StaticGeometryComponent(GameObject& owner, glm::vec3 size) :
			Component(owner){
			setup(owner);
			coll = new stage_common::AABBCollider(size, transform->getPosition());

		}

		virtual int id(){
			return STATICGEOMETRYCOMPONENT_ID;
		}

		virtual void doUpdate(){
			std::unique_lock<std::mutex> lock(updatemutex);
			coll->center = transform->getPosition();
		}

		virtual void handleEvent(const Event& ev){
			//Käsitellään vain törmäysviestit
			if (ev.getEventType() != PHYSICS_COLLISION_EVENT_TYPE) return;
			const PhysicsComponent::CollisionEvent& collEv = (const PhysicsComponent::CollisionEvent&)ev;
			//Jos tilaa ei vielä ole päivitetty tämän ruudunpäivityksen aikana, päivitetään se nyt

			std::unique_lock<std::mutex> ownlock(updatemutex, std::defer_lock);
			std::unique_lock<std::mutex> otherlock(collEv.sender.colliderMutex, std::defer_lock);
			std::lock(ownlock, otherlock);

			if (!coll->checkCollision(*collEv.sender.coll)) return;
			//Kimmotetaan toinen kappale tästä
			collEv.sender.velocity = stage_common::Collisions::reflect(collEv.sender.velocity, 
				coll->getCollisionNormal(*collEv.sender.getCollider(), collEv.sender.velocity));
			stage_common::Collisions::backOff(*collEv.sender.coll, -1.0f * collEv.sender.velocity, *coll);
		}
	private:
		stage_common::Collider* coll;
		Transform* transform;
		std::mutex updatemutex;

		void setup(GameObject& owner){
			transform = (Transform*)(owner.getComponentByID(TRANSFORM_ID));
			PhysicsComponent::getCollisionEventChannel().writeLock();
			PhysicsComponent::getCollisionEventChannel().registerRecipient(this);
			PhysicsComponent::getCollisionEventChannel().writeRelease();
		}

	};
}

#endif