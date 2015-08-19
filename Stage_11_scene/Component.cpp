#include "stdafx.h"
#include "Component.h"
#include "GameObject.h"

using namespace stage_11;

Component::Component(GameObject& owner){
		owner.addComponent(this);
}