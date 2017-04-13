#include "network_layer.h"

bool Network_Init()
{
	WSADATA wsaData;
	/*
		typedef struct WSAData {
		  WORD           wVersion;
		  WORD           wHighVersion;
		  char           szDescription[WSADESCRIPTION_LEN+1];
		  char           szSystemStatus[WSASYS_STATUS_LEN+1];
		  unsigned short iMaxSockets;
		  unsigned short iMaxUdpDg;
		  char FAR       *lpVendorInfo;
		} WSADATA, *LPWSADATA;
	*/
	uint16 VersionRequest = 2 | (2 << 8);

	int32 Result = WSAStartup(VersionRequest, &wsaData);
	if (Result != 0) {
		MessageBox(0, "Break", 0, 0);
		return 1;
	}

	struct addrinfo *result = NULL;
	struct addrinfo *ptr = NULL;
	struct addrinfo hints = {};

	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	char* Port = "27015";
	char* HostIP = "127.0.0.1";
	Result = getaddrinfo(HostIP, Port, &hints, &result);
	if (Result != 0)
	{
		MessageBox(0, "Break", 0, 0);
		return 1;
	}

	SOCKET ConnectSocket = INVALID_SOCKET;
	ptr = result;
	ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, 
		ptr->ai_protocol);

	if (ConnectSocket == INVALID_SOCKET)
	{
		freeaddrinfo(result);
		WSACleanup();
		MessageBox(0, "Break", 0, 0);
		return 1;
	}


	Result = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
	if (Result == SOCKET_ERROR)
	{
		closesocket(ConnectSocket);
		ConnectSocket = INVALID_SOCKET;
	}

	freeaddrinfo(result);
	if (ConnectSocket == INVALID_SOCKET)
	{
		WSACleanup();
		MessageBox(0, "Break", 0, 0);
		return 1;
	}
	// success connect

	//transmit..
	const uint32 DefaultBufferLength = 512;

	int recvbuflen = DefaultBufferLength;

	char *sendbuf = "Kevin";
	int StringLength = 5;
	char recvbuf[DefaultBufferLength];

	int iResult;

	// Send an initial buffer
	iResult = send(ConnectSocket, sendbuf, StringLength, 0);
	if (iResult == SOCKET_ERROR) {
		MessageBox(0, "Break", 0, 0);
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}

	//printf("Bytes Sent: %ld\n", iResult);
	Platform_TemporaryError(Platform_IntToChar(iResult));

	// shutdown the connection for sending since no more data will be sent
	// the client can still use the ConnectSocket for receiving data
	iResult = shutdown(ConnectSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		MessageBox(0, "Break", 0, 0);
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}

	// Receive data until the server closes the connection
	do {
		iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
		if (iResult > 0)
			Platform_TemporaryError(Platform_IntToChar(iResult)); //printf("Bytes received: %d\n", iResult);
		else if (iResult == 0)
			Platform_TemporaryError("Connection closed\n");
		else
			Platform_TemporaryError("recv failed: %d\n");
	} while (iResult > 0);









	// Cleanup
	Result = shutdown(ConnectSocket, SD_SEND);
	if (Result == SOCKET_ERROR)
	{
		closesocket(ConnectSocket);
		WSACleanup();
		MessageBox(0, "Break", 0, 0);
		return 1;
	}

	closesocket(ConnectSocket);
	WSACleanup();

	return 0;
}

bool Network_Connect(char* IPAddress, uint32 Port)
{

	return 0;
}