#ifndef GAMELOOP_H
#define GAMELOOP_H

#include "stdafx.h"
#include <iostream>
#include <thread>

namespace stage_11{
	class Gameloop{
	public:
		Gameloop(unsigned int threads = 1) : threadcount(threads){

		}

		void start(){

		}

		void loop(){

		}
	private:
		unsigned int threadcount;
	};
}

#endif