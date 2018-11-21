#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>


char request[10000];
//char requesttmp[10000];
pthread_t my_thread;
char receiveMessage[100000] = {};
char copyreceive[100000] = {};
char copyreceive2[100000] = {};
char output[100000]= {};
int sockfd = 0;
char content_line[1000000]= {};
char dir_down[1000000]= {};
char *file_or_dir;
char *local_input;
char *port_input;
char file_or_dir_tmp[100000];
void* thread(void* args);
static pthread_mutex_t  lock = PTHREAD_MUTEX_INITIALIZER;
void _mkdir(const char *path);

int main(int argc, char *argv[])
{

    file_or_dir = argv[2];
    local_input = argv[4];
    port_input = argv[6];
    memset(request,0,sizeof(request));

    //printf("%s\n",file_or_dir);
    sprintf(request, "GET %s HTTP/1.x\r\nHOST: %s:%s\r\n",file_or_dir,local_input,port_input);
    //printf("%s\n",request);

    //set up socket

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd == -1) {
        printf("Fail to create a socket.");
    }

    //socket connect

    struct sockaddr_in info;
    bzero(&info,sizeof(info));
    info.sin_family = AF_INET;
    int tm = 0;
    //localhost test
    info.sin_addr.s_addr = inet_addr("127.0.0.1");
    info.sin_port = htons(1234);


    int err = connect(sockfd,(struct sockaddr *)&info,sizeof(info));
    if(err==-1) {
        printf("Connection error");
    }

    //char message[] = {"Hi there"};

    send(sockfd,request,sizeof(request),0);


    //??
    memset(request,0,sizeof(request));
    memset(copyreceive,0,sizeof(copyreceive));
    recv(sockfd,receiveMessage,sizeof(receiveMessage),0);
    close(sockfd);
    //strcat(output,receiveMessage);
    strcpy(copyreceive,receiveMessage);

    memset(request,0,sizeof(request));
    //memset(receiveMessage,0,sizeof(receiveMessage));

    char* delim1 = "\n";
    char* pp = NULL;

    pp = strtok(copyreceive,delim1);
    for(int i=0; i<4; i++) {
        if(i==1) {
            //printf("try recursive:%s\n",pp);
            strcpy(content_line,pp);
            //printf("try recursive111:%c\n",content_line[14]);
        }
        if(content_line[14] == 'd') {
            if(i==3) {
                strcpy(dir_down,pp);
                //printf("try recursive111:%s\n",dir_down);
            }
        }
        pp = strtok(NULL,delim1);
    }

    printf("%s\n",receiveMessage);

    pthread_t *threadyyptr[100];
    if(content_line[14] == 'd') {
        printf("GOGO\n");
        //// if no inside??????????????????????????
        char* delim111 = " ";
        char* ppp = NULL;
        char* ptrr[100000];

        ppp = strtok(dir_down,delim111);
        tm = 0;
        if(dir_down == NULL) {
            printf(":)\n");
        }
        while(ppp!=NULL) {
            ptrr[tm] = ppp;
            //printf("ptrr[%d]:%s\n",tm,ptrr[tm]);
            tm++;
            ppp = strtok(NULL,delim111);
        }
        int j=0;
        char requesttmp;
        while(j<tm) {
            memset(file_or_dir_tmp,0,sizeof(file_or_dir_tmp));
            sprintf(file_or_dir_tmp,"%s/%s",file_or_dir,ptrr[j]);
            //printf("\n()()()file_tmp:%s\n",file_or_dir_tmp);
            memset(request,0,sizeof(request));
            sprintf(request,"GET %s/%s HTTP/1.x\r\nHOST: %s:%s\r\n\r\n",file_or_dir,ptrr[j],local_input,port_input);
            //printf("\ncheck:\n%s\n",request);

            pthread_create(&threadyyptr[j],NULL,thread,file_or_dir_tmp);
            j++;
            //pthread_create(&(thread_poo[i]),NULL,threadpool_thread,(void*)&i);
            sleep(1);
        }

    } else if(content_line[14] == 't' || content_line[14] == 'a') {
        printf("MMM come in\n");

        char mk_path_tmppp[1000000] = {};
        memset(mk_path_tmppp,0,sizeof(mk_path_tmppp));
        sprintf(mk_path_tmppp,"output");
        //printf("FUCK:%s\n",mk_path_tmppp);
        _mkdir(mk_path_tmppp);

        char copyreceivee[100000] = {};
        strcpy(copyreceivee,receiveMessage);

        char gett[100000] = {};
        char* delim15 = "\n";
        char* ppr = NULL;
        int c=0;
        ppr = strtok(copyreceivee,delim15);
        while(ppr!=NULL) {

            if(c>=3) {

                //sprintf(gett,"%s\n",ppr);
                strcat(gett,ppr);
                strcat(gett,"\n");
                //printf("try recursive:%s\n",ppr);
                //ppr = strtok(NULL,delim15);
            }
            ppr = strtok(NULL,delim15);
            c++;
        }
        //printf("\ntry recursive55:\n%s\n",gett);


        char path_in_write[1000000]= {};
        sprintf(path_in_write,"output%s",file_or_dir);
        FILE *fp = fopen(path_in_write,"w+");
        fprintf(fp,"%s",gett);
        fclose(fp);



    } else {
        close(sockfd);
    }
    for(int s=0; s<tm; s++) {
        pthread_join(threadyyptr[s], NULL);
    }
    //pthread_join(threadyy, NULL);
    //close(sockfd);
    memset(receiveMessage,0,sizeof(receiveMessage));
    return 0;
}
void _mkdir(const char *path)
{

    char formk[200];
    memset(formk,0,sizeof(formk));
    char* ptr_in_mk = NULL;

    strncpy(formk,path,sizeof(formk));
    for(ptr_in_mk =formk +2; *ptr_in_mk; ptr_in_mk++) {
        if(*ptr_in_mk == '/') {
            *ptr_in_mk = '\0';
            mkdir(formk, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
            *ptr_in_mk = '/';
        }
    }
    mkdir(formk, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
}
void* thread(void* args)
{
    char* tmp_path = (char*)args;
    //printf("\n this is path in th\n (((((%s)))))\n",tmp_path);
    char request_tmp[100000];
    memset(request_tmp,0,sizeof(request_tmp));

    sprintf(request_tmp,"GET %s HTTP/1.x\r\nHOST: %s:%s\r\n\r\n",tmp_path,local_input,port_input);

    int socktmp=0;
    socktmp = socket(AF_INET, SOCK_STREAM, 0);
    if (socktmp == -1) {
        printf("Fail to create a socket.");
    }

    //memset(tmp_path,0,sizeof(tmp_path));
    //socket connect

    struct sockaddr_in infotmp;
    bzero(&infotmp,sizeof(infotmp));
    infotmp.sin_family = AF_INET;

    //localhost test
    infotmp.sin_addr.s_addr = inet_addr("127.0.0.1");
    infotmp.sin_port = htons(1234);


    int errr = connect(socktmp,(struct sockaddr *)&infotmp,sizeof(infotmp));
    if(errr == -1) {
        printf("Connection error");
    }
    //printf("in thread\n");
    //pthread_mutex_lock(&lock);
    send(socktmp,request_tmp,sizeof(request_tmp),0);
    memset(request_tmp,0,sizeof(request_tmp));
    char recvmsg_tmp[1000000];
    memset(recvmsg_tmp,0,sizeof(recvmsg_tmp));
    printf("in thread2\n");
    recv(socktmp,recvmsg_tmp,sizeof(recvmsg_tmp),0);
    close(socktmp);
    printf("in thread2\n");
    printf("%s\n",recvmsg_tmp);
    //printf("!!!!!!!!!GET from server : \n%s\nEND!!!!!!\n",recvmsg_tmp);
    char content_line_tmp[1000000]= {};
    char dir_down_tmp[1000000]= {};
    char for_copy[100000]= {};
    char* delim1 = "\n";
    char* pp = NULL;
    strcpy(for_copy,recvmsg_tmp);
    pp = strtok(for_copy,delim1);
    for(int i=0; i<4; i++) {
        if(i==1) {
            //printf("try recursive:%s\n",pp);
            strcpy(content_line_tmp,pp);
            printf("try recursive111:%c\n",content_line[14]);
        }
        if(content_line_tmp[14] == 'd') {
            if(i==3) {

                strcpy(dir_down_tmp,pp);
                //printf("try recursive111:%s\n",dir_down_tmp);
            }
        }
        pp = strtok(NULL,delim1);
    }
    char file_or_dir_tmp_in_th[100000];

    pthread_t *threadyyptr[100];
    int tttt=0;
    printf("RRR\n");
    if(content_line_tmp[14] == 'd') {
        printf("in dir\n");
        char mk_path_tmp[1000000] = {};
        memset(mk_path_tmp,0,sizeof(mk_path_tmp));
        sprintf(mk_path_tmp,"output%s",tmp_path);
        printf("\noutput p : %s\n",mk_path_tmp);
        _mkdir(mk_path_tmp);
        //printf("GOGO\n");

        char* delim111 = " ";
        char* ppp = NULL;
        char* ptrr[100000];
        //int tttt=0;
        ppp = strtok(dir_down_tmp,delim111);
        while(ppp!=NULL) {
            ptrr[tttt] = ppp;
            //printf("ptrr[%d]:%s\n",tttt,ptrr[tttt]);
            tttt++;
            ppp = strtok(NULL,delim111);
        }
        int j=0;
        //char requesttmp;
        while(j<tttt) {

            memset(file_or_dir_tmp_in_th,0,sizeof(file_or_dir_tmp_in_th));
            sprintf(file_or_dir_tmp_in_th,"%s/%s",tmp_path,ptrr[j]);
            //printf("\n(th)()()file_tmp:%s\n", file_or_dir_tmp_in_th);


            memset(request_tmp,0,sizeof(request_tmp));
            sprintf(request_tmp,"GET %s/%s HTTP/1.x\r\nHOST: %s:%s\r\n\r\n",tmp_path,ptrr[j],local_input,port_input);
            //printf("\ncheck in th:\n%s\n",request_tmp);

            pthread_create(&threadyyptr[j],NULL,thread,file_or_dir_tmp_in_th);
            j++;
            //pthread_create(&(thread_poo[i]),NULL,threadpool_thread,(void*)&i);
            sleep(1);
        }

    } else if(content_line_tmp[14] == 't' || content_line_tmp[14] == 'a') {
        printf("comie in\n");

        char copyreceivee_th[100000] = {};
        strcpy(copyreceivee_th,recvmsg_tmp);

        char gett_th[100000] = {};
        char* delim15_th = "\n";
        char* ppr_th = NULL;
        int c_th=0;
        ppr_th = strtok(copyreceivee_th,delim15_th);
        while(ppr_th!=NULL) {

            if(c_th>=3) {

                //sprintf(gett,"%s\n",ppr);
                strcat(gett_th,ppr_th);
                strcat(gett_th,"\n");
                printf("try recursive:%s\n",ppr_th);
                //ppr = strtok(NULL,delim15);
            }
            ppr_th = strtok(NULL,delim15_th);
            c_th++;
        }
        printf("\ntry recursive55:\n%s\n",gett_th);


        char path_in_write_th[1000000]= {};
        sprintf(path_in_write_th,"output%s",tmp_path);
        FILE *fp = fopen(path_in_write_th,"w+");
        fprintf(fp,"%s",gett_th);
        fclose(fp);


    } else {
        close(sockfd);
    }
    for(int s=0; s<tttt; s++) {
        pthread_join(threadyyptr[s], NULL);
    }

    //pthread_mutex_unlock(&lock);
}


