#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>


int main(void)
{
    fd_set master;    // master file descriptor list
    fd_set read_fds;  // temp file descriptor list for select()
    int    fdmax;        // maximum file descriptor number
    int    listener;     // listening socket descriptor
    int    new_fd;        // new socket descriptor
    struct sockaddr_in addr;
    struct sockaddr_in client;
    socklen_t addrlen;
    char buf[256];    // buffer for client data
    char message[100];// buffer for server data
    int  nbytes;
    int  port;
    char ip[50];
    int  i, j;
    
    
    
    FD_ZERO(&master);    // clear the master and temp sets
    FD_ZERO(&read_fds);
    
    printf("Enter Port:");
    scanf("%d",&port);
    while(getchar() != '\n');// avoid sending 'next line'
    
    // Create Socket
    listener =  socket(AF_INET, SOCK_STREAM, 0);
    
    // Setting IP and Port
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);// port, which htons converts host number to network order
    addr.sin_addr.s_addr = INADDR_ANY;// bind to the IP of the host it's running on
    
    // Associate the socket with Port
    bind(listener, (struct sockaddr*)&addr, sizeof(addr));
    printf("\n\t[Info] Binding...\n");
    
    
    // Listen
    listen(listener, 5);
    printf("\t[Info] Listening...\n");
    
    
    FD_SET(listener, &master);// add the listener to the master set
    
    
    fdmax = listener;// keep track of the biggest file descriptor
    
    // Main loop
    while(1){
        
        read_fds = master; // copy it
        select(fdmax+1, &read_fds, NULL, NULL, NULL);
        
        
        for(i = 0; i <= fdmax; i++) {
            
            if (FD_ISSET(i, &read_fds)) {
                
                // New connection
                if (i == listener) {
                    
                    addrlen = sizeof client;
                    new_fd = accept(listener,(struct sockaddr *)&client,&addrlen);// new socket descriptor

                    FD_SET(new_fd, &master); // add to master set
                    
                    if (new_fd > fdmax) {    // keep track of the max
                        fdmax = new_fd;
                    }
                    printf("\n\t[Info] New connection from %s on "
                               "socket %d\n",inet_ntoa(client.sin_addr),new_fd);
                    send(new_fd,"Nice to meet you\n", 18, 0);// send data to new client
                    
                }
                // Active connection
                else {
                    
                    // Receive message from client
                    if ((nbytes = recv(i, buf, sizeof buf, 0)) <= 0) {
                        
                        // got error nbytes== -1 or connection closed by client nybtes ==0
                        if (nbytes == 0) {
                            // connection closed
                            printf("\n\t[Info] Socket %d lost connection\n", i);
                        } else {
                            perror("recv");
                        }
                        close(i);
                        printf("\n\t[Info] Close %d\n",i);
                        FD_CLR(i, &master); // remove from master set
                        
                    } else {
                        
                        printf("\n\t Socket %d : \n",i);
                        printf("\t %s",buf);
                        // Reply message to client
                        printf("\t Reply :\n\t ");
                        fgets(message,100,stdin);
                        send(i, message, sizeof(message), 0);// send data to specific client
                        
                    }
                }
            }
        }
    }
    
    return 0;
    
}
