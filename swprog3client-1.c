/*************************************************************************/
/*Program     : swprog3client.c                                          */
/*Date        : 11/17/2014                                               */
/*Author      : Sreenidhi Krishna                                        */
/*Description : This implements a client in stop and wait protocol,      */
/*              implementing UDP File transfer, with timer.              */ 
/*                                                                       */ 
/*************************************************************************/
#include<stdio.h>
#include<fcntl.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<time.h>
#include<errno.h>
#include<stdlib.h>
#include<unistd.h>
#include<strings.h>
#include<netdb.h> 

struct frame
{
   int seqno,ackno;
   char data[500];
   int checksum;
};


void main(int argc,char*argv[])
{  
   time_t u;
   struct frame f;
   f.seqno =0;
   f.ackno =0;
   f.checksum=0;
   int k;
   struct frame *g = &f;
   int t,listenfd,n,r,i;
   char c,buff[1024],mess[50],ackpacket[5],ackpacket2[5];
   struct sockaddr_in servaddr,cliaddr;
   socklen_t servlen,clilen;
   servlen= sizeof(servaddr);
   clilen= sizeof(cliaddr);
   int nextframetosend = 1;
    ackpacket[2]='A';
    ackpacket[3]='B';
    ackpacket[4]='\0';  
   //Checking command line arguments
   if(argc<4)
   {
       printf("\nError Enter all the required details");
       exit(1);
   }
   //Creating socket
   t = (listenfd=socket(AF_INET,SOCK_DGRAM,0));
   if(t<0)
   {
      error("\nError opening socket\n");
      exit(1);
   }  
    
   //Initialise data structure
   servaddr.sin_family=AF_INET;
   servaddr.sin_addr.s_addr=INADDR_ANY;
   servaddr.sin_port=htons(atoi(argv[1]));
	
   //Initialize with zero	
   bzero((char*)&buff,sizeof(buff));  
  
   //Sending message to server: name of output file
   strcpy(mess,argv[3]);
	 
   r=sendto(listenfd,mess,sizeof(mess),0,(struct sockaddr*)&servaddr,(socklen_t )servlen);
     
   FILE *fs;
	
   //Opening input file
   fs=fopen(argv[2],"rb");
   if(fs<0)
   {
       error("\nThe file does not exist\n");
       exit(1);
    }
   srand((unsigned) time(&u));
   k = rand() % 20 ;
   
    //Checksum :1
    //ACK      :2
    //Packet   :3
    printf("\nThe value of random number generated is %d",k);
    if(k==3)
       {
       printf("\nSimulating Error: randomvalue  %d: Data packet is skipped\n",k) ;   
       k=9;//k value changed so that next packet will be received without error
        }
  
   //Read the file till end of file
   while(!feof(fs))
  {      
      
     	g->seqno = 1-nextframetosend;
	//from_network_layer(fs,buff);
	int uo;
                for(uo=0;uo<10;uo++)
                  {
		buff[uo]=' ';
	 	   }
        fread(buff,10,1,fs); 
	buff[10]='\0';
        printf("\nThe data packet send is  %s",buff);
        strcpy(g->data,buff);
        g->ackno = nextframetosend;
        int wd =0,q;
        wd = g->ackno; 
        ackpacket[0] = (char)wd;      
        q = calculatechecksum(ackpacket,5);
        ackpacket[1] = (char)q;

       if(k==1)
       {
             printf("\nSimulating Error: randomvalue  %d: Checksum changed to zero\n",k) ;
             g->checksum=0;
             k=0;
             sleep(1);
         
  
       }
       else  
             g->checksum = calculatechecksum(g->data,sizeof(g->data));

      /*

       if(k==3)
       {
       printf("\nSimulating Error: randomvalue  %d: Data packet is skipped\n",k) ;   
       k=9;
       }

        */
         int qw=0;
           do
           {             
		//     Data send to physical layer
		//   Sequence number is       g->seqno
		//   Acknowledgment number is g->ackno
		//   Checksum of the frame is g->checksum
          
                //Send data to server
                sendto(listenfd,g,sizeof(struct frame),0,(struct sockaddr*)&servaddr,(socklen_t)servlen);
          	 
                 //Timer starts
  		 for(i=0;i<4;i++)
	  	 {
    		   sleep(1);
      
  		 }
		
		 //Receive ackpacket from server
      		   qw =recvfrom(listenfd,ackpacket2,sizeof(ackpacket2),0,(struct sockaddr*)&servaddr,(socklen_t *)&servlen);
                 
  
     		   g->checksum = calculatechecksum(g->data,sizeof(g->data));


            }while((ackpacket[0]!= ackpacket2[0] && ackpacket[1]!= ackpacket2[1] )|| qw==0);

     nextframetosend = 1-nextframetosend;
     }  

      if(feof(fs))
      
    {  
    //Send an empty packet to mark the end of file transfer
      sendto(listenfd,g,0,0,(struct sockaddr*)&servaddr,(socklen_t)servlen);

      printf("\nEnd of file.\nThe data transfer is completed.\n\n\n");
     
    }

     close(listenfd);
     fclose(fs);
 }

  int calculatechecksum(const char *buf, unsigned size)
   {
	unsigned int sum = 0;
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


