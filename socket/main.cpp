#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <pthread.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "EchoTcpServer.h"
#include "SimpleTcpClient.h"

using namespace std;
typedef void* (*ThreadProc)(void*);

static const int kListenPort = 52720;

int createThread(ThreadProc proc)
{
    bool success = false;

    pthread_t threadId;
    pthread_attr_t attributes;
    pthread_attr_init(&attributes);

    success = !pthread_create(&threadId, &attributes, proc, NULL);

    pthread_attr_destroy(&attributes);

    return success;
}

void* serverProc(void* params)
{
    printf("serverProc enter\n");
    EchoTcpServer server(kListenPort);
    server.start();

    printf("serverProc leave\n");
    exit(0);
}

void* clientProc(void*)
{
    printf("clinetProc enter\n");

    SimpleTcpClient client;
    client.connect("127.0.0.1", kListenPort);

    const char* msg = "hello";
    client.write(msg, strlen(msg));
    
    const int BUFFER_SIZE = 100;
    char buffer[BUFFER_SIZE] = {0};
    int length = 0;
    length = client.read(buffer, BUFFER_SIZE);
    if (length > 0)
    {
        printf("client recv %s\n", buffer);
    }

    printf("clientProc leave\n");
}

int main(int argc, char* argv[])
{
    printf("main enter\n");

    createThread(serverProc);
    usleep(1000 * 100); // let server proc run first
    createThread(clientProc);

    while(true)
    {
        usleep(1000 * 2000);
    }

    printf("main leave\n");
    return 0;
}


