#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <thread>

class UDP{
private:
    
public:
    UDP();
    int setMode(int, void* (std::string, std::string));
    int write(std::string, char*, int);
    void read(std::string, std::string);
    int close(std::string);
};