/*************************************************************************/
/*Program     : swprog3server.c                                          */
/*Date        : 11/17/2014                                               */
/*Author      : Sreenidhi Krishna                                        */
/*Description : This implements a server in stop and wait protocol,      */
/*              implementing UDP File transfer, with timer.              */ 
/*                                                                       */ 
/*************************************************************************/
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>
#include <netdb.h> 


struct frame
{
   int seqno,ackno;
   char data[500];
   int checksum;
};


void main(int argc,char*argv[])
{   int sfd,r,b,y,a,k;
    char c,buff[1024];
    char mess[50],ackpacket[5];
    time_t u;
    struct sockaddr_in servaddr,cliaddr;
    socklen_t clilen,servlen;
    servlen= sizeof(servaddr);
    clilen= sizeof(cliaddr);
    struct frame f;
    f.seqno =0;
    f.ackno =0;
    f.checksum=0;
    struct frame *g;
    g = &f;
    int frameexpected = 0,s;
    ackpacket[2]='A';
    ackpacket[3]='B';
    ackpacket[4]='\0';
    //Cheching command line arguments
    if(argc<2)
    {
        printf("\nError No port number provided");
        exit(1);
    }
     
    //Creating socket
    sfd=socket(AF_INET,SOCK_DGRAM,0);
    if(sfd<0)
    {
        error("\nError in opening a socket");
    }
    
    //Initialising to zero
    bzero((char *)&servaddr,sizeof(servaddr));
    bzero((char*)&buff,sizeof(buff));  
	
    //Initialise data structure
    servaddr.sin_family=AF_INET;
    servaddr.sin_addr.s_addr=INADDR_ANY;
    servaddr.sin_port=htons(atoi(argv[1]));
    b=bind(sfd,(struct sockaddr*)&servaddr,servlen);
	
    //Receiving the message from client :name of output file 
    recvfrom(sfd,mess,sizeof(mess),0,(struct sockaddr*)&cliaddr,(socklen_t *)&clilen);
	
    srand((unsigned) time(&u));
    k = rand() % 20 ;	
    printf("\nThe value of random number generated is %d",k);
   
    //Frame from_physical_layer
    y = recvfrom(sfd,g,sizeof(struct frame),0,(struct sockaddr*)&cliaddr,(socklen_t *)&clilen);

		FILE *ft;
  		 if(ft<0)
	  	error("\nError in opening file");
          	else ft = fopen(mess,"ab");

   while(y>0)
   {  
             
           //Data packet received from sender
         
           a = calculatechecksum(g->data,sizeof(g->data));
          
           if(g->seqno == frameexpected && a == g->checksum)
           {    int uo;
                for(uo=0;uo<10;uo++)
                  {
		buff[uo]=' ';
	 	   }
                strcpy(buff,g->data);             
               // to_network_layer(buff,mess); 
               //Writing data on to destination file
          	 
  		 fwrite(buff,5,2,ft);
   		 printf("\nThe data packet received is %s",buff);  
                
		if(k==2)  
                {
                   printf("\nSimulating Error: randomvalue %d :ACK skipped\n",k);
                   g->ackno = k;
                   k=0;
                }
                else
                g->ackno = 1-g->seqno;
           
             //Calculating checksum for acknowledgment packet

                 s = calculatechecksum(ackpacket,5);

            //Acknowledgment number and checksum written into the packet
                 int wd = 0;
                 wd = g->ackno;
                 ackpacket[0] = (char)wd;
                 ackpacket[1] = (char)s;
  
           //Sending acknowledgment packet     
         
                 sendto(sfd,ackpacket,sizeof(ackpacket),0,(struct sockaddr*)&cliaddr,(socklen_t )clilen);
             
                frameexpected = 1-frameexpected;               
           } 
           else
           {
                   	        	  
            	  printf("\nError in the packet received");
            	  ackpacket[0]= (char)frameexpected;
            
               	  s = calculatechecksum(ackpacket,sizeof(ackpacket));
               	  ackpacket[1] = (char)s;

            	  //Acknowledgment of previous packet send                            
               	  sendto(sfd,ackpacket,sizeof(ackpacket),0,(struct sockaddr*)&cliaddr,(socklen_t )clilen);
            	  sleep(1);
 

	    } 
           
   //Frame from_physical_layer
   y = recvfrom(sfd,g,sizeof(struct frame),0,(struct sockaddr*)&cliaddr,(socklen_t *)&clilen);
     
  } 
 printf("\nThe data transfer is completed\n\n");
      fclose(ft);
      close(sfd); 
 }
 
int calculatechecksum(const char *buf, unsigned size)
{
	unsigned sum = 0;
	int i;
	/* Accumulate checksum */
	for (i = 0; i < size - 1; i += 2)
	{
		unsigned short word16 = *(unsigned short *) &buf[i];
		sum += word16;
	}
	
	/* Handle odd-sized case */
	if (size & 1)
	{
		unsigned short word16 = (unsigned char) buf[i];
		sum += word16;
	}
	/* Fold to get the ones-complement result */
	while (sum >> 16) sum = (sum & 0xFFFF)+(sum >> 16);
	
	/* Invert to get the negative in ones-complement arithmetic */
	return ~sum;
	
 }

