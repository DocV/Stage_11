#ifndef MODELCOMPONENT_H
#define MODELCOMPONENT_H

#define MODELCOMPONENT_ID 3

#include "stdafx.h"
#include <Model.h>
#include <Component.h>
#include <Transform.h>
#include "GraphicsControlWrapper.h"

namespace stage_11{
	class ModelComponent : public Component{
	public:
		ModelComponent(GameObject& owner, stage_common::Model* mod) : Component(owner), mod(mod){
			tf = (Transform*)owner.getComponentByID(TRANSFORM_ID);
		}

		virtual void doRender(){
			GraphicsControlWrapper::getGlobalController().queue(mod, tf->getMatrix());
		}

		virtual int id(){
			return MODELCOMPONENT_ID;
		}
	private:
		stage_common::Model* mod;
		Transform* tf;
	};
}

#endif