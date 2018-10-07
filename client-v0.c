#include <stdio.h>
#include <string.h>
#include <winsock.h>
#include<windows.h>
#define color SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY | FOREGROUND_RED)
#define white SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE)
#define MAXLINE 1024
struct ThreadArgs
{
    int clntSock;
};
struct Thread
{
    SOCKET clntSock;
};
void *ThreadMain(void *arg);
void *Threadbroarecv(void *arg);
int key_down();
int flag=1,on=1,count=-1,cli_recv_len;
DWORD  threadID;
SOCKET        	sd1,muti,broa;
struct ThreadArgs *threadArgs;
struct Thread * thread;
struct sockaddr_in   cli,cli_recv,cli_recv1,broas,serv;
char opti[2];
char  		str[1024];
int n,cli_recv1_len,Q=0;
WSADATA 		wsadata;
int main(int argc, char** argv)
{


    WSAStartup(0x101,(LPWSADATA) &wsadata);
    sd1=socket(AF_INET, SOCK_STREAM, 0);
    cli.sin_family       = AF_INET;
    cli.sin_addr.s_addr  = inet_addr("192.168.1.106");
    cli.sin_port         = htons(5678);
    connect(sd1, (LPSOCKADDR) &cli, sizeof(cli)); // 連接至 echo server
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE);
    printf("歡迎來到肥肥愛心麵包店\n");
///#廣播/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    broa=socket(AF_INET, SOCK_DGRAM, 0);
    thread = (struct Thread *) malloc(sizeof(struct Thread));
                thread -> clntSock = broa;
    if (CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) Threadbroarecv, thread,0, (LPDWORD) &threadID) == NULL)
                    printf("thread_create() failed");
