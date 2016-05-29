#pragma once
#include <string>

struct ClientHelloMsg
{
	string cmd;
	string nick;
};

struct ConnectionStatusMsg
{
	string cmd;
	string status;
};

struct MoveMsg
{
	string cmd;
	string moveType;
	string userId;
	int dropCount;
};

struct StartGame
{
	string cmd;
	string gameType;
};