#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <vector>
#define REMOTE_PORT 8080 //i trided any numbers that the same on server.cpp
   
int main(int argc, char const *argv[])
{
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char* request_ = "GET /script.txt?name=igor&surname=golovin&mail=igolovin HTTP/1.1\n\n";
    char buffer[4048] = {0};
	
	
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }
   
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(REMOTE_PORT);
       
    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) 
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
   
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }
	
    send(sock , request_ , strlen(request_) , 0 );
    printf("CLIENT READ\n---------------------------------------------\n");
    valread = read( sock , buffer, 4024);
    std::cout << buffer << std::endl;
    return 0;
}