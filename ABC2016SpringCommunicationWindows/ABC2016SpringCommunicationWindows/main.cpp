
#include"bluetooth_winsock_wrapper.hpp"
#include<cstdio>

int main() {

    bthserver bluetooth_server_;
    int count_ = 0;
    int ret_ = 0;
    char buf_[2024];
    if (-1 == bluetooth_server_.init_wsadata()) {
        return -1;
    }
    if (-1 == bluetooth_server_.create_socket()) {
        return -1;
    }
    if (-1 == bluetooth_server_.set_bluetooth_sock()) {
        return -1;
    }
    if (-1 == bluetooth_server_.get_bluetooth_sockname()) {
        return -1;
    }
    if (-1 == bluetooth_server_.wrap_wsa_set_service()) {
        return -1;
    }
    if (-1 == bluetooth_server_.listen_accept_connect()) {
        return -1;
    }
    printf("start\n");

    while (count_ == 0) {
        memset(buf_, '\0', sizeof(buf_));
        ret_ = bluetooth_server_.recv_data(buf_,sizeof(buf_), 0);
        if (ret_ > 0) {
            printf("受信メッセージ→%s\n", buf_);
        }
        //memset(buf_, '\0', sizeof(buf_));
        ret_ = 0;
        //printf("送信メッセージ→");
        //scanf("%s", buf);
        bluetooth_server_.send_data("",sizeof(""), 0);
        Sleep(10);
    }


    bluetooth_server_.destroy_sock();

    return 0;
}