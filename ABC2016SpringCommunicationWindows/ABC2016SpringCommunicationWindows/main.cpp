
#include"bluetooth_winsock_wrapper.hpp"
#include<stdio.h>

int main() {

	bthserver bluetoothsrv;
	int count_ = 0;
	int ret_ = 0;
	char buf_[2024];
	if (-1 == bluetoothsrv.init_wsadata()) {
		return -1;
	}
	if (-1 == bluetoothsrv.create_socket()) {
		return -1;
	}
	if (-1 == bluetoothsrv.set_bluetooth_sock()) {
		return -1;
	}
	if (-1 == bluetoothsrv.get_bluetooth_sockname()) {
		return -1;
	}
	if (-1 == bluetoothsrv.wrap_wsa_set_service()) {
		return -1;
	}
	if (-1 == bluetoothsrv.listen_accept_connect()) {
		return -1;
	}
	printf("start\n");
	while (count_ == 0) {
		memset(buf_, '\0', sizeof(buf_));
		ret_ = bluetoothsrv.recv_data(buf_,sizeof(buf_), 0);
		if (ret_ > 0) {
			printf("受信メッセージ→%s\n", buf_);
		}
		memset(buf_, '\0', sizeof(buf_));
		ret_ = 0;
		//printf("送信メッセージ→");
		//scanf("%s", buf);
		bluetoothsrv.send_data(buf_,sizeof(buf_), 0);
	}


	bluetoothsrv.destroy_sock();

	return 0;
}