///#介面/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    while(1)
    {


        if(Q++>=1)
        {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE);
            printf("歡迎來到肥肥愛心麵包店\n");
        }

        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
        printf("需要甚麼服務呢?\n");
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        printf("1.活動專區\n2.會員專區 (登入/註冊)\n3.與愛心麵包店聯絡\n4.離開\n");
        scanf("%s",&opti);     //選擇服務
        while(strcmp(opti,"1")&&strcmp(opti,"2")&&strcmp(opti,"3")&&strcmp(opti,"4"))
        {
            color;
            printf("輸入錯誤，請重新輸入!\n");
            white;
            scanf("%s",&opti);
        }
        send(sd1, opti, strlen(opti)+1, 0); //傳送opti至 server
    ///#/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if(!strcmp(opti,"1"))//活動專區----(傳話愛心麵包活動)
        {
            char bigsay[3],forwho[50],saysmthin[200];
            color;
            n=recv(sd1, str, MAXLINE, 0);
            printf(str);
            n=recv(sd1, str, MAXLINE, 0);
            printf(str);
            white;
            scanf("%s",&bigsay); //悄悄話or大聲公
            while(strcmp(bigsay,"A")&&strcmp(bigsay,"B"))
            {
                color;
                printf("輸入錯誤，請重新輸入!\n");
                white;
                scanf("%s",&bigsay);
            }
            send(sd1, bigsay, strlen(bigsay)+1, 0);
            //printf("%s\n",bigsay);
            ///#///////傳送給特定對象(悄悄話)////////////////////////
            if(!strcmp(bigsay,"A"))
            {
                color;
                printf("想傳送給甚麼人? *(限會員)\n");
                white;
                scanf("%s",&forwho);
                send(sd1, forwho, strlen(forwho)+1, 0);
                color;
                printf("想對他說甚麼?\n");
                white;
                getchar();
                fgets(saysmthin,200,stdin);
                send(sd1, saysmthin, strlen(saysmthin)+1, 0);
                system("pause");

            }
            ///#///////傳送給大家(大聲公)////////////////////////
            if(!strcmp(bigsay,"B"))
            {
                char bigsay2[100];
                color;
                printf("請輸入你想對大家說的話:\n");
                white;
                getchar();
                fgets(bigsay2, sizeof(bigsay2), stdin);
                send(sd1, bigsay2, strlen(bigsay2)+1, 0);
                system("pause");

            }
        }

    ///#/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        else if (!strcmp(opti,"2"))
        {
            ///#////////登入及註冊需求///////////////////
            char manopti[2],id[8],name[50],signok[10];
            color;
            printf("歡迎愛心麵包店 請問是要 A.登入  B.註冊會員 呢?\n");
            white;
            scanf("%s",&manopti);
            while(strcmp(manopti,"A")&&strcmp(manopti,"B"))
            {
                color;
                printf("輸入錯誤，請重新輸入!\n");
                white;
                scanf("%s",&manopti);
            }
            send(sd1, manopti, strlen(manopti)+1, 0); //傳送manopti至 server
            if(!strcmp(manopti,"A"))//登入
            {
                color;
                printf("請輸入會員ID(四碼數字)\n");
                white;
                scanf("%s",&id);
                send(sd1, id, strlen(id)+1, 0); //傳送id至 server
                color;
                printf("請輸入會員名稱\n");
                white;
                scanf("%s",name);
                send(sd1, name, strlen(name)+1, 0); //傳送name至 server
                color;
                n=recv(sd1, signok, 10, 0);
                if(!strcmp(signok,"OK"))
                {
                    color;
                    printf("登入成功\\n");
                }
                else if (!strcmp(signok,"NOT"))
                {
                    color;
                    printf("登入失敗\n");
                }
                white;
            }
            if(!strcmp(manopti,"B"))//註冊
            {
                int ccount;
                char regi[5];
                strcpy(regi,"OK");
                ccount=0;
                do
                {
                    if((ccount>0)&&(!strcmp(regi,"NO")))
                    {
                        color;
                        printf("此id已存在\n");
                    }
                    color;
                    printf("請輸入會員ID(四碼數字)\n");
                    white;
                    scanf("%s",&id);
                    while(strlen(id)!=4)
                    {
                        color;
                        printf("輸入錯誤，請重新輸入會員ID(四碼數字)\n");
                        white;
                        scanf("%s",&id);
                    }
                    send(sd1, id, strlen(id)+1, 0); //傳送id至 server
                    ccount++;
                }while((n=recv(sd1, regi, 5, 0))&&(!strcmp(regi,"NO")));

                color;
                printf("請輸入會員名稱\n");
                white;
                scanf("%s",&name);
                send(sd1, name, strlen(name)+1, 0); //傳送name至 server
            }
            ///#////////此會員接收會員優惠///////////////////
            if(!strcmp(signok,"OK"))
            {
                int offernum;
                muti=socket(AF_INET, SOCK_DGRAM, 0);
                int multicastTTL=1;
                int numid;
                numid=atoi(id);
                cli_recv.sin_family       = AF_INET;
                cli_recv.sin_addr.s_addr  = 0;
                cli_recv.sin_port         = htons(numid);
                if(setsockopt(muti, SOL_SOCKET, SO_REUSEADDR,  (char *)&on, sizeof(on)))
                    printf("11setsockopt() failed");
                if( bind(muti, (LPSOCKADDR) &cli_recv, sizeof(cli_recv)) <0 )
                    printf("bind error!\n");
                struct ip_mreq multicastRequest;
                multicastRequest.imr_multiaddr.s_addr = inet_addr("224.1.1.2");
                multicastRequest.imr_interface.s_addr = htonl(INADDR_ANY);
                if (setsockopt(muti, IPPROTO_IP, IP_ADD_MEMBERSHIP, (void *) &multicastRequest,sizeof(multicastRequest)) < 0)
                    printf("setsockopt() failed");

                cli_recv_len=sizeof(cli_recv);
                n=recvfrom(muti, str, MAXLINE, 0,(LPSOCKADDR) &cli_recv,&cli_recv_len );//會員接收優惠訊息
                str[n]='\0';
                offernum=atoi(str);
                n=recvfrom(muti, str, MAXLINE, 0,(LPSOCKADDR) &cli_recv,&cli_recv_len );
                str[n]='\0';
                printf("%s\n",str);
                while(offernum--)
                {
                    color;
                    n=recvfrom(muti, str, MAXLINE, 0,(LPSOCKADDR) &cli_recv,&cli_recv_len );
                    str[n]='\0';
                    printf("%s\n",str);
                }
                FILE *fptr2;
                fptr2=fopen("c:\\temp\\clibread.jpg","wb");
                while(1)//接收圖片
                {
                    n=recvfrom(muti, str, MAXLINE, 0,(LPSOCKADDR)&cli_recv,&cli_recv_len);
                    fwrite(str,sizeof(char),n,fptr2); //寫檔
                    if(!strcmp(str,"OK"))break;
                }
                n=recvfrom(muti, str, MAXLINE, 0,(LPSOCKADDR) &cli_recv,&cli_recv_len );  //已接收相片
                str[n]='\0';
                printf("%s\n",str);
            }
            ///#////////此會員接收留言(活動)///////////////////.
            if(!strcmp(signok,"OK"))
            {
                char saynum[7],getsomsay[200];
                int countsay;
                n=recvfrom(muti, saynum, 7, 0,(LPSOCKADDR) &cli_recv,&cli_recv_len);
                saynum[n]='\0';
                countsay=atoi(saynum);
                while(countsay--)
                {
                    color;
                    printf("************************\n");
                    n=recvfrom(muti, getsomsay, 200, 0,(LPSOCKADDR) &cli_recv,&cli_recv_len );//接收此會員的留言
                    printf("收到留言 : %s",getsomsay);
                }
            }
            system("pause");

        }
    ///#/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        else if (!strcmp(opti,"3"))
        {
            char chat_mes[100],count[10];
            color;
            printf("歡迎愛心麵包店 請問是要 A.留言給老闆  B.與老闆對話 呢?\n");
            white;
            scanf("%s",&chat_mes);//選擇留言或聊天室
            while(strcmp(chat_mes,"A")&&strcmp(chat_mes,"B"))
            {
                color;
                printf("輸入錯誤，請重新輸入!\n");
                white;
                scanf("%s",&chat_mes);
            }
            send(sd1, chat_mes, strlen(chat_mes)+1, 0);//傳送A
            if(!strcmp(chat_mes,"A"))//留言
            {
                color;
                printf("請留言想對老闆說的話:\n");
                white;
                getchar();
                fgets(chat_mes, sizeof(chat_mes), stdin);
                send(sd1, chat_mes, strlen(chat_mes)+1, 0);//傳送留言
                color;
                printf("留言完成\n");
                white;
                system("pause");
            }
            else if(!strcmp(chat_mes,"B"))//聊天室
            {
                color;
                printf("可以開始對話!(單行輸入NNN即可離開)\n");
                white;
                n = recv(sd1, count, 10,0);
                int num;
                num=atoi(count);


                char c;
                getchar();
                threadArgs = (struct ThreadArgs *) malloc(sizeof(struct ThreadArgs));
                threadArgs -> clntSock = sd1;
                if (CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) ThreadMain, threadArgs,0, (LPDWORD) &threadID) == NULL)
                    printf("thread_create() failed");
                while(1)
                {
                    if(num>0)
                    {
                        color;
                        printf("老闆忙線中，請稍後再試!\n");
                        flag=0;
                        white;
                        system("pause");
                        break;

                    }
                    white;
                    fgets(chat_mes, sizeof(chat_mes), stdin);//傳送訊息
                    if(!strcmp(chat_mes,"NNN\n"))
                    {
                        flag=0;
                        send(sd1, chat_mes, strlen(chat_mes)+1, 0);
                        break;
                    }

                    send(sd1, chat_mes, strlen(chat_mes)+1, 0);

                }


            }

        }
        else if (!strcmp(opti,"4"))
        {
            break;
        }
        system("CLS");

    }

    closesocket(sd1);
    WSACleanup();
    white;
    return 0;
}


