#include "EchoTcpServer.h"

#include <iostream>
#include <stdio.h>
#include <strings.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>


EchoTcpServer::EchoTcpServer(int port)
{
    mListenPort = port;
    
    bzero(&mServerAddr, sizeof(mServerAddr));
    mServerAddr.sin_family = AF_INET;
    mServerAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    mServerAddr.sin_port = htons(mListenPort);

    createSocket();
    setupSocket();
    bindSocket();
}

void EchoTcpServer::start()
{
    if (listen(mServerSocket, 5) < 0)
    {
        perror("listen failed");
        return;
    }

    int communicateSocket;
    struct sockaddr_in client_addr;
    bzero(&client_addr, sizeof(client_addr));
    while (true)
    {
        printf("enter while loop\n");
        unsigned int client_addr_len = sizeof(client_addr);
        printf("server begin accept\n");
        communicateSocket = accept(mServerSocket, (sockaddr*)&client_addr, &client_addr_len);
        if (communicateSocket < 0)
        {
            perror("accept failed");
            break;
        }
        else
        {
            printf("Client(IP: %s) connected.\n", inet_ntoa(client_addr.sin_addr));
        }

        const int BUFFERSIZE = 1024;
        char buffer[BUFFERSIZE];
        int recvMsgSize = 0;

        printf("server begin recv\n");
        recvMsgSize = recv(communicateSocket, buffer, BUFFERSIZE, 0);
        if (recvMsgSize < 0)
        {
            perror("server recv msg failed");
            break;
        }
        else if (recvMsgSize == 0)
        {
            printf("server recv finished\n");
            break;
        }
        else
        {
            printf("server recv msg success: %s\n", buffer);
            if (send(communicateSocket, buffer, recvMsgSize, 0) != recvMsgSize)
            {
                perror("server send msg failed");
                break;
            }
        }
    }
    
    close(communicateSocket);
}

void EchoTcpServer::createSocket()
{
    mServerSocket= socket(PF_INET, SOCK_STREAM, 0);
    if (mServerSocket < 0)
    {
        perror("create socket failed");
        return;
    }
}

void EchoTcpServer::setupSocket()
{
    int opt = 1;
    setsockopt(mServerSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
}

void EchoTcpServer::bindSocket()
{
    if (bind(mServerSocket, (sockaddr*)&mServerAddr, sizeof(mServerAddr)) < 0)
    {
        perror("bind socket failed");
    }
}

