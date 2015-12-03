#include "EventChannel.h"

using namespace stage_11;

void EventChannel::registerRecipient(EventHandler* recipient){
	recipients.push_back(recipient);
}
void EventChannel::readLock(){
	std::unique_lock<std::mutex> lock(entry);
	//Odotetaan, kunnes kirjoittajia ei ole
	if (writing) canRead.wait(lock, [this]{return !writing; });
	readers++;
}
void EventChannel::readRelease(){
	std::unique_lock<std::mutex> lock(entry);
	readers--;
	//Herätetään kirjoittaja, jos ei lukijoita
	if (readers < 1) canWrite.notify_one();
}
void EventChannel::writeLock(){
	std::unique_lock<std::mutex> lock(entry);
	//Odotetaan, kunnes kirjoittajia ja lukijoita ei ole
	if (writing || readers > 0) canWrite.wait(lock, [this]{return !writing && readers < 1; });
	writing = true;
}
void EventChannel::writeRelease(){
	std::unique_lock<std::mutex> lock(entry);
	writing = false;
	//Herätetään yksi kirjoittaja ja kaikki lukijat; nopein saa lukon
	canWrite.notify_one();
	canRead.notify_all();
}