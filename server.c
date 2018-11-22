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
#include "status.h"

char msg[10000000];
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
char buffer[2000000];
void* thread(void* args);
void* threadpool_thread(void* args);
void Push(int data);
int Pop(void);
void Get_inform(void);
void print_file(void);
void get_dir(void);
//void _mkdir(const char *path);

struct node {
    int data;
    struct node* next;
};
//struct extn *checkex;
struct node* Q_HEAD = NULL;
struct node* Q_TAIL = NULL;
int Q_NUM = 0;
int thread_number = 10;
int gan;
char pathname[256];
int thread_number_input;
int port;
int main(int argc, char *argv[])
{

    //usage : ./server -r root -p port -n thread number
    root = argv[2];
    port = atoi(argv[4]);
    thread_number_input = atoi(argv[6]);
    //port = atoi(argv[4]);
    //memset(msg,0,sizeof(msg));
    memset(msg,'\0',strlen(msg));

    printf("THTH:%d\n",thread_number_input);
    printf("PORT:%d\n",port);
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
    //SERVER
    serverInfo.sin_family = AF_INET;
    serverInfo.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverInfo.sin_port = htons(port);

    bind(sockfd,(struct sockaddr *)&serverInfo,sizeof(serverInfo));
    listen(sockfd,5);

    //main thread
    ////////////////////////////////////////////////////////////////////////////////////////////
    pthread_create(&main_thread,NULL,thread,NULL); //create thread
    ////////////////////////////////////////////////////////////////////////////////////////////

    //thread pool
    ////////////////////////////////////////////////////////////////////////////////////////////
    pthread_t *thread_pool = (pthread_t *)malloc(sizeof(pthread_t) * thread_number_input);
    for(int i=0; i<thread_number_input; i++) {
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
        //printf("in lock:%d\n",forClientSockfd);
        pthread_mutex_unlock(&lock);
        //printf("lock end\n");
    }
    //pthread_exit(NULL)
}

void* threadpool_thread(void* args)
{
    while(1) {

        int id = *(int*)args;
        //memset(inputBuffer,0,sizeof(inputBuffer));
        pthread_mutex_lock(&lock);
        /////////////////////////////////////////////////////////
        if(isEmpty()) {

        } else {
            memset(msg,0,sizeof(msg));
            gan = Pop();
            recv(gan,inputBuffer,sizeof(inputBuffer),0);
            printf("Get:%c\n",inputBuffer[4]);
            printf("Get:%s\n",inputBuffer);
            if(inputBuffer[4] != '/') {
                //printf("AAAAA\n");
                sprintf(msg,"HTTP/1.x 400 BAD_REQUEST\nContent-type: \nServer: httpserver/1.x\n\n");
                //printf("hello : \n%s\n",msg);
                send(forClientSockfd,msg,sizeof(msg),0);
            }

            else if( inputBuffer[0] != 'G') {
                sprintf(msg,"HTTP/1.x 405 METHOD_NOT_ALLOWED\nContent-type: \nServer: httpserver/1.x\n\n");
                send(forClientSockfd,msg,sizeof(msg),0);
            } else {

                Get_inform();
            }
            memset(inputBuffer,0,sizeof(inputBuffer));
        }
        pthread_mutex_unlock(&lock);
    }
}

