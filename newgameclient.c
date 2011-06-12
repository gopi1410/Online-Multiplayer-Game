//USAGE: ./a.out ip_address



//#define DEST_IP "172.31.4.170"
//#define DEST_PORT 5000

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
	int sockfd,n;
	struct sockaddr_in dest_addr; //struct to hold destination address
	struct hostent *dest;
	char buff1[256];
	typedef struct sendfloat
	 {
		float alpha,phi,r;
	 } x;
	 x buff;
	 x* bptr;
	 float a,b,c;
	//char end[]={"exit\n"};
	int DEST_PORT=5000;
	char *DEST_IP=argv[2];
	
	while(1)
	{
		a=0.0;b=0.0;c=0.0;
		sockfd=socket(AF_INET,SOCK_STREAM,0);
		if(sockfd<0)
		{
			error("ERROR opening socket");
		}
		dest=gethostbyname(DEST_IP);
		if(dest==NULL)
		{
			fprintf(stderr,"ERROR!! Invalid IP");
			exit(0);
		}
		bzero((char *) &dest_addr, sizeof(dest_addr));
		//bcopy((char *)dest->h_addr,(char *)&dest_addr.sin_addr.s_addr,dest->h_length);
		dest_addr.sin_family=AF_INET;
		dest_addr.sin_port=htons(DEST_PORT);
		dest_addr.sin_addr.s_addr=inet_addr(DEST_IP);
		memset(&(dest_addr.sin_zero),'\0',8);
				
		if(connect(sockfd,(struct sockaddr *)&dest_addr,sizeof(dest_addr))<0)
			error("Error!! Could not connect");
		printf("Enter the message: ");
		//bzero(buff,256);
		//fgets(buff,255,stdin);
		buff.alpha=0.0;
		buff.phi=0.0;
		buff.r=0.0;
		bptr=&buff;
		scanf("%f%f%f",&a,&b,&c);
		buff.alpha=a;
		buff.phi=b;
		buff.r=c;
		bptr=&buff;
		n=write(sockfd,bptr,sizeof(buff));
		if (n < 0)
			 error("ERROR writing to socket");
		//if(!strcmp(buff,end))
			//break;
		//bzero(buff,256);
		buff.alpha=0.0;
		buff.phi=0.0;
		buff.r=0.0;
		bptr=&buff;
		bzero(buff1,256);
		n=read(sockfd,buff1,255);
		if (n < 0)
			 error("ERROR reading from socket");
		printf("%s\n",buff);
		close(sockfd);
	}
	close(sockfd);
	return 0;
}
