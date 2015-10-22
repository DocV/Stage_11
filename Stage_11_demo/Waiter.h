#ifndef WAITER_H
#define WAITER_H

#define WAITER_ID 7

#include "stdafx.h"
#include <Component.h>

namespace stage_11{
	class Waiter : public Component{
	public:
		Waiter(GameObject& owner, int limit) :
			Component(owner), limit(limit){}

		virtual void doUpdate(float elapsedMS){
			for (int i = 3; i <= limit; i++){
				for (int j = 2; j < i; j++){
					if (i % j == 0) break;
				}
			}
		}

		virtual int id(){ return WAITER_ID; }
	private:
		int limit;
	};

}

#endif