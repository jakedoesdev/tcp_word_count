/* 
Author: Jacob Everett (jae0204)
Class:  CSCE3530.002
Description:  TCP client that sends strings to a server for processing.
*/


#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
 
int main (int argc, char **argv)
{
    int sockfd, n, portNum;
    int len = sizeof(struct sockaddr);
    char recvMsg[1024];
    char sendMsg[1024];
    struct sockaddr_in servaddr;

    if (argc != 2)
	{
		fprintf(stderr, "Usage: ./client <portnumber>\n");
		exit(EXIT_FAILURE);
	}

    /* AF_INET - IPv4 IP , Type of socket, protocol*/
    sockfd=socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servaddr,sizeof(servaddr));
 
    //store port num from user-entered arg
    portNum = atoi(argv[1]);
    servaddr.sin_family=AF_INET;
    servaddr.sin_port=htons(portNum); // Server port number
 
    /* Convert IPv4 and IPv6 addresses from text to binary form */
    inet_pton(AF_INET,"129.120.151.94",&(servaddr.sin_addr));
 
    /* Connect to server */
    connect(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr));
    //printf("client connected\n");

    while (1)
    {
        bzero(sendMsg, 1024);
        bzero(recvMsg, 1024);
        //printf("loop\n");
        //blocks, problems?
        if (n = read(sockfd, recvMsg, sizeof(recvMsg)) > 0) 
        {
            //print the received text from server
            printf("%s",recvMsg);
            //bzero(recvMsg, 1024);
            //bzero(sendMsg, 1024);
            fgets(sendMsg,1024, stdin);
            
            //exit client if user inputs "quit"
            if (strcmp(sendMsg, "quit") == 0)
            {
                printf("Exiting client...\n");
                close(sockfd);
            }
            else
            {
                write(sockfd, sendMsg, strlen(sendMsg));
            }
            bzero(sendMsg, 1024);
            bzero(recvMsg, 1024);
        }
    }
    close(sockfd);
}
