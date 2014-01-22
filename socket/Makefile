all : compile

TARGET := test

GPP := g++

SOURCES += \
    main.cpp \
    EchoTcpServer.cpp \
    SimpleTcpClient.cpp

INCLUDES += -I.

LIBS += -lpthread

compile : 
	$(GPP) -o $(TARGET) $(SOURCES) $(LIBS)
