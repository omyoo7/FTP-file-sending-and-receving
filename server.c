#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <signal.h>
#include <ctype.h>          
#include <arpa/inet.h>
#include <netdb.h>

#define PORT 20000 
#define BACKLOG 5
#define LENGTH 1024 

void error(const char *msg){
perror(msg);
exit(1);

}
int main(){

	int sockfd; 
	int nsockfd; 
	int num;
	int sin_size; 
	struct sockaddr_in addr_local; //client address
	struct sockaddr_in addr_remote;//server address
	char revbuf[LENGTH];//recieve buf

if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1 ){
	error("Error: socket creation\n");
}
else printf("socket creation success");


addr_local.sin_family = AF_INET; //ipv4 used
addr_local.sin_port = htons(PORT);
addr_local.sin_addr.s_addr = INADDR_ANY; // AutoFill local address
bzero(&(addr_local.sin_zero), 8); // Flush the rest of struct


if( bind(sockfd, (struct sockaddr*)&addr_local, sizeof(struct sockaddr)) == -1 ){
	    error("Error in port binding\n");
	}
	else 
		printf("[Server] Binded port %d in addr 127.0.0.1 sucessfully.\n",PORT);

if(listen(sockfd,BACKLOG) == -1){
	    error("ERROR: Failed to listen Port.\n");
	}
	else
		printf ("[Server] Listening the port %d successfully.\n", PORT);

	int success = 0;
while(success == 0)
	{
	    sin_size = sizeof(struct sockaddr_in);


if ((nsockfd = accept(sockfd, (struct sockaddr *)&addr_remote, &sin_size)) == -1){
	        error("ERROR: Obtaining new Socket Despcritor.\n");
		}
	    else printf("[Server] Server has got connected from %s.\n", inet_ntoa(addr_remote.sin_addr));

//printf("Enter path of the file:\n"):
char* fs_name= "/home/omy/ssss/CP/server-client/server/temps.txt";//Path
char sdbuf[LENGTH]; // Send buffer
printf("[Server] Sending %s to the Client...", fs_name);
            FILE *fs = fopen(fs_name, "r");
            if(fs == NULL)
            {
                fprintf(stderr, "ERROR: File %s not found on server. (errno = %d)\n", fs_name, errno);
                exit(1);
            }

            bzero(sdbuf, LENGTH); 
            int fs_block_sz;
while((fs_block_sz = fread(sdbuf, sizeof(char), LENGTH, fs))>0)
            {
                if(send(nsockfd, sdbuf, fs_block_sz, 0) < 0)
                {
                    fprintf(stderr, "ERROR: Failed to send file %s. (errno = %d)\n", fs_name, errno);
                    exit(1);
                }
                bzero(sdbuf, LENGTH);
            }
            printf("Ok sent to client!\n");
            success = 1;
            close(nsockfd);
            printf("[Server] Connection with Client closed. Server will wait now...\n");
            while(waitpid(-1, NULL, WNOHANG) > 0);
}

return 0;
}