void *Threadbroarecv(void *threadArgs)//廣播接收
{
    SOCKET clntSock;
    clntSock = ((struct Thread *) threadArgs) -> clntSock;
    free(threadArgs);
	char echoBuffer[1000];
    int recvMsgSize,on=1;
    clntSock=socket(AF_INET, SOCK_DGRAM, 0);
    char broadcast = 'a';
    if(setsockopt(clntSock, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast))<0)
            printf("setsockopt() error!\n");
    while(1)
    {
        setsockopt(clntSock, SOL_SOCKET, SO_REUSEADDR,  (char *)&on, sizeof(on));
        broas.sin_family       = AF_INET;
        broas.sin_addr.s_addr  = 0;
        broas.sin_port         = htons(1234);

        bind(clntSock, (LPSOCKADDR) &broas, sizeof(broas));

        int serv_len=sizeof(serv);
        Sleep(50);
        n=recvfrom(clntSock, str, MAXLINE, 0,(LPSOCKADDR) &serv,&serv_len); //由echo server接收
        str[n]='\0';
        color;
        printf("大聲公 :%s",str);
    }

}
void *ThreadMain(void *threadArgs)//聊天室
{
    int clntSock;
    clntSock = ((struct ThreadArgs *) threadArgs) -> clntSock;
    free(threadArgs);
	char echoBuffer[1000];
    int recvMsgSize;
    while (1)
    {
        if(flag==0)
            break;
        if ((recvMsgSize = recv(clntSock, echoBuffer, 1000,0)) < 0)
            printf("recv() failed");
        color;
        printf("Message from Boss:%s",echoBuffer);
        white;

    }

    closesocket(clntSock);

    return (NULL);
}


