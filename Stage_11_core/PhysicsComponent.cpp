#include "PhysicsComponent.h"

using namespace stage_11;

PhysicsComponent::PhysicsComponent(GameObject& owner, float radius, glm::vec3 initialV, float mass) :
Component(owner), velocity(initialV), mass(mass){
	setup(owner);
	coll = new stage_common::SphereCollider(radius, transform->getPosition());
}
PhysicsComponent::PhysicsComponent(GameObject& owner, glm::vec3 size, glm::vec3 initialV, float mass) :
Component(owner), velocity(initialV), mass(mass){
	setup(owner);
	coll = new stage_common::AABBCollider(size, transform->getPosition());
}
PhysicsComponent::~PhysicsComponent(){
	delete coll;
}
void PhysicsComponent::doUpdate(float elapsedMS){
	updatePosition(elapsedMS);
	//Lukulukitaan tapahtumakanava
	getCollisionEventChannel().readLock();
	CollisionEvent e(*this, elapsedMS);
	//Ilmoitetaan muuttuneesta sijainnista
	getCollisionEventChannel().broadcastOthers(e, this);
	//Vapautetaan lukko
	getCollisionEventChannel().readRelease();
}
void PhysicsComponent::doRender(){
	updatedThisFrame = false;
}
void PhysicsComponent::handleEvent(const Event& ev){
	//Käsitellään vain törmäysviestit
	if (ev.getEventType() != PHYSICS_COLLISION_EVENT_TYPE) return;
	const CollisionEvent& collEv = (const CollisionEvent&)ev;
	//Jos tilaa ei vielä ole päivitetty tämän ruudunpäivityksen aikana, päivitetään se nyt
	updatePosition(collEv.elapsedMS);

	std::unique_lock<std::mutex> ownlock(colliderMutex, std::defer_lock);
	std::unique_lock<std::mutex> otherlock(collEv.sender.colliderMutex, std::defer_lock);
	//Lukitaan oma ja toisen fysiikkaolion lukko atomisesti lukkiutumisen estämiseksi
	std::lock(ownlock, otherlock);

	if (!coll->checkCollision(*collEv.sender.coll)) return;
	//Lasketaan törmäyksen aiheuttamat muutokset nopeuksiin ja sijaintiin
	stage_common::Collisions::collisionVelocityChange(velocity, mass, collEv.sender.velocity, collEv.sender.mass);
	stage_common::Collisions::backOff(*collEv.sender.coll, -1.0f * collEv.sender.velocity, *coll);
	//Synkronoidaan sijainnin muutos omistajaolion sijainnin kanssa
	collEv.sender.commitMovement();
}
void PhysicsComponent::setup(GameObject& owner){
	transform = (Transform*)(owner.getComponentByID(TRANSFORM_ID));
	//Kirjoituslukitaan tapahtumakanava ja lisätään sen vastaanottajalistaan oma osoite
	getCollisionEventChannel().writeLock();
	getCollisionEventChannel().registerRecipient(this);
	getCollisionEventChannel().writeRelease();
}
void PhysicsComponent::updatePosition(float elapsedMS){
	std::unique_lock<std::mutex> lock(colliderMutex);
	if (updatedThisFrame) return;
	//Haetaan sijainti, päivitetään se ja tallennetaan se törmäyshahmoon ja isäntäolion sijaintikomponenttiin
	oldPos = transform->getPosition();
	glm::vec3 translation = velocity * elapsedMS;
	coll->center = oldPos + translation;
	//Sijainninmuutos translate-metodilla, koska muut komponentit voivat muuttaa sijaintia rinnakkain
	transform->translate(translation);
	oldPos = coll->center;
	updatedThisFrame = true;
}
void PhysicsComponent::commitMovement(){
	glm::vec3 translation = coll->center - oldPos;
	transform->translate(translation);
	oldPos = coll->center;
}