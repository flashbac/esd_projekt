CXXFLAGS =	-O2 -g -Wall -fmessage-length=0

OBJS =		main.o commando.o

LIBS =		commando.c

TARGET =	SocketServer

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
