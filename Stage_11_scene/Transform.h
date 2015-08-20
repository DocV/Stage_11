#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "stdafx.h"
#include "Component.h"
#include <glm\glm.hpp>
#include <mutex>
#include <glm\gtc\matrix_transform.inl>

#define TRANSFORM_ID 1

namespace stage_11{
	class Transform : public Component{
	public:
		Transform(GameObject& owner, const glm::mat4& matrix = glm::mat4()) : Component(owner), transform(matrix){}

		glm::mat4 getMatrix(){
			std::unique_lock<std::mutex>(tfmutex);
			return transform;
		}

		void setMatrix(const glm::mat4& matrix){
			std::unique_lock<std::mutex>(tfmutex);
			transform = matrix;
		}

		glm::vec3 getPosition(){
			std::unique_lock<std::mutex>(tfmutex);
			return glm::vec3(transform[3]);
		}

		void translate(glm::vec3 direction){
			std::unique_lock<std::mutex>(tfmutex);
			transform = glm::translate(transform, direction);
		}

		virtual int id(){
			return TRANSFORM_ID;
		}
	private:
		glm::mat4 transform;
		std::mutex tfmutex;
	};
}

#endif