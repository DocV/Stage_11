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

		void registerRecipient(EventHandler* recipient){
			recipients.push_back(recipient);
		}

		template <class EventType>
		void broadcastOthers(EventType e, EventHandler* sender){
			for (std::list<EventHandler*>::iterator i = recipients.begin(); i != recipients.end(); i++){
				if (*i =! sender) TaskManager::pushTask(new Forward<EventType>(e, *i));
			}
		}

		template <class EventType>
		void broadcast(EventType e){
			broadcastOthers<EventType>(e, nullptr);
		}

		void readLock(){
			std::unique_lock<std::mutex> lock(entry);
			if (writing) canRead.wait(lock, [this]{!writing; });
			readers++;
		}
		void readRelease(){
			std::unique_lock<std::mutex> lock(entry);
			readers--;
			if (readers < 1) canWrite.notify_one();
		}

		void writeLock(){
			std::unique_lock<std::mutex> lock(entry);
			if (writing || readers > 0) canWrite.wait(lock, [this]{!writing && readers < 1; });
			writing = true;
		}

		void writeRelease(){
			std::unique_lock<std::mutex> lock(entry);
			writing = false;
			canWrite.notify_one();
			canRead.notify_all();
		}
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