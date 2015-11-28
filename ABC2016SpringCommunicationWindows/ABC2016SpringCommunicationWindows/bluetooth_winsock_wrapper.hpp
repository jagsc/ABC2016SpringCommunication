#pragma once 
#ifndef __BLUETOOTHWINSOCKWRAPPER_H_INCLUEDED__
#define __BLUETOOTHWINSOCKWRAPPER_H_INCLUEDED__

#include <iostream>
#include <WinSock2.h>
#include <ws2bth.h>

#pragma comment(lib, "Ws2_32")

class BthServer {
private:
	WSADATA mWsaData;
	SOCKADDR_BTH mSa;
	SOCKET mListen_sock;
	CSADDR_INFO mInfo;
	WSAQUERYSET mQuset;
	SOCKET mSock;
	GUID mBluetoothspp_uuid;
public:

	int InitWSAdata();

	int CreateSocket();

	int SetBluetoothSock();

	int BluetoothGetsockname();

	int WrapWSASetService();

	int ListenAcceptConnect();

	int SockRecv(char*, int);

	int SockSend(char*, int);

	void DestroySock();

};

#endif