CC := g++
.o := bin/%.o
.cc:= src/%.cc

dict := bin/dict.exe
objs_dict := bin/Configuration.o bin/DicProducer.o bin/SplitTool.o bin/_dictionary.o


$(.o):$(.cc)
	$(CC) -c $< -o $@ -g


all:dict
dict:$(dict)


$(dict):$(objs_dict)
	$(CC) $^ -o $@




clean_all:
	$(RM) $(wildcard bin/*.o bin/*.exe) 

clean_dict:
	$(RM) $(objs_dict) bin/dict.exe
