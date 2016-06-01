#include "stdafx.h"
#include "UserServerThread.h"
#include <string>


UserServerThread::UserServerThread(shared_ptr<RemoteUser> remoteUser, list<shared_ptr<ParentGameEngine>> *gamesList) : remoteUser(remoteUser), gamesList(gamesList)
{
}

UserServerThread::~UserServerThread()
{
}

void UserServerThread::launchUserThread()
{
	thread(&UserServerThread::run, this).detach();
}

void UserServerThread::run()
{
	cout << "Rozpoczeto watek uzytkownika ";
	
	ConnectionStatusMsg msg;
	msg.cmd = Cmds::connStatus;
	msg.status = "accepted";
	sf::Packet packet;
	packet << msg.cmd << msg.status;
	remoteUser->send(packet);

	int cmd = Cmds::connStatus;
	
	while (cmd != Cmds::endServer) //endGame JU¯ NIE ROZ£¥CZA!!
	{
		//TODO: Dorobiæ roz³¹czanie na timeout
		if(cmd == Cmds::startGame) startNewGame(packet);
		else if (cmd == Cmds::move) forwardMove(packet);
		else if (cmd == Cmds::getGamesList) sendWaitingGames(packet);
		else if (cmd == Cmds::connectToGame) connectToGame(packet);

		packet.clear();
		packet = remoteUser->receive();
		packet >> cmd;
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
	cout << "koniec gry" << endl;
}

void UserServerThread::startNewGame(sf::Packet packet)
{
	cout << endl << "Rozpoczecie nowej gry" << endl;
	StartGame msg;
	packet >> msg.gameType >> msg.playersNumber >> msg.userIds;
	cout << msg.gameType << endl;
	if(msg.gameType == GameType::single)
	{
		remoteUser->setStartPosition(sf::Vector2i(5, 2));
		game = shared_ptr<ParentGameEngine> (new SingleGame(remoteUser, getCurrentGameId()));
		gamesList->push_back(game);
		game->startThread();
	}
	else if (msg.gameType == GameType::cooperation)
	{
		game = shared_ptr<ParentGameEngine>(new CooperationGame(remoteUser, getCurrentGameId(), msg.playersNumber));
		gamesList->push_back(game);
		game->startThread();
	}
}

void UserServerThread::forwardMove(sf::Packet packet)
{
	MoveMsg msg;
	packet >> msg.moveType;
	if (msg.moveType == MoveType::DOWN)
	{
		game->registerMove(shared_ptr<UserMove>(new UserMove(remoteUser, DOWN)));
	}
	else if (msg.moveType == MoveType::LEFT)
	{
		game->registerMove(shared_ptr<UserMove>(new UserMove(remoteUser, LEFT)));
	}
	else if (msg.moveType == MoveType::DROP)
	{
		game->registerMove(shared_ptr<UserMove>(new UserMove(remoteUser, DROP)));
	}
	else if (msg.moveType == MoveType::RIGHT)
	{
		game->registerMove(shared_ptr<UserMove>(new UserMove(remoteUser, RIGHT)));
	}
	else if (msg.moveType == MoveType::ROTATE)
	{
		game->registerMove(shared_ptr<UserMove>(new UserMove(remoteUser, ROTATE)));
	}
}

void UserServerThread::sendWaitingGames(sf::Packet packet)
{
	
	GamesList msg;
	msg.cmd = Cmds::getGamesList;
	string gamesIds = "";
	string nicknames = "";

	for (shared_ptr<ParentGameEngine> game : *gamesList)
	{
		
		if(game->gameType == GameType::cooperation)
		{
			gamesIds += std::to_string(game->gameId) + ";";
			string gameUsersNicknames = "";
			for (shared_ptr<RemoteUser> user : game->usersList)
			{
				gameUsersNicknames += user->getNick() + ", ";
			}
			nicknames+= gameUsersNicknames + "|";
		}

	}
	msg.gamesIds = gamesIds;
	msg.nickNames = nicknames;
	sf::Packet gamesPacket;
	gamesPacket << msg.cmd << msg.gamesIds << msg.nickNames;
	remoteUser->send(gamesPacket);
}

void UserServerThread::connectToGame(sf::Packet packet)
{
	ConnectToGame conn;
	packet >> conn.gameId;
	cout << "connect: " << conn.gameId << endl;
	for (shared_ptr<ParentGameEngine> game : *gamesList)
	{
		if (game->gameId == conn.gameId)
		{
			cout << "connected" << endl;
			game->addPlayer(remoteUser);
			break;
		}
	}
}

int UserServerThread::getCurrentGameId()
{
	static int gameId;
	gameId++;
	return gameId;
}