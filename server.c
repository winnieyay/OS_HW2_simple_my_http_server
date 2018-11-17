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
#include <pthread.h>


char msg[10000];
char get_rqst_data;
struct sockaddr_in serverInfo,clientInfo;
char *root;
pthread_t main_thread;
void* thread(void* args);
int sockfd = 0,forClientSockfd = 0;
int addrlen;
char inputBuffer[256] = {};
char message[] = {"Hi,this is server\n"};
struct node {

    int data;
    struct *node next;
};

struct node* Q_HEAD = NULL;
struct node* Q_TAIL = NULL;
int Q_NUM = 0;


int main(int argc, char *argv[])

{

    //usage : ./server -r root -p port -n thread number
    root = argv[2];
    memset(msg,0,sizeof(msg));

    //set up socket
    //char inputBuffer[256] = {};
    char message[] = {"Hi,this is server.\n"};
    //int sockfd = 0,forClientSockfd = 0;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd == -1) {
        printf("Fail to create a socket.");
    }

    //socket connection
    //struct sockaddr_in serverInfo,clientInfo;
    addrlen = sizeof(clientInfo);
    bzero(&serverInfo,sizeof(serverInfo));

    serverInfo.sin_family = AF_INET;
    serverInfo.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverInfo.sin_port = htons(1234);
    bind(sockfd,(struct sockaddr *)&serverInfo,sizeof(serverInfo));
    //listen(sockfd,5);
    /*
    while(1) {
        //forClientSockfd = accept(sockfd,(struct sockaddr*) &clientInfo, &addrlen);


        recv(forClientSockfd,inputBuffer,sizeof(inputBuffer),0);
        printf("Get:%s\n",inputBuffer);
        //get request
        char* delim = " ";
        char* pch = NULL;
        pch = strtok(inputBuffer,delim);
        printf("pch:%s\n",pch);

        int c = 0;
        char *file_or_dir;
        char *host_input;

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


        send(forClientSockfd,message,sizeof(message),0);
        //printf("Get:%s\n",inputBuffer);
    }
    */
    //main thread
    ////////////////////////////////////////////////////////////////////////////////////////////

    //malloc
    pthread_create(&main_thread,NULL,thread,NULL); //create thread

    //do something

    pthread_join(main_thread,NULL); // wait for thread to complete
    ////////////////////////////////////////////////////////////////////////////////////////////
    //while(1);
    return 0;
}


void* thread(void* args)
{
    while(1) {

        //do
        listen(sockfd,5);
        forClientSockfd = accept(sockfd,(struct sockaddr*) &clientInfo, &addrlen);
        recv(forClientSockfd,inputBuffer,sizeof(inputBuffer),0);
        printf("Get:%s\n",inputBuffer);
        //leave
        //pthread_exit(NULL);
        memset(inputBuffer,0,sizeof(inputBuffer));
        send(forClientSockfd,message,sizeof(message),0);
        //pthread_exit(NULL);
    }
    //pthread_exit(NULL);
}


void Push(int data)
{
    if(Q_HEAD == NULL) {
        Q_HEAD = (struct node*)malloc(sizeof(struct node));
        Q_HEAD->data = data;
        Q_HEAD->next = NULL;
        Q_TAIL = Q_HEAD;
    } else {
        struct node* ptr = (struct node*)malloc(sizeof(struct node));
        ptr->data = data;
        ptr->next = NULL;
        Q_TAIL->next = ptr;
        Q_TAIL = ptr;
    }
    Q_NUM++;
}

int Pop(void)
{
    struct *node = ptr

    }
