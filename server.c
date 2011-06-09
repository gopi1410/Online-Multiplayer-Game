//USAGE: ./a.out port_number



//#define DEST_IP "172.31.4.169"
//#define DEST_PORT 5000
#define BACKLOG 5

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

int main(int argc, char *argv[])
{
     int sockfd, newsockfd;
     socklen_t clilen;
     typedef struct sendfloat
	 {
		float alpha,phi,r;
	 } x;
	 x buffer;
	 x* bptr;
	 float a,b,c;
     struct sockaddr_in serv_addr, cli_addr;
     int n,DEST_PORT=atoi(argv[1]);
     //char end[]={"exit\n"};
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0)
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY; //uses self ip address
     serv_addr.sin_port = htons(DEST_PORT);
	 memset(&(serv_addr.sin_zero),'\0',8);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
		error("ERROR on binding");
		
	listen(sockfd,BACKLOG);
	
	while(1)
	{
		a=0.0;b=0.0;c=0.0;
		clilen = sizeof(cli_addr);
		newsockfd = accept(sockfd,(struct sockaddr *) &cli_addr,&clilen);
		if (newsockfd < 0)
			error("ERROR on accept");
		//bzero(buffer,256);
		buffer.alpha=0.0;
		buffer.phi=0.0;
		buffer.r=0.0;
		bptr=&buffer;
		n = read(newsockfd,bptr,255);
		if (n < 0)
			error("ERROR reading from socket");
		printf("Here is the message: %f  %f  %f\n",bptr->alpha,bptr->phi,bptr->r);
		printf("Enter the message: ");
		//bzero(buffer,256);
		//fgets(buffer,255,stdin);
		buffer.alpha=0.0;
		buffer.phi=0.0;
		buffer.r=0.0;
		scanf("%f%f%f",&a,&b,&c);
		buffer.alpha=a;
		buffer.phi=b;
		buffer.r=c;
		bptr=&buffer;
		n = write(newsockfd,bptr,sizeof(buffer));
		if (n < 0)
			error("ERROR writing to socket");
		//if(!strcmp(buffer,end))
			//break;
	}
	
	close(newsockfd);
	close(sockfd);
	return 0;
}
