OBJS = mas.cpp gui.cpp util.cpp map.cpp agent.cpp auction.cpp

CC = g++

COMPILER_FLAGS = -w -g

LINKER_FLAGS = -lncursesw

all : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) 