void Get_inform(void)
{

    //get request
    char* delim = " ";
    char* pch = NULL;

    pch = strtok(inputBuffer,delim);
    //printf("pch:%s\n",pch);

    for(int i=0; i<4; i++) {
        if(i == 1) {
            file_or_dir = pch;
        }
        if(i == 3) {
            host_input = pch;
        }
        pch = strtok(NULL,delim);
    }

    memset(msg,0,sizeof(msg));
    memset(msg,'\0',strlen(msg));
    memset(pathname,0,sizeof(pathname));

    struct stat sb;
    sprintf(pathname, "%s%s",root,file_or_dir);
    printf("pathname:%s\n",pathname);
    char checktttt[1000000];
    memset(checktttt,0,sizeof(checktttt));
    strcat(checktttt,file_or_dir);
    int t_flag = -1;
    //cut extention
    for(int y=0; y<strlen(checktttt); y++) {
        if(checktttt[y] == '.') {
            //printf("INN\n");
            t_flag =0;
            //FILE
        }
    }
    int flag55 = -1;
    int flag_o = 0;
    if(t_flag == 0) {

        //FILE
        char* delim3 = ".";
        char* expt = NULL;
        char copy_for_cut[100000];
        memset(copy_for_cut,0,sizeof(copy_for_cut));
        strcpy(copy_for_cut,file_or_dir);
        //printf("yooooo;%s\n",copy_for_cut);
        expt = strtok(copy_for_cut,delim3);
        expt = strtok(NULL,delim3);
        //printf("yooooo:*---%s\n",expt);
        //int flag55 = 0;

        for(int i=0; i<8; i++) {
            if(strcmp(extensions[i].ext,expt) == 0) {
                memset(msg,0,sizeof(msg));
                flag55=0;
            } else {
                //printf("yooo\n");
            }
        }
        if(flag55 == -1) {
            flag_o = -1;
            //printf("gooo\n");
            sprintf(msg,"HTTP/1.x 415 UNSUPPORT_MEDIA_TYPE\nContent-type: \nServer: httpserver/1.x\n\n\n");
            send(forClientSockfd,msg,sizeof(msg),0);
        }
    }
    if(flag_o == 0) {
        //printf("goooooooo\n");
        if(stat(pathname,&sb) == 0 && S_ISDIR(sb.st_mode)) {

            memset(msg,0,sizeof(msg));
            printf("this is DIR\n");
            sprintf(msg,"HTTP/1.x 200 OK\nContent-type: directory\nServer: httpserver/1.x\n\n\n");

            get_dir();
            printf("HI\n");
            //printf("in this is DiR:%s\n",msg);
            send(forClientSockfd,msg,sizeof(msg),0);
            printf("this is msg:\n%s",msg);
        } else if(stat(pathname,&sb) == 0 && S_ISREG(sb.st_mode)) {
            memset(msg,0,sizeof(msg));
            printf("this is FILE\n");

            char copy_for_cut2[100000];
            memset(copy_for_cut2,0,sizeof(copy_for_cut2));
            strcpy(copy_for_cut2,file_or_dir);
            char* delim1 = ".";
            char* p = NULL;
            //printf("1\n");
            p = strtok(copy_for_cut2,delim1);

            p = strtok(NULL,delim1);

            int flag=-1;
            printf("p2:%s\n",p);
            for(int i=0; i<8; i++) {
                //printf("11\n");
                if(strcmp(extensions[i].ext,p) == 0) {
                    flag=0;
                    sprintf(msg,"HTTP/1.x 200 OK\nContent-type: %s\nServer: httpserver/1.x\n\n\n",extensions[i].mime_type);

                } else {
                    //printf("yooo\n");
                }
            }
            if(flag == 0) {
                //printf("this is FILE2\n");

                print_file();
                //printf("msg:%s\n",msg);
                strcat(msg,buffer);
            } else if(flag == -1) {

                sprintf(msg,"HTTP/1.x 415 UNSUPPORT_MEDIA_TYPE\nContent-type: \nServer: httpserver/1.x\n\n");
                //printf("msg:%s\n",msg);
            }
            //strcat(msg,buffer);
            send(forClientSockfd,msg,sizeof(msg),0);


        } else if(stat(pathname,&sb)!=0) {
            //printf("nope\n");
            //??????????????????????????????????????????????
            sprintf(msg,"HTTP/1.x 404 NOT_FOUND\nContent-type: \nServer: httpserver/1.x\n\n");
            ////////////////////////////////////////////////////////
            send(forClientSockfd,msg,sizeof(msg),0);

        }


    }

}
void get_dir()
{
    DIR *d;
    printf("in get_dir\n");
    struct dirent* dir;
    //memset(msg,0,sizeof(msg));
    d = opendir(pathname);
    printf("in get_dir\n");
    if(d) {
        printf("in get_dir\n");
        while((dir = readdir(d))!=NULL) {
            if(strcmp(dir->d_name,".") && strcmp(dir->d_name,"..")) {
                char tmp[100];
                //printf("%s\n", dir->d_name);
                sprintf(tmp, "%s ",dir->d_name);
                printf("IIII\n");
                strcat(msg,tmp);
            }
        }
        strcat(msg,"\n");
        printf("in get dir2:\n%s",msg);
        closedir(d);
    }
}
void print_file()
{
    memset(buffer,0,sizeof(buffer));
    //printf("in print\n");
    int x = 0;
    FILE *file;
    file = fopen(pathname,"r");
    char ch;
    while((ch=fgetc(file))!=EOF) {
        buffer[x] = ch;
        x++;
    }
    printf("in print bye bye\n");
    fclose(file);
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

