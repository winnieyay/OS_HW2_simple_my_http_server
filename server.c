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


char msg[200];
char get_rqst_data;
struct sockaddr_in serverInfo,clientInfo;
char *root;
pthread_t main_thread;
static pthread_mutex_t  lock = PTHREAD_MUTEX_INITIALIZER;
//char *pathname;
int sockfd = 0,forClientSockfd = 0;
int addrlen;
char inputBuffer[256] = {};
char message[] = {"Hi,this is server\n"};
char *file_or_dir;
char *host_input;
char buffer[2000];
//char ch;


void* thread(void* args);
void* threadpool_thread(void* args);
void Push(int data);
int Pop(void);
void Get_inform(void);
void print_file(void);
void get_dir(void);

struct node {
    int data;
    struct node* next;
};

struct node* Q_HEAD = NULL;
struct node* Q_TAIL = NULL;
int Q_NUM = 0;
int thread_number = 10;
int gan;
char pathname[256];


int main(int argc, char *argv[])
{

    //usage : ./server -r root -p port -n thread number
    root = argv[2];
    memset(msg,0,sizeof(msg));
    memset(msg,'\0',strlen(msg));


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
    listen(sockfd,5);
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

    pthread_create(&main_thread,NULL,thread,NULL); //create thread
    ////////////////////////////////////////////////////////////////////////////////////////////


    //thread pool
    ////////////////////////////////////////////////////////////////////////////////////////////
    pthread_t *thread_pool = (pthread_t *)malloc(sizeof(pthread_t) * thread_number);

    for(int i=0; i<thread_number; i++) {
        pthread_create(&(thread_pool[i]),NULL,threadpool_thread,(void*)&i);
        usleep(2000);
        printf("FUCK\n");
    }
    ////////////////////////////////////////////////////////////////////////////////////////////
    //while(1);
    pthread_join(main_thread, NULL);
    return 0;
}


void* thread(void* args)
{
    while(1) {
        //do
        //listen(sockfd,5);
        forClientSockfd = accept(sockfd,(struct sockaddr*) &clientInfo, &addrlen);
        //printf("forClient:%d\n",forClientSockfd);
        pthread_mutex_lock(&lock);
        Push(forClientSockfd);
        printf("in lock:%d\n",forClientSockfd);
        pthread_mutex_unlock(&lock);
        //gan = Pop();
        //printf("%d\n",gan);
        //test if main thread get request
        /////////////////////////////////////////////////////////
        //recv(forClientSockfd,inputBuffer,sizeof(inputBuffer),0);
        //printf("Get:%s\n",inputBuffer);
        //leave
        //pthread_exit(NULL);
        //memset(inputBuffer,0,sizeof(inputBuffer));
        /////////////////////////////////////////////////////////
        //send(forClientSockfd,message,sizeof(message),0);
        //pthread_exit(NULL);
        printf("lock end\n");
    }
    //pthread_exit(NULL)
}

void* threadpool_thread(void* args)
{
    while(1) {

        int id = *(int*)args;
        memset(inputBuffer,0,sizeof(inputBuffer));
        //printf("77");
        pthread_mutex_lock(&lock);
        //printf("77");
        //critical sec
        /////////////////////////////////////////////////////////
        if(isEmpty()) {
            //printf("88");
        } else {
            //printf("99");
            gan = Pop();
            //printf("gan:%d\n",gan);
            recv(gan,inputBuffer,sizeof(inputBuffer),0);
            printf("Get:%s\n",inputBuffer);
            Get_inform();
            memset(inputBuffer,0,sizeof(inputBuffer));
            //send(gan,message,sizeof(message),0);
        }
        //gan = Pop();
        //recv(gan,inputBuffer,sizeof(inputBuffer),0);
        //printf("Get:%s\n",inputBuffer);
        //memset(inputBuffer,0,sizeof(inputBuffer));
        /////////////////////////////////////////////////////////
        pthread_mutex_unlock(&lock);
    }
}
void Get_inform(void)
{

    //get request
    char* delim = " ";
    char* pch = NULL;

    pch = strtok(inputBuffer,delim);
    printf("pch:%s\n",pch);

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
    memset(pathname,0,sizeof(inputBuffer));
    printf("file_or_dir:%s\n",file_or_dir);
    printf("host_input:%s\n",host_input);
    struct stat sb;
    sprintf(pathname, "%s%s",root,file_or_dir);
    //strcat(pathname,root);
    //strcat(pathname,file_or_dir);
    printf("pathname:%s\n",pathname);

    if(stat(pathname,&sb) == 0 && S_ISDIR(sb.st_mode)) {
        printf("this is DIR\n");
        get_dir();
        //send(sockfd,request,sizeof(request),0);
        send(forClientSockfd,msg,sizeof(msg),0);
        printf("this is msg: %s",msg);
    } else if(stat(pathname,&sb) == 0 && S_ISREG(sb.st_mode)) {
        printf("this is FILE\n");
        print_file();
        send(forClientSockfd,buffer,sizeof(buffer),0);
    } else if(stat(pathname,&sb)!=0) {
        printf("nope\n");
    }
    //printf("byee\n");
}
void get_dir()
{
    DIR *d;
    struct dirent* dir;
    memset(msg,0,sizeof(msg));


    d = opendir(pathname);

    if(d) {
        while((dir = readdir(d))!=NULL) {
            if(strcmp(dir->d_name,".") && strcmp(dir->d_name,"..")) {
                char tmp[100];
                printf("%s\n", dir->d_name);
                sprintf(tmp, "%s ",dir->d_name);
                strcat(msg,tmp);
            }
            //printf("in while \n");
        }
        printf("this is msg: %s",msg);
        closedir(d);
        //printf("this is msg: %s",msg);
    }

    //printf("this is msg: %s",msg);

}
void print_file()
{

    int x = 0;
    FILE *file;
    file = fopen(pathname,"r");
    char ch;
    while((ch=fgetc(file))!=EOF) {
        buffer[x] = ch;
        x++;
    }
    fclose(file);
    /*
    if(buffer){
        printf("%s",buffer);
    }
    */
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
    struct node* ptr = Q_HEAD;
    int result = ptr->data;
    Q_HEAD = ptr->next;
    free(ptr);
    Q_NUM--;
    return result;
}
int isEmpty(void)
{
    if(Q_NUM == 0) {
        return 1;
    } else {
        return 0;
    }
}

