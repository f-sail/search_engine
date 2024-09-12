CC := g++
.h := include/%.h
.o := bin/%.o
.cc:= src/%.cc

dict := bin/dict.exe
objs_dict := bin/Configuration.o  bin/DicProducer.o  bin/SplitTool.o  bin/DirScanner.o bin/_dict.o

page_lib := bin/page_lib.exe
objs_page_lib := bin/Configuration.o  bin/DirScanner.o  bin/PageLib.o  bin/PageLibPreprocessor.o  bin/SplitTool.o  bin/tinyxml2.o  bin/_page_lib.o

server := bin/server.exe
objs_server := bin/_server.o  bin/log4cpp.o  bin/Acceptor.o  bin/Configuration.o  bin/Dictionary.o  bin/EventLoop.o  bin/InetAddress.o  bin/KeyRecommander.o  bin/Socket.o  bin/SocketIO.o  bin/MessageHandler.o  bin/SplitTool.o  bin/TaskQueue.o  bin/TcpConnection.o  bin/TcpServer.o  bin/ThreadPool.o  bin/WebPages.o


client := bin/client.exe
objs_client := bin/_client.o


$(.o):$(.cc) $(.h)
	$(CC) -c $< -o $@ -g


all:dict page_lib server client


dict:$(dict)
page_lib:$(page_lib)
server:$(server)
client:$(client)

$(dict):$(objs_dict)
	$(CC) $^ -o $@
$(page_lib):$(objs_page_lib)
	$(CC) $^ -o $@
$(server):$(objs_server)
	$(CC) $^ -o $@ -llog4cpp
$(client):$(objs_client)
	$(CC) $^ -o $@



clean_all:
	$(RM) $(wildcard bin/*.o bin/*.exe) 
clean_dict:
	$(RM) $(objs_dict) $(dict)
clean_page_lib:
	$(RM) $(objs_page_lib) $(page_lib)
clean_server:
	$(RM) $(objs_server) $(server)
clean_client:
	$(RM) $(objs_client) $(client)


rebuild_all:clean_all all
rebuild_dict:clean_dict dict
rebuild_page_lib:clean_page_lib page_lib
rebuild_server:clean_server server
rebuild_client:clean_client client
