#include "stdafx.h"
#include "MainServerThread.h"


MainServerThread::MainServerThread(int portNumber) : portNumber(portNumber), gamesList()
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
		if (listener.accept(*clientSocket.get()) != sf::Socket::Done)
		{
			cout << "Podczas laczenia do serwera wystapil blad.";
		}
		sf::Packet packet;
		clientSocket->receive(packet);
		ClientHelloMsg msg;
		packet >> msg.cmd >> msg.nick;
		if (msg.cmd == Cmds::connect && !msg.nick.empty())
		{
			acceptNewConnection(msg.nick, clientSocket);
		}
		else
		{
			rejectNewConnection(clientSocket);
		}
	}
}

void MainServerThread::acceptNewConnection(string nick, shared_ptr<sf::TcpSocket> clientSocket)
{
	shared_ptr<RemoteUser> remoteUser = shared_ptr<RemoteUser>(new RemoteUser(nick, clientSocket));
	shared_ptr<UserServerThread> userServerThread = shared_ptr<UserServerThread> (new UserServerThread(remoteUser, gamesList));
	userThreadsList.push_back(userServerThread);
	userServerThread->launchUserThread();
}

void MainServerThread::rejectNewConnection(shared_ptr<sf::TcpSocket> clientSocket)
{
	cout << "Podczas tworzenia watku klienta wystapil blad.";
	ConnectionStatusMsg msg;
	msg.cmd = Cmds::connStatus;
	msg.status = "rejected";
	sf::Packet packet;
	packet << msg.cmd << msg.status;
	clientSocket->send(packet);
	clientSocket->disconnect();
}

thread * MainServerThread::getServerThread()
{
	return & serverThread;
}