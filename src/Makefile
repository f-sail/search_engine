objs_dict:=Configuration.o  DicProducer.o SplitTool.o  _dictionary.o

all:$(wildcard *.exe)

dict.exe:$(objs_dict)
	g++ $^ -o $@


%.o:%.cc
	g++ -c $^ -g


clean_all:
	$(RM) $(wildcard *.o *.exe) 

clean_dict:
	$(RM) $(objs_dict) dict.exe
