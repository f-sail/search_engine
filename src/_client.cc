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

 
void test() {
	int clientfd = ::socket(AF_INET, SOCK_STREAM, 0);
	if(clientfd < 0) 
    {
		perror("socket");
		return ;
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
		return;
	}
	printf("conn has connected!...\n");

	while(1){
        TLV msg;
		getline(cin, msg.value);
		cout << ">> pls input some message:";
		//1. 客户端先发数据 
        msg.type = TYPE_RECOMMEND;
        msg.len = msg.value.size();
        /* cout << "type = " << msg.type << "\n"; */
        /* cout <<"len = " <<  msg.len << "\n"; */
        /* cout <<"value = " <<  msg.value << "\n"; */
        /* send(clientfd, &msg, sizeof(msg), 0); */
        send(clientfd, &msg.type, sizeof(msg.type), 0);
        send(clientfd, &msg.len, sizeof(msg.len), 0);
        send(clientfd, msg.value.data(), msg.len, 0);
		
        char buff[65535] = {0};
		recv(clientfd, buff, sizeof(buff), 0);
		printf("recv msg from server: %s\n", buff);
	}
	close(clientfd);
} 
 
int main(int argc, char *argv[])
{
	test();
	return 0;
}
