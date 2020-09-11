#include <iostream>
#include <fstream>
#include <unistd.h>
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

        thread r(&Client::receiver, this);
        r.detach();

    }

    void sendFile(string fileName){
        ifstream file(fileName, ios::binary | ios::ate);
        streamsize filesize = file.tellg();
        file.seekg(0, ios::beg);

        char buffer[bufflen];
        bzero(buffer, bufflen);

        while(file.read(buffer, bufflen) || file.gcount()){
            send(buffer);
        }

    }

    void receiver(){
        char buffer[bufflen];
        while (true){
            read(buffer);
            cout << "Mensaje recibido: " << buffer << endl;
        }
    }

    void send(char buffer[bufflen]){
        sendto(sockfd, buffer, bufflen, 0, (struct sockaddr *)&serv, m);
        bzero(buffer, bufflen);
    }

    void read(char buffer[bufflen]){
        bzero(buffer, bufflen);
        recvfrom(sockfd, buffer, bufflen, 0, (struct sockaddr *)&client, &l);
    }

    void fec(char buffer1[bufflen], char buffer2[bufflen], char bufferResult[bufflen]){
        for(int i = 0; i < bufflen; ++i){
            bufferResult[i] == buffer1[i] ^ buffer2[i];
        }
    }

    unsigned short crc16(char* numPacket, unsigned char length) {
        unsigned char x;
        unsigned short crc = 0xFFFF;

        while (length--) {
            x = crc >> 8 ^ *numPacket++;
            x ^= x >> 4;
            crc = (crc << 8) ^ ((unsigned short)(x << 12)) ^ ((unsigned short)(x << 5)) ^ ((unsigned short)x);
        }
        return crc;
    }

};

int main()
{
    Client r(8080, "127.0.0.1");
    char buffer[bufflen] = "Enviando mensaje";
    cout << r.crc16(buffer, bufflen) << endl;

    cout << getpid() << endl;

    return 0;
}
