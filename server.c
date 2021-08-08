#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>



void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main (int argc , char *argv[])  //argc = total no. of parameters we are passing 
{

    if(argc < 2)
    {
        fprintf(stderr, "Port no. not provided , program terminated !\n");
        exit(1);
    }

    int sockfd, newsockfd , portno , n; 
    char buffer[255];

    struct sockaddr_in srv , cli;
    socklen_t clilen;

    // socket creation 

    sockfd = socket(AF_INET,SOCK_STREAM,0);

    if(sockfd < 0)
    {
        error("Error opening socket");
    }
   
    bzero((char *)&srv , sizeof(srv));
    portno = atoi(argv[1]);

    // environment for socket sockaddr structure

  srv.sin_family = AF_INET;  // designates the type of address (in this case IPV4)
  srv.sin_port = htons(portno);  // port can be any number greater than 256 
  srv.sin_addr.s_addr = INADDR_ANY;  // provides ip address of my machine

   // bind the socket to local port 

   int nRet = bind(sockfd,(struct sockaddr*) &srv , sizeof(srv));

   if(nRet < 0)
   {
       error("Binding Failed");
   }

  listen(sockfd , 5); // here 5 represents no. of maximum clients can connects to the server 
  clilen = sizeof(cli);

  newsockfd = accept(sockfd , (struct sockaddr *)&cli , &clilen);

  if(newsockfd < 0)
  {
      error("Error on Accept");
  }

while(1)
{
    bzero(buffer , 255);
    n = read(newsockfd , buffer , 255 );

    if(n < 0)
    {
        error("Error on reading.");
    }

    printf("Client : %s\n",buffer);
    bzero(buffer , 255);
    fgets(buffer , 255 , stdin);

    n = write(newsockfd , buffer , strlen(buffer));

    if(n < 0)
    {
        error("Error on writing.");
    }

    int l = strncmp("Bye" ,buffer , 3);

    if(l == 0)
    {
        break;
    }

    printf("\n");

}

close(newsockfd);
close(sockfd);
return 0;

}