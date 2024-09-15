#include "../include/_client.h"

#include "../include/TLV.h"
#include "../include/SplitTool.h"
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
    SplitTool* cutter = new SplitToolCppJieba();
	while(1){
        int i = -1;
        cout << ">> 选择测试功能: 1、推荐词     2、网页搜索: \n";
        cin >> i;
        
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        

        TLV msg;
        if(1 == i){
		    cout << ">> 推荐词测试: ";
		    getline(cin, msg.value);
            
            msg.type = TYPE_RECOMMEND;
            msg.len = msg.value.size();
            sendTLV(clientfd, msg);
            
            cout << ">> 等待回复...\n";
            char buff[65535] = {0};
		    recv(clientfd, buff, sizeof(buff), 0);
            cout << buff << "\n";
        }else if(2 == i){
		    cout << ">> 网页搜索测试: ";
		    getline(cin, msg.value);
            
            msg.type = TYPE_SEARCH;
            msg.len = msg.value.size();
            sendTLV(clientfd, msg);
	
            cout << ">> 等待回复...\n";
            string str;
            while(1){
                char buff[65535] = {0};
		        int ret = recv(clientfd, buff, sizeof(buff), 0);
                if(-1 == ret){
                    std::cerr << "recv error\n";
                }if(0 == ret){
                    break;
                }else{
                    str += buff;
                    if(ret < sizeof(buff)){
                        break;
                    }
                }
            }
	        printf(">> 服务端回复: \n");
		    /* printf(">> %s\n", str.c_str()); */
            nlohmann::json jsonArr = nlohmann::json::parse(str);
            for(auto& str: jsonArr){
                WebPage wp(cutter->rss(str));
                cout << ">> title: " << wp.title << "\n";
                cout << ">> url: " << wp.url << "\n";
                cout << ">> content" << wp.content << "\n\n";
            }
        }
	}
    
    delete cutter;
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
