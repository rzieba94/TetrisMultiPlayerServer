#include "stdafx.h"
#include "UserServerThread.h"


UserServerThread::UserServerThread(shared_ptr<RemoteUser> remoteUser)
{
}


UserServerThread::~UserServerThread()
{
}

void UserServerThread::launchUserThread()
{
	thread(&UserServerThread::run, this);
}

void UserServerThread::run()
{
	cout << "Rozpoczeto watek uzytkownika ";
}
