/*************************************************************************/
/*Program     : tcpfileserver.c                                          */
/*Date        : 11/17/2014                                               */
/*Author      : Sreenidhi Krishna                                        */
/*Description : This implements a server in TCP File transfer.           */ 
/*              The program performs a file transfer from client        */ 
/*              to server based on TCP protocol                          */ 
/*************************************************************************/
#include<arpa/inet.h>
#include<unistd.h>
#include<stdlib.h>
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
char inputfilename[50],outputfilename[50],buffer2[50];

int main(int argc,char*argv[])
{
 int fd,sockfd,socketfd,newsockfd;
 struct sockaddr_in servaddr,cliaddr;
 pid_t childpid;
 char buffer[200];
 int portno,t,i,rb;
 socklen_t client;

 //Checking for command line arguments
 if(argc<2)
 {
    printf("\nError No port number provided\n");
    exit(1);
 }
 //Creating socket
 socketfd=socket(AF_INET,SOCK_STREAM,0);
 if(socketfd<0)
 {
   error("\nError in opening a server socket\n");
 }
 
 //Initialising with zero
 bzero((char *)&servaddr,sizeof(servaddr));
 bzero((char *)&buffer, sizeof(buffer));
 
 //Converting from string to integer
 portno = atoi(argv[1]);

 //Initialise data structure
 servaddr.sin_family=AF_INET;
 servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
 servaddr.sin_port=htons(portno);

 bind(socketfd,(struct sockaddr*)&servaddr,sizeof(servaddr));

 //Listen for connections,Set max waitlist as 5
 listen(socketfd,5);
 int p = sizeof(cliaddr);
 
 //Accept the connection
 newsockfd=accept(socketfd,(struct sockaddr*)&cliaddr,&p);
 if(newsockfd<0)
  {
    error("\nError in accepting");
  }
 //Reading the name of input and output file
 for(i=0;i<2;i++)
 {
    read(newsockfd,buffer2,50); 
 	if(i==0)
 	{
       strcpy(inputfilename, buffer2);
    } 
    if(i==1)
    {
	   strcpy(outputfilename, buffer2);  	
    }
 }  
 //Create a file where data will be stored  
 FILE *f;
 f=fopen(outputfilename,"ab");
 if(f<0)
 {
    error("\nError opening file\n");  
 }
 rb = read(newsockfd,buffer,10);
 while(rb>0)   
 {
    //Writing to the file as 5 bytes each
    printf("\nThe data received is %s\n",buffer);
    fwrite(buffer,5,2,f);
    rb = read(newsockfd,buffer,10);
 }  
 printf("\nEnd of file\n");   
 printf("\nThe file was received succesfully"); 
 printf("\nThe new file created is %s \n", outputfilename); 
 close(newsockfd); 
 close(sockfd); 
 fclose(f); 
 return 0;  
   
}
