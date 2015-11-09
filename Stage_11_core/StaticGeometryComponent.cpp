#include "StaticGeometryComponent.h"

using namespace stage_11;

StaticGeometryComponent::StaticGeometryComponent(GameObject& owner, float radius) :
Component(owner){
	setup(owner);
	coll = new stage_common::SphereCollider(radius, transform->getPosition());
}

StaticGeometryComponent::StaticGeometryComponent(GameObject& owner, glm::vec3 size) :
Component(owner){
	setup(owner);
	coll = new stage_common::AABBCollider(size, transform->getPosition());
}

void StaticGeometryComponent::doUpdate(){
	std::unique_lock<std::mutex> lock(updatemutex);
	//Haetaan nykyinen sijainti, koska muut komponentit voivat muuttaa sitä
	coll->center = transform->getPosition();
}

void StaticGeometryComponent::handleEvent(const Event& ev){
	//Käsitellään vain törmäysviestit
	if (ev.getEventType() != PHYSICS_COLLISION_EVENT_TYPE) return;
	const PhysicsComponent::CollisionEvent& collEv = (const PhysicsComponent::CollisionEvent&)ev;

	std::unique_lock<std::mutex> ownlock(updatemutex, std::defer_lock);
	std::unique_lock<std::mutex> otherlock(collEv.sender.colliderMutex, std::defer_lock);
	//Lukitaan molempien törmäävien olioiden lukot atomisesti
	std::lock(ownlock, otherlock);

	if (!coll->checkCollision(*collEv.sender.coll)) return;

	//Kimmotetaan toinen kappale tästä
	collEv.sender.velocity = stage_common::Collisions::reflect(collEv.sender.velocity,
		coll->getCollisionNormal(*collEv.sender.getCollider(), collEv.sender.velocity));
	stage_common::Collisions::backOff(*collEv.sender.coll, -1.0f * collEv.sender.velocity, *coll);
	collEv.sender.commitMovement();
}

void StaticGeometryComponent::setup(GameObject& owner){
	transform = (Transform*)(owner.getComponentByID(TRANSFORM_ID));
	//Kirjoituslukitaan tapahtumakanava ja lisätään sen vastaanottajalistaan oma osoite
	PhysicsComponent::getCollisionEventChannel().writeLock();
	PhysicsComponent::getCollisionEventChannel().registerRecipient(this);
	PhysicsComponent::getCollisionEventChannel().writeRelease();
}