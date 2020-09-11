#include <iostream>
#include <fstream>
#include "UDPLibrary.h"

using namespace std;

#define bufflen 1000

class Client{
private:
    int sockfd;
    sockaddr_in serv, client;
    socklen_t l, m;
public:
    Client(int port, string ip){
        sockfd = socket(AF_INET,SOCK_DGRAM,0);
        serv.sin_family = AF_INET;
        serv.sin_port = htons(port);
        serv.sin_addr.s_addr = inet_addr(ip.c_str());
        l = sizeof(client);
        m = sizeof(serv);
    }

    void sendFile(string fileName){
        ifstream file(fileName, ios::binary | ios::ate);
        streamsize filesize = file.tellg();
        file.seekg(0, ios::beg);

        char buffer[bufflen];
        bzero(buffer, bufflen);

        while(file.read(buffer, bufflen) || file.gcount()){
            sendto(sockfd, buffer, bufflen, 0, (struct sockaddr *)&serv, m);
            bzero(buffer, bufflen);
        }

    }

    void fec(char buffer1[bufflen], char buffer2[bufflen], char bufferResult[bufflen]){
        for(int i = 0; i < bufflen; ++i){
            bufferResult[i] == buffer1[i] ^ buffer2[i];
        }
    }
};


void error(char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

void sender(int sockfd, sockaddr_in serv){
    socklen_t m = sizeof(serv);
    string msg;
    cout << "Ready for send messagges" << endl;
    while (true){
        cin >> msg;
        sendto(sockfd, msg.c_str(), msg.size(), 0, (struct sockaddr *)&serv, m);
    }
}

void receiver(int sockfd, sockaddr_in client){
    char buffer[bufflen];
    bzero(buffer, bufflen);
    socklen_t l = sizeof(client);
    cout << "Ready for read messagges" << endl;
    while (true){
        recvfrom(sockfd, buffer, bufflen, 0, (struct sockaddr *)&client, &l);
        cout << "Mensaje recibido: " << buffer << endl;
        bzero(buffer, bufflen);
    }
    
}

void buildMsg(){

}

int main()
{
    Client r(8080, "127.0.0.1");
    char buffer[10] = "hola";
    char buffer2[10] = "como";
    char buffer3[10];
    r.fec(buffer, buffer2, buffer3);
    return 0;
}
