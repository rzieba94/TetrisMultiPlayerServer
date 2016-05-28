#pragma once
#include "RemoteUser.h"

class UserServerThread
{
public:
	UserServerThread(shared_ptr<RemoteUser> remoteUser);
	~UserServerThread();
	void launchUserThread();
private: 
	void run();
};

