#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <dirent.h>
#include <sys/stat.h>


char msg[10000];
char get_rqst_data;

int main(int argc, char *argv[])

{

    //usage : ./server -r root -p port -n thread number


    //??
    //int thread_number = (int)(argv[6]);

    char *root;
    root = argv[2];


    memset(msg,0,sizeof(msg));


    //set up socket
    char inputBuffer[256] = {};
    char message[] = {"Hi,this is server.\n"};
    int sockfd = 0,forClientSockfd = 0;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd == -1) {
        printf("Fail to create a socket.");
    }

    //socket connection
    struct sockaddr_in serverInfo,clientInfo;
    int addrlen = sizeof(clientInfo);
    bzero(&serverInfo,sizeof(serverInfo));

    serverInfo.sin_family = AF_INET;
    serverInfo.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverInfo.sin_port = htons(1234);
    bind(sockfd,(struct sockaddr *)&serverInfo,sizeof(serverInfo));
    listen(sockfd,5);

    while(1) {
        forClientSockfd = accept(sockfd,(struct sockaddr*) &clientInfo, &addrlen);
        recv(forClientSockfd,inputBuffer,sizeof(inputBuffer),0);
        printf("Get:%s\n",inputBuffer);
        //get request
        char* delim = " ";
        char* pch = NULL;
        pch = strtok(inputBuffer,delim);
        printf("pch:%s\n",pch);
        //int i=0;
        int c = 0;
        char *file_or_dir;
        char *host_input;
        /*
        while(pch!=NULL){
            printf("%s\n",pch);
            pch = strtok(NULL,delim);
            //i++;
        }
        */


        for(int i=0; i<4; i++) {
            //printf("%d : %s\n",i,pch);
            if(i == 1) {
                file_or_dir = pch;
            }
            if(i == 3) {
                host_input = pch;
            }
            pch = strtok(NULL,delim);
        }
        //printf("dir : %s\n",file_or_dir);

        //printf("Get:%s\n",inputBuffer);
        //check path or dir


        send(forClientSockfd,message,sizeof(message),0);
        //printf("Get:%s\n",inputBuffer);
    }
    return 0;
}

/*
void* thread(void* args){
    //do

    //leave
    pthread_exit(NULL);
}
*/