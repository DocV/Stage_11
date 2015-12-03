#ifndef MODELCOMPONENT_H
#define MODELCOMPONENT_H

#define MODELCOMPONENT_ID 3

#include "stdafx.h"
#include <Model.h>
#include <Component.h>
#include <Transform.h>
#include "GraphicsControlWrapper.h"
#include <GameObject.h>

namespace stage_11{
	/** Komponenttiolio, jolla liitetään peliolioon 3D-malli
	*/
	class ModelComponent : public Component{
	public:
		/** Luo uuden 3D-mallikomponentin
		Katso oikea käyttö yläluokasta
		@param owner	Viite tämän peliolion omistavaan peliolioon
		@param mod		Osoitin tämän olion 3D-malliin
		*/
		ModelComponent(GameObject& owner, stage_common::Model* mod) : Component(owner), mod(mod){
			tf = (Transform*)owner.getComponentByID(TRANSFORM_ID);
		}
		/** Valmistelee tämän mallin piirtämisen ruudulle*/
		virtual void doRender(){
			GraphicsControlWrapper::getGlobalController().queue(mod, tf->getMatrix());
		}
		/** Palauttaa mallikomponentin komponenttitunnuksen
		@returns	Mallikomponentin komponenttitunnus
		*/
		virtual int id(){
			return MODELCOMPONENT_ID;
		}
	private:
		/** Osoitin tämän komponentin 3D-malliin*/
		stage_common::Model* mod;
		/** Osoitin tämän komponentin omistavan olion sijaintia ylläpitävään komponenttiin*/
		Transform* tf;
	};
}
#endif