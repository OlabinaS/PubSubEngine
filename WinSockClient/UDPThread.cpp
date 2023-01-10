/*#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

void* udp_thread(void* arg) {
    int sockfd;
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t clilen;
    char buffer[1024];

    // Create a socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("ERROR opening socket");
        exit(1);
    }

    // Initialize the server address structure
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket to the address and port
    if (bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("ERROR on binding");
        exit(1);
    }

    // Set the size of the client address structure
    clilen = sizeof(cli_addr);

    while (1) {
        // Receive data from the client
        int n = recvfrom(sockfd, buffer, 1024, 0, (struct sockaddr*)&cli_addr, &clilen);
        if (n < 0) {
            perror("ERROR reading from socket");
            exit(1);
        }

        // Print the received data
        printf("Received message: %s\n", buffer);

        // Send data back to the client
        n = sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr*)&cli_addr, clilen);
        if (n < 0) {
            perror("ERROR writing to socket");
            exit(1);
        }
    }
}
/*
int main(int argc, char* argv[]) {
    pthread_t thread;
    int rc;

    // Create the UDP thread
    rc = pthread_create(&thread, NULL, udp_thread, NULL);
    if (rc) {
        perror("ERROR creating thread");
        exit(1);
    }

    // Wait for the thread to finish
    pthread_join(thread, NULL);

    return 0;
}
*/