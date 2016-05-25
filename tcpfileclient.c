/*************************************************************************/
/*Program     : tcpfileclient.c                                          */
/*Date        : 11/17/2014                                               */
/*Author      : Sreenidhi Krishna                                        */
/*Description : This implements a Client in TCP File transfer.           */
/*              The program performs a file transfer from client        */ 
/*              to server based on TCP protocol                          */ 
/*************************************************************************/

#include<sys/types.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<strings.h>
#include<string.h>
#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 


void error(char * message)
{
   perror(message);
   exit(1);
}

int main(int argc,char* argv[])
{
   int sockfd;
   char fname[25];
   char buffer[200],inputfilename[50],outputfilename[50];
   int len,portno,m;
   struct hostent *server;
   struct sockaddr_in servaddr,cliaddr;
   printf("%s %s %s %s %s \n",argv[0],argv[1],argv[2],argv[3],argv[4]);
   
   //Checking for command line arguments
   if(argc<5)
   { 
	 printf("\nEnter all the details(port number -hostname-input filename-output filename \n");
     exit(1);
   }
   //Converting from string to integer 
   portno = atoi(argv[2]);  
   
   //Creating socket
   sockfd=socket(AF_INET,SOCK_STREAM,0);
   
   if(sockfd<0)
   {
      error("\nError opening socket\n");
   }
   server=gethostbyname(argv[1]);
   if (server == NULL)
   {
      fprintf("\nError, no such host\n");
      exit(1);
   }
   //Initialising with zero
   bzero((char*)&servaddr,sizeof(servaddr)); 
   bzero((char*)&buffer,sizeof(buffer));  
    
  //Initialise data structure
    servaddr.sin_family=AF_INET;   
  servaddr.sin_port=htons(portno);
  
  //Connecting to socket
  m = connect(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr));
  if(m<0)
  {
     error("\nError connecting to client socket\n");   
  }
  
  //Transferring input and output file name
  strcpy(inputfilename, argv[3]);
  strcpy(outputfilename, argv[4]);  	
  
  //Writing to the socket
  write(sockfd,inputfilename,sizeof(inputfilename));
  write(sockfd,outputfilename,sizeof(outputfilename));
  
  //Open the file that you wish to transfer
  FILE *fp;
  fp=fopen(inputfilename,"rb");
  if(fp<=0)
  {
     printf("\nCould not open file %s \n",inputfilename);	  
     error("\nError opening file\n");
     exit(1);
  }  
  
  //Read data from file - continue reading data till end of file
  while(!feof(fp))
  {
     int nread;
     nread = fread(buffer, 10, 1, fp);
     //If read successfully, send data as 10 bytes each
     if(nread>0)
     {
         printf("Data read from the input file is %s \n",buffer);
         write(sockfd,buffer,10);
     }

   }
   if(feof(fp))
   printf("\n End of file reached.\n");
   close(sockfd);
   fclose(fp);
   return 0;

}

