#include "../include/_client.h"

#include "../include/TLV.h"
#include "../include/json.hpp"

#include <ostream>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <iostream>
#include <sstream>
#include <string>
using std::cin;
using std::cout;
using std::endl;
using std::string;



int tcpConnect(void);
int sendTLV(int,const TLV&);

int main(int argc, char *argv[]){
    int clientfd = tcpConnect();
	while(1){
        TLV msg;
#if 1
		getline(cin, msg.value);
		cout << ">> pls input some message:";
        msg.type = TYPE_RECOMMEND;
        msg.len = msg.value.size();
        sendTLV(clientfd, msg);
        char buff[65535] = {0};
		recv(clientfd, buff, sizeof(buff), 0);
		printf("recv msg from server: %s\n", buff);
#endif    
#if 0
        msg.type = TYPE_SEARCH;
        msg.value = "第一班";
        msg.len = msg.value.size();

        sendTLV(clientfd, msg);
	
        char buff[65535] = {0};
		recv(clientfd, buff, sizeof(buff), 0);
		printf("recv msg from server: %s\n", buff);
        break;
#endif
	}

	close(clientfd);
	return 0;

}





int tcpConnect(void){
	int clientfd = ::socket(AF_INET, SOCK_STREAM, 0);
	if(clientfd < 0) 
    {
		perror("socket");
		return -1;
	}

	struct sockaddr_in serverAddr;
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(8888);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	socklen_t length = sizeof(serverAddr);

	if(::connect(clientfd,(struct sockaddr*)&serverAddr, length) < 0) {
		perror("connect");
		close(clientfd);
		return -1;
	}
	printf("conn has connected!...\n");
    return clientfd;
}

int sendTLV(int clientfd,const TLV& msg){
    send(clientfd, &msg.type, sizeof(msg.type), 0);
    send(clientfd, &msg.len, sizeof(msg.len), 0);
    send(clientfd, msg.value.data(), msg.len, 0);
    return 0;
}
