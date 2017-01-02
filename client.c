#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


int main()
{

    struct sockaddr_in server;
    int  sockfd;
    char buf[100];// buffer for server data
    char message[100];// buffer for client data
    char ip[50];
    int  n;
    int  port;
    
    printf("Enter IP:");
    scanf("%s",ip);
    
    printf("Enter Port:");
    scanf("%d",&port);
    while(getchar() != '\n');
    
    // Creat Socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    
    // Setting IP and Port
    server.sin_family = AF_INET;
    server.sin_port = htons(port);// port, which htons converts host number to network order
    inet_pton(AF_INET, ip, &server.sin_addr);// convert readable IP to their binary representation
    
    // Request a connection with Server
    printf("\n\t[Info] Connecting...\n");
    connect(sockfd, (struct sockaddr *)&server, sizeof(server));
    printf("\t[Info] Connect!\n");

    
    while (1) {
        
        memset(buf, 0, sizeof(buf));
        recv(sockfd, buf, sizeof(buf),0);// receive data from server
        printf("\n\t Server : \n");
        printf("\t %s",buf);
        printf("\t Reply :\n\t ");
        fgets(message,100,stdin);
        send(sockfd, message, sizeof(message), 0);// send data to server
        
    }
    
    //End connection
    close(sockfd);  
    return 0;  
}  