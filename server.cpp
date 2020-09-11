#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include "UDPLibrary.h"

using namespace std;

#define bufflen 1000

struct Packet{
    char p[bufflen];
    Packet(char buffer[bufflen]){
        strncpy(p, buffer, bufflen);
    }
};

struct clientConnection{
    sockaddr_in client;
    socklen_t l;
    vector<Packet> message;
    bool operator ==(sockaddr_in comp){
        if(client.sin_addr.s_addr == comp.sin_addr.s_addr && client.sin_port == comp.sin_port)
            return true;
        return false;
    }
    clientConnection(sockaddr_in c){
        client = c;
        l = sizeof(client);
    }
};

class Server{
private:
    int sockfd;
    sockaddr_in serv;
    socklen_t l;
    vector<clientConnection> clients;
public:
    bool running;
    Server(int port){
        sockfd = socket(AF_INET,SOCK_DGRAM,0);
        serv.sin_family = AF_INET;
        serv.sin_port = htons(port);
        serv.sin_addr.s_addr = INADDR_ANY;
        bind(sockfd, (const struct sockaddr *)&serv, sizeof(serv));
        running = true;
        thread r(&Server::connectClient, this);
        r.join();
    }

    int findClient(sockaddr_in client){
        for(int i = 0; i < clients.size(); ++i){
            if(clients[i] == client) 
                return i;
        }
        return -1;
    }

    void connectClient(){
        sockaddr_in client;
        char buffer[bufflen];
        bzero(buffer, bufflen);
        socklen_t l = sizeof(client);
        while (true)
        {
            recvfrom(sockfd, buffer, bufflen, 0, (struct sockaddr *)&client, &l);
            cout << "**********" << buffer << endl;
            int pos = findClient(client);
            if(pos == -1){
                clientConnection c(client);
                Packet p(buffer);
                c.message.push_back(p);
                clients.push_back(c);
            }
            else {
                clients[pos].message.push_back(buffer);
            }
        }
    }

    void send(char buffer[bufflen], clientConnection client){
        socklen_t m = sizeof(client.client);
        sendto(sockfd, buffer, bufflen, 0, (struct sockaddr *)&client.client, m);
        bzero(buffer, bufflen);
    }
};

int main()
{
    Server s(8080);

    while(s.running);


    return 0;
}
