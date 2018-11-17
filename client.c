#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

char request[200];

int main(int argc, char *argv[])
{
    /*
    for(int i=0;i < argc;i++){

    }
    */

    //request = NULL;
    char *file_or_dir;
    char *local_input;
    char *port_input;
    //char request;

    file_or_dir = argv[2];
    local_input = argv[4];
    port_input = argv[6];
    memset(request,0,sizeof(request));

    printf("%s\n",file_or_dir);
    sprintf(request, "GET %s HTTP/1.x\r\nHOST: %s:%s\r\n",file_or_dir,local_input,port_input);
    //request = strcat(request,"GET ");
    //request = strcat(request,file_or_dir);
    printf("%s\n",request);
    /*
    //request = strcat(request ," ");
    request = strcat(request," HTTP/1.x\r\nHOST: ");
    request = strcat(request,local_input);
    request = strcat(request,":");
    request = strcat(request,port_input);
    request = strcat(request,"\r\n");
    printf("%s\n",request);
    //check file_or_dir start with /
    //false: status 400
    */





    //set up socket
    int sockfd = 0;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd == -1) {
        printf("Fail to create a socket.");
    }

    //socket connect

    struct sockaddr_in info;
    bzero(&info,sizeof(info));
    info.sin_family = AF_INET;

    //localhost test
    info.sin_addr.s_addr = inet_addr("127.0.0.1");
    info.sin_port = htons(1234);


    int err = connect(sockfd,(struct sockaddr *)&info,sizeof(info));
    if(err==-1) {
        printf("Connection error");
    }


    //Send a message to server
    //strcpy(request,)
    //char message[] = {"Hi there"};
    char receiveMessage[100] = {};
    send(sockfd,request,sizeof(request),0);
    recv(sockfd,receiveMessage,sizeof(receiveMessage),0);

    printf("%s",receiveMessage);
    printf("close Socket\n");
    memset(request,0,sizeof(request));
    close(sockfd);
    return 0;
}