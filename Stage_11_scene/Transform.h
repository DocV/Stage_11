#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "stdafx.h"
#include "Component.h"
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <mutex>

#define TRANSFORM_ID 1

/** Peliolion sijaintia 3D-pelimaailmassa ylläpitävä komponentti
*/
namespace stage_11{
	class Transform : public Component{
	public:
		/** Luo uuden sijaintikomponentin.
		@param owner	Komponentin omistava peliolio
		@param matrix	Peliolion sijainti simulaation alussa
		*/
		Transform(GameObject& owner, const glm::mat4& matrix = glm::mat4()) : Component(owner), transform(matrix){}

		/** Hakee peliolion sijaintimatriisin
		@returns	Peliolion sijaintia ja orientaatiota esittävä 4x4-matriisi
		*/
		glm::mat4 getMatrix(){
			std::unique_lock<std::mutex>(tfmutex);
			return transform;
		}

		/** Asettaa peliolion sijaintimatriisin
		@param matrix	Peliolion sijaintia ja orientaatiota esittävä 4x4-matriisi
		*/
		void setMatrix(const glm::mat4& matrix){
			std::unique_lock<std::mutex>(tfmutex);
			transform = matrix;
		}

		/** Hakee peliolion sijaintivektorin
		@returns	Peliolion sijaintia esittävä kolmipaikkainen vektori
		*/
		glm::vec3 getPosition(){
			std::unique_lock<std::mutex>(tfmutex);
			return glm::vec3(transform[3]);
		}

		/** Siirtää pelioliota haluttuun suuntaan
		@param direction	Vektori, joka ilmoittaa mihin suuntaan ja miten kauas pelioliota siirretään
		*/
		void translate(glm::vec3 direction){
			std::unique_lock<std::mutex>(tfmutex);
			transform = glm::translate(transform, direction);
		}

		/** Hakee sijaintikomponentin komponenttitunnuksen
		@returns	Sijaintikomponentin komponenttitunnus
		*/
		virtual int id(){
			return TRANSFORM_ID;
		}
	private:
		/** Peliolion sijaintia ja orientaatiota kuvaava 4x4-matriisi
		*/
		glm::mat4 transform;
		/** Sijaintimatriisia suojaava lukko
		*/
		std::mutex tfmutex;
	};
}

#endif