#include "stdafx.h"
#include "MainServerThread.h"


MainServerThread::MainServerThread(int portNumber) : portNumber(portNumber)
{
}


MainServerThread::~MainServerThread()
{
}

void MainServerThread::launchServerThread()
{
	serverThread = thread(&MainServerThread::run, this);
}

void MainServerThread::run()
{
	cout << "Rozpoczeto nasluchiwanie na porcie: " << portNumber << endl;

	while (true)
	{
		sf::TcpListener listener;
		if (listener.listen(portNumber) != sf::Socket::Done)
		{
			cout << "Podczas oczekiwania na polaczenie wystapil blad.";
		}

		shared_ptr<sf::TcpSocket> clientSocket = shared_ptr<sf::TcpSocket>(new sf::TcpSocket);
		if (listener.accept(*clientSocket) != sf::Socket::Done)
		{
			cout << "Podczas laczenia do serwera wystapil blad.";
		}
		sf::Packet packet;
		clientSocket->receive(packet);
		string cmd;
		string nick;
		packet >> cmd >> nick;
		if (cmd == "connect" && !nick.empty())
		{
			acceptNewConnection(nick, clientSocket);
		}
		else
		{
			cout << "Podczas tworzenia watku klienta wystapil blad.";
		}
	}
}

void MainServerThread::acceptNewConnection(string nick, shared_ptr<sf::TcpSocket> clientSocket)
{
	shared_ptr<RemoteUser> remoteUser = shared_ptr<RemoteUser>(new RemoteUser(nick, clientSocket));
	shared_ptr<UserServerThread> userServerThread = shared_ptr<UserServerThread> (new UserServerThread(remoteUser));
	userThreadsList.push_back(userServerThread);
	userServerThread->launchUserThread();
}

thread * MainServerThread::getServerThread()
{
	return & serverThread;
}