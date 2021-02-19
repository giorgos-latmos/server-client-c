// Server side implementation of UDP client-server model 
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h>
#include <time.h>
#include "cJSON.h"
#include "cJSON.c" 
  
#define PORT     8080 
#define MAXLINE 1024 
  
// Driver code 
int main() { 
    int sockfd; 
    char buffer[MAXLINE]; 
    struct sockaddr_in servaddr, cliaddr;
      
    // Creating socket file descriptor 
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    } 
      
    memset(&servaddr, 0, sizeof(servaddr)); 
    memset(&cliaddr, 0, sizeof(cliaddr)); 
      
    // Filling server information 
    servaddr.sin_family    = AF_INET; // IPv4 
    servaddr.sin_addr.s_addr = INADDR_ANY; 
    servaddr.sin_port = htons(PORT); 
      
    // Bind the socket with the server address 
    if ( bind(sockfd, (const struct sockaddr *)&servaddr,  
            sizeof(servaddr)) < 0 ) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    }

    int len, n;
    len = sizeof(cliaddr);  //len is value/resuslt

    while(1){
        //Receive Data From Client
        n = recvfrom(sockfd, (char *)buffer, MAXLINE,  
                MSG_WAITALL, ( struct sockaddr *) &cliaddr, 
                &len);

        //Decrypt Data
        for (int i=0; i<strlen(buffer); i++){
            buffer[i] = buffer[i]^1;
        } 
        buffer[n] = '\0';

        //Parse Data
        cJSON *rdata = cJSON_Parse(buffer);
        cJSON *timestamp = cJSON_GetObjectItemCaseSensitive(rdata, "Timestamp");
        cJSON *measurement = cJSON_GetObjectItemCaseSensitive(rdata, "Measurement");
        printf("[%s] %s\n", cJSON_Print(timestamp), cJSON_Print(measurement));
        
        //Send Acknowledgement to Client
        char *resp = "[OK]"; 
        sendto(sockfd, (const char *)resp, strlen(resp),  
            MSG_CONFIRM, (const struct sockaddr *) &cliaddr, 
                len); 
    }
    return 0; 
} 