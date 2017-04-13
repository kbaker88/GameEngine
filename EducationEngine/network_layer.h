#ifndef NETWORK_LAYER_H
#define NETWORK_LAYER_H

#include "platform_layer.h"
//NOTE: Library for the majority of the Winsock functionality
#include <WinSock2.h>
//NOTE: Library for newer functions and structures
#include <WS2tcpip.h>

bool Network_Init();
bool Network_Connect(char* IPAddress, uint32 Port);

#endif
