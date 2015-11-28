
#include"bluetooth_winsock_wrapper.hpp"
#include<stdio.h>

int main() {

	BthServer bluetoothsrv;
	int count = 0;
	int ret = 0;
	char buf[2024];
	if (-1 == bluetoothsrv.InitWSAdata()) {
		return -1;
	}
	if (-1 == bluetoothsrv.CreateSocket()) {
		return -1;
	}
	if (-1 == bluetoothsrv.SetBluetoothSock()) {
		return -1;
	}
	if (-1 == bluetoothsrv.BluetoothGetsockname()) {
		return -1;
	}
	if (-1 == bluetoothsrv.WrapWSASetService()) {
		return -1;
	}
	if (-1 == bluetoothsrv.ListenAcceptConnect()) {
		return -1;
	}
	printf("start\n");
	while (count == 0) {
		memset(buf, '\0', sizeof(buf));
		ret = bluetoothsrv.SockRecv(buf, 0);
		if (ret > 0)
			printf("受信メッセージ→%s\n", buf);
		memset(buf, '\0', sizeof(buf));
		ret = 0;

		//printf("送信メッセージ→");
		//scanf("%s", buf);
		bluetoothsrv.SockSend(buf, 0);
	}


	bluetoothsrv.DestroySock();

	return 0;
}