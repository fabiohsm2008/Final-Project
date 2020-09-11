#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include "UDPLibrary.h"

using namespace std;

#define bufflen 1000

struct clientConnection{
    sockaddr_in client;
    vector<char[bufflen]> messagge;
    bool operator ==(clientConnection comp){
        if(client.sin_addr.s_addr == comp.client.sin_addr.s_addr && client.sin_port == comp.client.sin_port)
            return true;
        return false;
    }
};

char* fec(char buffer1[bufflen], char buffer2[bufflen]){
    char bufferResult[bufflen];
    for(int i = 0; i < bufflen; ++i){
        bufferResult[i] == buffer1[i] ^ buffer2[i];
    }
    return bufferResult;
}

void error( char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

bool findClient(vector<sockaddr_in> clients, sockaddr_in client){
    for(int i = 0; i < clients.size(); ++i){
        if(clients[i].sin_addr.s_addr == client.sin_addr.s_addr && clients[i].sin_port == client.sin_port) return true;
    }
    return false;
}

void sender(int sockfd, sockaddr_in client){
    socklen_t m = sizeof(client);
    string msg;
    cout << "Ready for send messagges" << endl;
    while (true){
        cin >> msg;
        sendto(sockfd, msg.c_str(), msg.size(), 0, (struct sockaddr *)&client, m);
    }
}

void receiver(int sockfd, sockaddr_in client){
    char buffer[bufflen];
    bzero(buffer, bufflen);
    socklen_t l = sizeof(client);
    cout << "Ready for read messagges" << endl;
    while (true){
        if(recvfrom(sockfd, buffer, bufflen, 0, (struct sockaddr *)&client, &l) > 0){
            cout << buffer << endl;
            bzero(buffer, bufflen);
        };
    }
}

void connectClient(int sockfd, sockaddr_in client){
    vector<sockaddr_in> clients;

    char buffer[bufflen];
    bzero(buffer, bufflen);
    socklen_t l = sizeof(client);
    ofstream file2("testServer.txt");
    while (true)
    {
        recvfrom(sockfd, buffer, bufflen, 0, (struct sockaddr *)&client, &l);
        file2.write(buffer, bufflen);
        bzero(buffer, bufflen);
        if( !findClient(clients, client) ){
            clients.push_back(client);
            /*thread r(receiver, sockfd, client);
            thread s(sender, sockfd, client);
            r.detach();
            s.detach();*/
        }
        cout << clients.size() << endl;
    }
}

int main()
{
    int sockfd;
    sockfd = socket(AF_INET,SOCK_DGRAM,0);
    struct sockaddr_in serv,client;

    serv.sin_family = AF_INET;
    serv.sin_port = htons(8080);
    serv.sin_addr.s_addr = inet_addr("127.0.0.1");

    bind(sockfd, (const struct sockaddr *)&serv, sizeof(serv));

    /*thread c(connectClient, sockfd, client);
    c.join();*/

    char b[bufflen] = "hola";
    char b2[bufflen] = "como";

    fec(b, b2);

    return 0;
}
