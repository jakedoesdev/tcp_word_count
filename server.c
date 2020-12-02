/* 
Author: Jacob Everett
Description: TCP server that processes strings from a client. Counts words/letters/vowels, capitalizes the first letter of each word, and writes this data to the client. Exits on "quit" command.

Usage: ./wcserver <portnumber>
*/


#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

//string processing function declarations
int wordCount(char* str);
int letterCount(char* str);
int vowelCount(char* str);

int main(int argc, char **argv)
{
  char recvMsg[1024];
  char sendMsg[1024];
  int listen_fd, conn_fd, n, portNum;
  struct sockaddr_in servaddr;

  if (argc != 2)
	{
		fprintf(stderr, "Usage: ./wcserver <portnumber>\n");
		exit(EXIT_FAILURE);
	}

  //store user-entered port number
  portNum = atoi(argv[1]);
 
  /* AF_INET - IPv4 IP , Type of socket, protocol*/
  listen_fd = socket(AF_INET, SOCK_STREAM, 0);

  bzero(&servaddr, sizeof(servaddr));

  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htons(INADDR_ANY);
  servaddr.sin_port = htons(portNum);

  //allow reuse of the same socket
	int on = 1;
	setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
 
  /* Bind the socket struct to the socket */
	bind(listen_fd,  (struct sockaddr *) &servaddr, sizeof(servaddr));

	/* Start listening to incoming connections */
	listen(listen_fd, 10);

  /* Accept an incoming connection */
  conn_fd = accept(listen_fd, (struct sockaddr*)NULL, NULL);

  while(1)
  {
    int lc, wc, vc;  //letter, word, vowel counts

    //Send request for input to client
    strcpy(sendMsg, "Input string: ");
    write(conn_fd, sendMsg, strlen(sendMsg));
    bzero(sendMsg, 1024);
    bzero(recvMsg, 1024);


    //recvMsg has a newline on the end of it?
    //read is blocking here, maybe causing the hanging problem?
    if (n = read(conn_fd, recvMsg, sizeof(recvMsg)) > 0)
    //while (n = read(conn_fd, recvMsg, sizeof(recvMsg)) > 0)
    {


      //maybe add this and then have a while loop until the null char for everything below
      //the input string received msg?
      //recvMsg[n] = '\0';

      
      printf("Input string received...\n");
      


      //close connection if quit message received
      if (strcmp(recvMsg, "quit") == 0 || strcmp(recvMsg, "Quit") == 0)
      {
        printf("Server exiting...\n");
        bzero(recvMsg, 1024);
        close(conn_fd);
      }
      else
      {
        wc = wordCount(recvMsg);
        lc = letterCount(recvMsg);
        vc = vowelCount(recvMsg);

        //capitalize first letters
        for (int i = 0; recvMsg[i] != '\0'; i++)
        {
          //check first char for capitalization
          if (i == 0)
          {
            if (recvMsg[i] >= 97 && recvMsg[i] <= 122)
            {
              //capitalize letter
              recvMsg[i] = recvMsg[i] - 32;
            }
          }
          else if (recvMsg[i] >= 97 && recvMsg[i] <= 122 && recvMsg[i-1] == ' ')
          {
            //capitalize letter
            recvMsg[i] = recvMsg[i] - 32;
          }
        }
        
        //recvMsg[n] = '\0';
        //Write capitalized string to client
        sprintf(sendMsg, "Output string: %s\n", recvMsg);
        write(conn_fd, sendMsg, strlen(sendMsg));
        bzero(sendMsg, 1024);
        bzero(recvMsg, 1024);

        //Write word count to client
        sprintf(sendMsg, "Word count: %d\n", wc);
        write(conn_fd, sendMsg, strlen(sendMsg));
        bzero(sendMsg, 1024);

        //Write letter count to client
        sprintf(sendMsg, "Letter count: %d\n", lc);
        write(conn_fd, sendMsg, strlen(sendMsg));
        bzero(sendMsg, 1024);

        //Write vowel count to client
        sprintf(sendMsg, "Vowel count: %d\n", vc);
        write(conn_fd, sendMsg, strlen(sendMsg));
        bzero(sendMsg, 1024);

        //bzero(recvMsg, 1024);
      }
      bzero(recvMsg, 1024);
    }
  }
  close(conn_fd);
}

/*
ASCII vals:
A-Z: 65 >= x <= 90
a-z: 97 >= x <= 122
Vowels: 65, 69, 73, 79, 85 // 97, 101, 105, 111, 117
CAPS to lowercase:  -32
*/

/*
Given a char*, will return the number of empty spaces found + 1 (i.e the number of words)
*/
int wordCount(char* str)
{
  int wc = 1; //word count

  for (int i = 0; str[i] != '\0'; i++)
  {
    if (str[i] == ' ')
    {
      wc++;
    }
  }
  return wc;
}

/*
Given a char*, will return the number of letters (upper/lowercase) found.
*/
int letterCount(char* str)
{
  int lc = 0; //letter count
  
  for (int i = 0; str[i] != '\0'; i++)
  {
    //if str[i] is a letter, increment lc
    if (str[i] >= 65 && str[i] <= 90 || str[i] >= 97 && str[i] <= 122)
    {
      lc++;
    }
  }
  return lc;
}

/*
Given a char*, will return the number of vowels (upper/lowercase) found
*/
int vowelCount(char* str)
{
  int vc = 0;
  for (int i = 0; str[i] != '\0'; i++)
  {
    switch (str[i])
    {
      case 65:  //A
      case 69:  //E
      case 73:  //I
      case 79:  //O
      case 85:  //U
      case 97:  //a
      case 101:  //e
      case 105:  //i
      case 111:  //o
      case 117:  //u
        vc++;
        break;
      default:
        break;
    }
  }
  return vc;
}
