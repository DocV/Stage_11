#ifndef COMPONENT_H
#define COMPONENT_H

#include "stdafx.h"
#include <Task.h>

namespace stage_11{
	class GameObject;
	class Component{
	public:
		virtual void doUpdate(){}
		virtual void doRender(){}
		virtual int id() = 0;

		Component(GameObject& owner);

		Task* update(){
			return new Update(this);
		}
		Task* render(){
			return new Render(this);
		}
	protected:
		class Update : public Task{
		public:
			TASK_EXECUTE {
				c->doUpdate();
			}
			Update(Component*c):c(c){}
		private:
			Component* c;
		};
		class Render : public Task{
		public:
			TASK_EXECUTE{
				c->doUpdate();
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