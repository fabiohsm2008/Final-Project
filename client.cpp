#include <iostream>
#include <sstream>
#include <fstream>
#include <unistd.h>
#include <iomanip>
#include <vector>
#include "UDPLibrary.h"

using namespace std;

#define bufflen 1000
#define msgBuffLen 1050

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

        stringstream strStream;
        strStream << file.rdbuf();

        string fullBuffer = strStream.str();

        vector<string> msg;

        int i = 0;
        while (i <= fullBuffer.size())
        {
            string buffer;
            if(i + bufflen < fullBuffer.size()){
                buffer = fullBuffer.substr(i, bufflen);
            }
            else{
                buffer = fullBuffer.substr(i, fullBuffer.size() - i);
            }
            buffer.resize(bufflen);
            i += bufflen;
            msg.push_back(buffer);
        }

        file.close();

        buildMsg(msg);

    }

    string ZeroPadNumber(int num){
        ostringstream ss;
        ss << setw(4) << setfill('0') << num;
        return ss.str();
    }

    string buildMsg(vector<string> msg){
        string result;
        result += 'D';
    }

    string buildFec(){

    }

    void receiver(){
        char buffer[msgBuffLen];
        while (true){
            read(buffer);
            string readed = buffer;
            cout << "Mensaje recibido: " << readed << endl;
        }
    }

    void send(string buffer){
        sendto(sockfd, buffer.c_str(), buffer.size(), 0, (struct sockaddr *)&serv, m);
    }

    void read(char buffer[msgBuffLen]){
        bzero(buffer, msgBuffLen);
        recvfrom(sockfd, buffer, msgBuffLen, 0, (struct sockaddr *)&client, &l);
    }

    string fec(string buffer1, string buffer2){
        string result;
        result.resize(bufflen);
        for(int i = 0; i < bufflen; ++i){
            result[i] = buffer1[i] ^ buffer2[i];
        }
        return result;
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
    //cout << r.crc16(buffer, bufflen) << endl;
    //cout << getpid() << endl;

    int value;

    while (true)
    {
        cout << "Ingrese un valor: ";
        cin >> value;
        if(value == 0){
            r.sendFile("test.txt");
        }
        else if(value == 1){
            cout << "First Value" << endl;
        }
        else break;
    }
    

    return 0;
}