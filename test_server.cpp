#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include<stdio.h>

struct pac{
    uint8_t Id;
    char msg[20];
};

void print_packet(char * packet){
    struct pac *ptr = (struct pac*)packet;
    printf("Id : %d\n",(int)ptr->Id);
    printf("Message : %s\n",ptr->msg);
    return;
}


int main() {
    // Step 1: Create a socket
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        std::cerr << "Failed to create socket.\n";
        return 1;
    }

    // Step 2: Bind the socket to an IP/Port
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080); // Port 8080
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Localhost

    if (bind(server_socket, (sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        std::cerr << "Failed to bind to IP/Port.\n";
        close(server_socket);
        return 1;
    }

    // Step 3: Listen for incoming connections
    if (listen(server_socket, 5) == -1) {
        std::cerr << "Failed to listen on socket.\n";
        close(server_socket);
        return 1;
    }
    std::cout << "Server is listening on 127.0.0.1:8080\n";

    // Step 4: Accept a client connection
    sockaddr_in client_addr;
    socklen_t client_size = sizeof(client_addr);
    int client_socket = accept(server_socket, (sockaddr*)&client_addr, &client_size);
    if (client_socket == -1) {
        std::cerr << "Failed to connect to client.\n";
        close(server_socket);
        return 1;
    }

    // Step 5: Read message from the client
    char buffer[4096];
    memset(buffer, 0, 4096);
    int bytes_received = recv(client_socket, buffer, 4096, 0);
    if (bytes_received == -1) {
        std::cerr << "Error in recv().\n";
        close(client_socket);
        close(server_socket);
        return 1;
    }

    printf("Protocol is : %d\n",(int)buffer[0]);

    print_packet(buffer+1);

    std::cout << "Received message from client: " << buffer << "\n";

    // Step 6: Send a response to the client
    const char* response = "Hello from the server!\n";
    int bytes_sent = send(client_socket, response, strlen(response), 0);
    if (bytes_sent == -1) {
        std::cerr << "Error in send().\n";
    } else {
        std::cout << "Response sent to client.\n";
    }

    // Step 7: Close the sockets
    sleep(1);
    close(client_socket);
    close(server_socket);

    return 0;
}
