CXX = g++
CC = gcc
CFLAGS = -c -o
CXXFLAGS = -c -o

OUTPUTDIR = ./bin/
SOURCEDIR = ./src/
MKDIR = mkdir -p $(OUTPUTDIR)
RM = rm

MAINOBJ = main

COBJECT = socket.o

CXXOBJECT = $(OUTPUTDIR)main.o\
	$(OUTPUTDIR)threads.o\
	$(OUTPUTDIR)regex.o

TEXT = $(info  ) \
	$(info Build finished. If you want to run please write this command;) \
	$(info  ) \
	$(info -    sudo ./main '([A-Z])\w+' 15) \
	$(info  )

all: server
	$(TEXT)

server: $(CSOURCES)
	$(MKDIR)
	$(CC) $(CFLAGS) $(OUTPUTDIR)socket.o $(SOURCEDIR)socket.c
	$(CXX) $(CXXFLAGS) $(OUTPUTDIR)threads.o $(SOURCEDIR)threads.cpp
	$(CXX) $(CXXFLAGS) $(OUTPUTDIR)main.o $(SOURCEDIR)main.cpp
	$(CXX) -pthread -o $(MAINOBJ) $(foreach p,$(CXXOBJECT),$(p)) $(OUTPUTDIR)$(COBJECT)

.PHONY: clean
clean: main
	$(RM) -rf $(OUTPUTDIR)
	$(RM) -f alldata.log0
	$(RM) main
