// TetrisMultiplayerServer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "MainServerThread.h"

int main()
{
	int serverPort;

	cout << "TETRIS MULTIPLAYER SERVER" << endl << endl;
	cout << "Autorzy: " << endl;
	cout << "Marcin Muskala" << endl;
	cout << "Marek Nawrot" << endl;
	cout << "Michal Sliwa" << endl;
	cout << "Rafal Zieba" << endl << endl;

	cout << "Podaj numer portu dla serwera: ";
	cin >> serverPort;

	MainServerThread mainServerThread(serverPort);
	mainServerThread.launchServerThread();
	mainServerThread.getServerThread()->join();
}

