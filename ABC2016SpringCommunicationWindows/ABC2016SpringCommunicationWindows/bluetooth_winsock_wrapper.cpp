
#include"bluetooth_winsock_wrapper.hpp"


	int BthServer::InitWSAdata() {
		mWsaData = { 0 };
		mInfo = { 0 };
		mSa = { 0 };
		mBluetoothspp_uuid = { 0x11111111, 0x1111, 0x1111, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x23 };
		if (WSAStartup(MAKEWORD(2, 2), &mWsaData) !=0) {
			return -1;
		}
		return 0;
	}

	int BthServer::CreateSocket() {
		if (INVALID_SOCKET==(mListen_sock = socket(AF_BTH, SOCK_STREAM, BTHPROTO_RFCOMM))) {
			return -1;
		}
		return 0;
	}

	int BthServer::SetBluetoothSock() {
		mSa.addressFamily = AF_BTH;
		mSa.port = BT_PORT_ANY;
		if (SOCKET_ERROR==bind(mListen_sock, (SOCKADDR*)&mSa, sizeof(mSa))) {
			return -1;
		}
		return 0;
	}

	int BthServer::BluetoothGetsockname() {
		int socksize = sizeof(mSa);
		if (-1 == getsockname(mListen_sock, (SOCKADDR*)&mSa, &socksize)) {
			return -1;
		}
		return 0;
	}



	int BthServer::WrapWSASetService() {
		mInfo.LocalAddr.lpSockaddr = (LPSOCKADDR)&mSa;
		mInfo.LocalAddr.iSockaddrLength = sizeof(mSa);
		mInfo.iSocketType = SOCK_STREAM;
		mInfo.iProtocol = BTHPROTO_RFCOMM;

		mQuset.dwSize = sizeof(WSAQUERYSET);
		mQuset.dwOutputFlags = 0;
		mQuset.lpszServiceInstanceName = (LPWSTR)"Server";
		mQuset.lpServiceClassId = (LPGUID)&mBluetoothspp_uuid;
		mQuset.lpVersion = NULL;
		mQuset.lpszComment = NULL;
		mQuset.dwNameSpace = NS_BTH;
		mQuset.lpNSProviderId = NULL;
		mQuset.lpszContext = NULL;
		mQuset.dwNumberOfProtocols = 0;
		mQuset.lpafpProtocols = NULL;
		mQuset.lpszQueryString = NULL;
		mQuset.dwNumberOfCsAddrs = 1;
		mQuset.lpcsaBuffer = &mInfo;
		mQuset.lpBlob = NULL;
		if (0 != WSASetService(&mQuset, RNRSERVICE_REGISTER, 0)) {
			return -1;
		}
		return 0;
	}

	int BthServer::ListenAcceptConnect() {
		listen(mListen_sock, 1);
		SOCKADDR_BTH sa2;
		int ilen = sizeof(sa2);
		if (INVALID_SOCKET==(mSock = accept(mListen_sock, (SOCKADDR*)&sa2, &ilen))) {
			return -1;
		}
		return 0;
	}

	int BthServer::SockRecv(char *buf, int Flag) {
		int ret;
		ret = recv(mSock, buf, sizeof(buf), Flag);
		return ret;
	}

	int BthServer::SockSend(char *buf,int Flag){
		int ret;
		ret = send(mSock, buf, sizeof(buf), Flag);
		return ret;
	}

	void BthServer::DestroySock() {
		closesocket(mListen_sock);
		closesocket(mSock);
		WSACleanup();
	}
	

