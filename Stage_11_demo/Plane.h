#ifndef PLANE_H
#define PLANE_H

#include "stdafx.h"
#include <vector>
#include <glm\glm.hpp>

/*Sisältää litteän pinnan 3D-mallin datan*/
namespace stage_11{
	/** Litteän pinnan verteksit*/
	static std::vector<glm::vec3> plane_vertices = {
		glm::vec3(-1.000000, 0.000000, 1.000000),
		glm::vec3(1.000000, 0.000000, 1.000000),
		glm::vec3(-1.000000, 0.000000, -1.000000),
		glm::vec3(1.000000, 0.000000, -1.000000)
	};
	/** Pinnan tahkot
	Jokainen kolmen numeron sarja määrittelee yhden kolmion, jonka kärjet ovat numeroiden ilmaisemat verteksit*/
	static std::vector<int> plane_faces = {
		2, 4, 3,
		1, 2, 3
	};
	/** Yhdistää tahkot ja verteksit yhtenäiseksi verteksilistaksi
	@returns	Litteää pintaa kuvaavan 3D-mallin verteksit
	*/
	static std::vector<glm::vec3> generate_plane_vertices(){
		std::vector<glm::vec3> ret;
		//Jokaista tahkoa kohden: lisää listaan järjestyksessä tahkon verteksit
		for (unsigned int i = 0; i < plane_faces.size(); i++){
			ret.push_back(plane_vertices[plane_faces[i] - 1]);
		}
		return ret;
	}
	/** Arpoo vertekseille väriarvot
	@returns	Litteää pintaa kuvaavan 3D-mallin värit
	*/
	static std::vector<glm::vec3> generate_plane_colors(){
		std::vector<glm::vec3> ret;
		for (unsigned int i = 0; i < plane_faces.size(); i++){
			ret.push_back(glm::vec3(static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
				static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
				static_cast <float> (rand()) / static_cast <float> (RAND_MAX)));
		}
		return ret;
	}
}
#endif