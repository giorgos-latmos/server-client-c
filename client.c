// Client side implementation of UDP client-server model 
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
  
#define PORT    8080 
#define MAXLINE 1024 
  
// Driver code 
int main() { 
    int sockfd; 
    char buffer[MAXLINE]; 
    struct sockaddr_in servaddr; 
  
    // Creating socket file descriptor 
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    } 
  
    memset(&servaddr, 0, sizeof(servaddr)); 
      
    // Filling server information 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_port = htons(PORT); 
    servaddr.sin_addr.s_addr = INADDR_ANY; 
      
    int n, len;
    time_t current_time;
    srand(time(0));

    while(1){

        //Keep Track of Time
        current_time = time(NULL);

        //Create JSON files to save data
        cJSON *data = cJSON_CreateObject();
        cJSON_AddItemToObject(data, "Timestamp", cJSON_CreateNumber(current_time));
        cJSON_AddItemToObject(data, "Measurement", cJSON_CreateNumber(rand()%256));
        char *send_data = cJSON_Print(data);

        //Encrypt Data
        for (int i=0; i<strlen(send_data); i++){
            send_data[i] = send_data[i]^1;
        }
        
        //Uncomment to print encrypted data
        //printf("%s\n", send_data);

        //Send Encrypted Data to Server
        sendto(sockfd, (const char *)send_data, strlen(send_data), 
            MSG_CONFIRM, (const struct sockaddr *) &servaddr,  
                sizeof(servaddr));

        //Wait for Server Response
        current_time = time(NULL);
        n = recvfrom(sockfd, (char *)buffer, MAXLINE,  
                MSG_WAITALL, (struct sockaddr *) &servaddr, 
                &len); 
        buffer[n] = '\0';

        //Retry Sending the Message if 4 seconds passed without response, otherwise, print [OK].
        if (time(NULL)-current_time > 4){
            printf("Retry Sending Message\n");
            sendto(sockfd, (const char *)send_data, strlen(send_data), 
            MSG_CONFIRM, (const struct sockaddr *) &servaddr,  
                sizeof(servaddr));
        }else{
            printf("Server : %s\n", buffer);
        } 

        //Delay 1 sec before sending next message
        while(time(NULL)-current_time <= 1){}

    }
  
    close(sockfd); 
    return 0; 
} 
