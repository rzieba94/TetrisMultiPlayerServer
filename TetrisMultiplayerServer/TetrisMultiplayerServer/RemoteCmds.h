#pragma once
#include "stdafx.h"

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