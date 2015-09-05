#ifndef COMPONENT_H
#define COMPONENT_H

#include "stdafx.h"
#include <Task.h>

namespace stage_11{
	class GameObject;
	class Component{
	public:
		virtual void doUpdate(float elapsedMS){}
		virtual void doRender(){}
		virtual int id() = 0;

		Component(GameObject& owner);

		Task* update(float elapsedMS){
			return new Update(this, elapsedMS);
		}
		Task* render(){
			return new Render(this);
		}
	protected:
		class Update : public Task{
		public:
			TASK_EXECUTE {
				c->doUpdate(elapsedMS);
			};
			Update(Component*c, float elapsedMS) :c(c), elapsedMS(elapsedMS){}
		private:
			Component* c;
			float elapsedMS;
		};
		class Render : public Task{
		public:
			TASK_EXECUTE{
				c->doRender();
			}
				Render(Component*c) :c(c){}
		private:
			Component* c;
		};
	private:
		Component(const Component& other) = delete;
		Component& operator= (const Component& other) = delete;
	};

}

#endif