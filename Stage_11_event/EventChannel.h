#ifndef EVENTCHANNEL_H
#define EVENTCHANNEL_H

#include "stdafx.h"
#include <list>
#include "EventHandler.h"
#include <TaskManager.h>
#include <mutex>

namespace stage_11{
	class EventChannel{
	public:
		EventChannel(){}

		void registerRecipient(EventHandler* recipient);

		template <class EventType>
		void broadcastOthers(EventType e, EventHandler* sender){
			std::list<Task*> tasks;
			for (std::list<EventHandler*>::iterator i = recipients.begin(); i != recipients.end(); i++){
				if (*i != sender) tasks.push_back(new Forward<EventType>(e, *i));
			}
			TaskManager::pushTaskList(tasks);
		}

		template <class EventType>
		void broadcast(EventType e){
			broadcastOthers<EventType>(e, nullptr);
		}

		void readLock();
		void readRelease();

		void writeLock();

		void writeRelease();
	private:
		EventChannel(const EventChannel& other) = delete;
		EventChannel& operator= (const EventChannel& other) = delete;

		std::list<EventHandler*> recipients;
		int readers = 0;
		bool writing = false;
		std::mutex entry;
		std::condition_variable canRead;
		std::condition_variable canWrite;

		template <class EventType>
		class Forward : public Task{
		public:
			Forward(EventType e, EventHandler* handler) : e(e), handler(handler){}
			TASK_EXECUTE{
				handler->handleEvent(e);
			}
		private:
			EventType e;
			EventHandler* handler;
		};
	};
}

#endif