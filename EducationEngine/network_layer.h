#ifndef NETWORK_LAYER_H
#define NETWORK_LAYER_H

#include "utility.h"
#include <WinSock2.h>
#include <WS2tcpip.h>

bool Network_Init();
bool Network_Connect(char* IPAddress, uint32 Port);

#endif
