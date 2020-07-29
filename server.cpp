#include <iostream>
#include "UDPLibrary.h"

using namespace std;

void error( char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
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

    char buffer[256];
    socklen_t l = sizeof(client);
    cout<<"\ngoing to recv\n";
    while (true)
    {
        int rc= recvfrom(sockfd,buffer,sizeof(buffer),0,(struct sockaddr *)&client,&l);
        if(rc<0)
        {
            cout<<"ERROR READING FROM SOCKET";
        }
        cout<<"\n the message received is : "<<buffer<<endl;
        int rp= sendto(sockfd,"hi",2,0,(struct sockaddr *)&client,l);

        if(rp<0)
        {
            cout<<"ERROR writing to SOCKET";
        }
    }

    return 0;
}