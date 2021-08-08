#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>


void error(const char *msg)
{
    perror(msg);
    exit(1);
}
int main (int argc , char *argv[])  //argc = total no. of parameters we are passing 
{

    int sockfd, portno , n; 
    char buffer[255];

    struct sockaddr_in srv;
    struct hostent *server;

        if(argc < 3)
    {
        fprintf(stderr, "Usage %s hostname port\n",argv[0]);
        exit(1);
    }

       portno = atoi(argv[2]);

    // socket creation 

    sockfd = socket(AF_INET,SOCK_STREAM,0);

    if(sockfd < 0)
    {
        error("Error opening socket");
    }
   
   server = gethostbyname(argv[1]); 

   if(server == NULL)
   {
       fprintf(stderr,"Error , no such host");
   }

   bzero((char *)&srv , sizeof(srv));
   
  srv.sin_family = AF_INET;  // designates the type of address (in this case IPV4)
   
   bcopy((char*) server->h_addr , (char*)&srv.sin_addr.s_addr , server->h_length);
   
  srv.sin_port = htons(portno);  // port can be any number greater than 256 
  srv.sin_addr.s_addr = INADDR_ANY;  // provides ip address of my machine

  if(connect(sockfd , (struct sockaddr *)&srv , sizeof(srv)) < 0)
  {
      error("Connection Faliled");
  }

while(1)
{
    bzero(buffer , 255);
    fgets(buffer , 255 , stdin);
    n = write(sockfd , buffer , strlen(buffer));

    if(n < 0)
    {
        error("Error on writing .");
    }

    bzero(buffer , 255);

    n = read(sockfd , buffer , 255);

    if(n < 0)
    {
        error("Error on reading ");
    }

    printf("Server : %s\n",buffer);

     int l = strncmp("Bye" ,buffer , 3);

    if(l == 0)
    {
        break;
    }

     printf("\n");
}

close(sockfd);
return 0;

}