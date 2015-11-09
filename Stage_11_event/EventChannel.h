#ifndef EVENTCHANNEL_H
#define EVENTCHANNEL_H

#include "stdafx.h"
#include <list>
#include "EventHandler.h"
#include <TaskManager.h>
#include <mutex>

namespace stage_11{
	/** Mallintaa tapahtumakanavaa, joka mahdollistaa viestien lähettämisen joukolle peliolioita
	*/
	class EventChannel{
	public:
		/** Luo uuden tapahtumakanavan
		*/
		EventChannel(){}

		/** Lisää olion tapahtumakanavan vastaanottajalistalle
		HUOM: kanava on lukittava writeLock-metodilla ennen tämän metodin kutsumista
		@param recipient	Osoitin vastaanottajalistalle lisättävään olioon
		*/
		void registerRecipient(EventHandler* recipient);

		/** Lähettää viestin kaikille vastaanottajalistan olioille yhtä lukuunottamatta
		HUOM: kanava on lukittava readLock-metodilla ennen tämän metodin kutsumista
		@param e		Lähetettävä viesti
		@param sender	Se olio, jolle viestiä ei lähetetä
		*/
		template <class EventType>
		void broadcastOthers(EventType e, EventHandler* sender){
			std::list<Task*> tasks;
			for (std::list<EventHandler*>::iterator i = recipients.begin(); i != recipients.end(); i++){
				//Luodaan uusi työtehtävä jokaiselle viestille
				if (*i != sender) tasks.push_back(new Forward<EventType>(e, *i));
			}
			//Lisätään tehtävät työlistaan
			TaskManager::pushTaskList(tasks);
		}

		/** Lähettää viestin kaikille vastaanottajalistan olioille
		HUOM: kanava on lukittava readLock-metodilla ennen tämän metodin kutsumista
		@param e	Lähetettävä viesti
		*/
		template <class EventType>
		void broadcast(EventType e){
			broadcastOthers<EventType>(e, nullptr);
		}

		/** Lukulukitsee tapahtumakanavan
		Tämän metodin kutsumisen jälkeen kanavan kautta voidaan turvallisesti lähettää viestejä
		Lukko on vapautettava jälkeenpäin readRelease-metodilla
		*/
		void readLock();
		/** Vapauttaa kanavan lukulukon
		*/
		void readRelease();
		/** Kirjoituslukitsee tapahtumakanavan
		Tämän metodin kutsumisen jälkeen kanavalle voidaan turvallisesti lisätä kuuntelijoita
		Lukko on vapautettava jälkeenpäin writeRelease-metodilla
		*/
		void writeLock();
		/** Vapauttaa kanavan kirjoituslukon
		*/
		void writeRelease();
	private:
		EventChannel(const EventChannel& other) = delete;
		EventChannel& operator= (const EventChannel& other) = delete;

		/** Lista tämän tapahtumakanavan viestien vastaanottajiksi rekisteröityneistä olioista
		*/
		std::list<EventHandler*> recipients;
		/** Tällä hetkellä vastaanottajalistaa lukevien olioiden lukumäärä
		*/
		int readers = 0;
		/** Kirjoitetaanko vastaanottajalistaan tällä hetkellä
		*/
		bool writing = false;
		/** Lukko, jolla suojataan pääsyä lukemaan tai kirjoittamaan vastaanottajalistaan
		*/
		std::mutex entry;
		/** Odottavat säikeet herätetään, kun vastaanottajalistaa voi lukea
		*/
		std::condition_variable canRead;
		/** Odottava säie herätetään, kun vastaanottajalistaan voi kirjoittaa
		*/
		std::condition_variable canWrite;

		/** Työtehtävä, joka käsittelee yhden kanavan kautta lähetetyn viestin
		*/
		template <class EventType>
		class Forward : public Task{
		public:
			/** Luo uuden viestityön
			@param e		Lähetettävä viesti
			@param handler	Viestin käsittelevä olio
			*/
			Forward(EventType e, EventHandler* handler) : e(e), handler(handler){}
			TASK_EXECUTE{
				handler->handleEvent(e);
			}
		private:
			/** Lähetetty viesti
			*/
			EventType e;
			/** Viestin vastaanottaja
			*/
			EventHandler* handler;
		};
	};
}

#endif