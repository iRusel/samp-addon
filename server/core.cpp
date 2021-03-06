#pragma once



#include "core.h"





boost::shared_ptr<amxCore> gCore;


extern boost::shared_ptr<amxDebug> gDebug;
extern boost::shared_ptr<amxPool> gPool;





amxCore::amxCore()
{
	gDebug = boost::shared_ptr<amxDebug>(new amxDebug());
	gPool = boost::shared_ptr<amxPool>(new amxPool());

	gDebug->Log("Core constructor called");

	this->mutexInstance = boost::shared_ptr<boost::mutex>(new boost::mutex());
	this->threadInstance = boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&amxCore::Thread)));
}



amxCore::~amxCore()
{
	gDebug->Log("Core destructor called");

	this->threadInstance->interruption_requested();
	this->mutexInstance->destroy();
}



void amxCore::Thread()
{
	assert(gCore->getThreadInstance()->get_id() == boost::this_thread::get_id());

	gDebug->Log("Started Core thread with id 0x%x", gCore->getThreadInstance()->get_thread_info()->id);

	while(!gPool->pluginInit)
		boost::this_thread::sleep_for(boost::chrono::seconds(1));

	std::string ip = gPool->getServerVar("serverIP", gCore->getMutexInstance());
	unsigned int port = atoi(gPool->getServerVar("serverPort", gCore->getMutexInstance()).c_str());
	unsigned int maxclients = atoi(gPool->getServerVar("maxClients", gCore->getMutexInstance()).c_str());

	gDebug->Log("Addon TCP server started on %s:%i with maxclients: %i", ip.c_str(), port, maxclients);
}