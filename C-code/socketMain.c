//
//  socketMain.c
//
//  Created by Yu Xia on 1/2/25.
//

#include "socketMain.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>

int set_stow = 256;
char test_message[256] = "Testing for connection... if you can read this then it has worked!";

int server_exe(int port) {
    int server_socket;
    struct sockaddr_in server;
    
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    //32-bit address, TCP connection, default
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = INADDR_ANY;
    //assign to default address to the same machine - test for other options
    
    bind(server_socket, (struct sockaddr*) &server, sizeof(server));
    
    char* stowage = calloc(1, set_stow);
    char storage[256];
    
    listen(server_socket, 5);
    
    int client_socket;
    client_socket = accept(server_socket, NULL, NULL);
    //the last two parameters allow you to record client information, their address and location
    //figure out a way to thread a function call to record and store information elsewhere
    
    send(client_socket, test_message, set_stow, 0);
    //last parameter is for flags for message protocols, might want to look into it later but
    //at the current it is low priority as it is not entirely necessary
    
    //shutdown(server_socket, 2);
    close(server_socket);
    
    printf("Shutting down server...");
    free(stowage);
    return 0;
}
int client_exe(int port) {
    int client_socket;
    struct sockaddr_in client;
    
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    //32-bit address, TCP connection, default
    client.sin_family = AF_INET;
    client.sin_port = htons(port);
    client.sin_addr.s_addr = INADDR_ANY;
    //assign default address to the same machine - test for other options and alter accordingly
    
    int connection_check = connect(client_socket, (struct sockaddr *) &client, sizeof(client));
    if(connection_check == -1) {
        printf("ERROR :: there was an error connecting to remote socket \n");
        return 0;
    }
    char* stowage = calloc(1, set_stow);
    char storage [256];
    recv(client_socket, &storage, sizeof(set_stow), 0);
    //recieve data from host
    
    printf("Message recieved: %s\n", storage);
    //print information received
    
    //shutdown(client_socket, 2);
    close(client_socket);
    //shutdown both read and write functions, refuse to continue connection
    
    printf("Shutting down client...");
    free(stowage);
    return 0;
}

int main(int argc, char** argv) {
    if(argc != 3) {
        printf("Wrong number of arguments when compiling...\nyou have %d arguements but we need 3", argc);
    }
    char* type = *(argv+1);
    char* port = *(argv+2);
    
    int type2 = *type - '0';
    int port2 = 9001;
    //i need to make a function to be able to read character inputs and spit out an int
    
    if(type2 == 1) {
        server_exe(port2);
    }
    else if (type2 == 2){
        client_exe(port2);
    }
    return 0;
}

/*
NOTES:
	the program at the most basic level work as intended *BUT*
 		there seems to be an issue when sending data as the message is cut off when the client recieves it
 		this needs some investigation and maybe get back to it in the future
 		frankly packet loss is extreme in this case
*/
