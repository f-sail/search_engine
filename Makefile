CC := g++
.o := bin/%.o
.cc:= src/%.cc

dict := bin/dict.exe
objs_dict := bin/Configuration.o bin/DicProducer.o bin/SplitTool.o  bin/DirScanner.o bin/_dict.o

page_lib := bin/page_lib.exe
objs_page_lib := bin/Configuration.o bin/DirScanner.o bin/PageLib.o bin/tinyxml2.o bin/_page_lib.o


$(.o):$(.cc)
	$(CC) -c $< -o $@ -g


all:dict page_lib

dict:$(dict)
page_lib:$(page_lib)


$(dict):$(objs_dict)
	$(CC) $^ -o $@
$(page_lib):$(objs_page_lib)
	$(CC) $^ -o $@


clean_all:
	$(RM) $(wildcard bin/*.o bin/*.exe) 
clean_dict:
	$(RM) $(objs_dict) $(dict)
clean_page_lib:
	$(RM) $(objs_page_lib) $(page_lib)


rebuild_all:clean_all all
rebuild_dict:clean_dict dict
rebuild_page_lib:clean_page_lib page_lib
