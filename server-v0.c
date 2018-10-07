#include <stdio.h>
#include <string.h>
#include <winsock.h>
#include<windows.h>
#define MAXLINE 1024    /* 字串緩衝區長度 */
void key_down();

struct ThreadArgs // chat struct
{
    int clntSock;
};

struct mainthread//server thread
{
    int clntSock;
};

void *ThreadMain(void *arg);//chat thread
void *Threadaccept(void *arg);//one to many thread
char chatf[5]="no",str2[100],line[4]="\n";
DWORD  threadID;
struct ThreadArgs *threadArgs;
struct mainthread *serverArgs;
SOCKET	serv_sd,muti,broa;        //sk 送
int   	cli_len,cli_sd, n,serv_len,count=-1,flag=0;//0沒人在聊天室
struct 	sockaddr_in   	serv, cli,serv_send,broas;
int main()
{
  	WSADATA wsadata;

    WSAStartup(0x101, &wsadata);

  	serv_sd=socket(AF_INET, SOCK_STREAM, 0);

   	serv.sin_family      = AF_INET;
   	serv.sin_addr.s_addr = 0;
   	serv.sin_port        = htons(5678);

    bind(serv_sd, (LPSOCKADDR) &serv, sizeof(serv));
   	listen(serv_sd, 5) ; //呼叫 listen() 使 socket 進入「監聽」狀態
    cli_len = sizeof(cli);

    printf("Waiting for client\n");
    while(1)
    {
        cli_sd=accept(serv_sd, (LPSOCKADDR) &cli, &cli_len);
        printf("Client connect!\n");
        broa=socket(AF_INET, SOCK_DGRAM, 0);
        char broadcast = 'a';
        if(setsockopt(broa, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast))<0)
        printf("setsockopt() error!\n");

        broas.sin_family       = AF_INET;
        broas.sin_addr.s_addr  = inet_addr("255.255.255.255");
        broas.sin_port         = htons(1234);

        serverArgs = (struct mainthread *) malloc(sizeof(struct mainthread));
        serverArgs -> clntSock = cli_sd;

        if (CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) Threadaccept, serverArgs,0, (LPDWORD) &threadID) == NULL)
            printf("thread_create() failed");
    }
   	closesocket(serv_sd);
   	closesocket(cli_sd);
   	WSACleanup();

   	return 0;
}
int W=0;
void *Threadaccept(void *serverArgs) // server thread
{
    int cli_sd[100],len,n,i=0;

    char  	str[MAXLINE],opti[5];
    cli_sd[i] = ((struct mainthread *) serverArgs) -> clntSock;
    free(serverArgs);

	char echoBuffer[1000];
    int recvMsgSize;
    while(1)
    {
        n=recv(cli_sd[i], opti, 5, 0);
    ///#/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if(!strcmp(opti,"1"))
        {
            char bigsay[3],bigsay2[100],forwho[50],saysmthin[200];
            strcpy(str,"悄悄話大聲說!寫下你想要對他(她)說的話! A.傳給某個人(個人) B.大聲公(大家) \n");
            send(cli_sd[i], str, strlen(str)+1, 0);
            strcpy(str,"我們將會幫您傳達訊息並送她一個精緻小蛋糕\n");
            Sleep(50);
            send(cli_sd[i], str, strlen(str)+1, 0);
            n=recv(cli_sd[i], bigsay, 100, 0);
            ///#///////傳送給特定對象(悄悄話)////////////////////////
            if(!strcmp(bigsay,"A"))
            {
                FILE *pFile3;
                pFile3 = fopen( "message.txt","a+" );//可讀可寫，若檔案不存在會新增一個新的檔案
                n=recv(cli_sd[i], forwho, 50, 0);
                n=recv(cli_sd[i], saysmthin, 200, 0);
                printf("%s%s%s%s\n","有人想對 ",forwho," 說: ",saysmthin);
                fwrite(forwho,1,strlen(forwho),pFile3);
                fwrite(line,1,strlen(line),pFile3);
                fwrite(saysmthin,1,strlen(saysmthin),pFile3);
                fclose(pFile3);
            }
            ///#///////傳送給大家(大聲公)////////////////////////
            if(!strcmp(bigsay,"B"))
            {
                n=recv(cli_sd[i], bigsay2, 100, 0);
                sendto(broa, bigsay2, strlen(bigsay2)+1, 0,(LPSOCKADDR)&broas,sizeof(broas));
                printf("大聲公: %s",bigsay2);
            }
        }
    ///#/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        else if (!strcmp(opti,"2"))
        {
            char manopti[2],id[8],name[50],oldid[8],oldname[50],signok[5];
            int oldid1;
            FILE *pFile;
            pFile = fopen( "member.txt","a+" );//可讀可寫，若檔案不存在會新增一個新的檔案
            n=recv(cli_sd[i], manopti, 2, 0);  //Ａ.登入 或 Ｂ.註冊
            ///#////////登入及註冊需求///////////////////
            if (!strcmp(manopti,"A"))  //登入
            {
                printf("會員要求登入\n");
                n=recv(cli_sd[i], id, 8, 0);  //id
                n=recv(cli_sd[i], name, 50, 0);  //name
                while((oldid1=fread(oldid, sizeof(char), 4, pFile))&& (!feof(pFile)))
                {
                    oldid[oldid1]='\0';
                    oldid1=fread(oldname, sizeof(char), 4, pFile);
                    oldname[oldid1]='\0';
                    if(!strcmp(oldid,id)&&!strcmp(oldname,name))//若id相同
                    {
                        printf("登入成功\\n");
                        strcpy(signok,"OK");
                        send(cli_sd[i], signok, strlen(signok)+1, 0);
                        break;
                    }
                }
                if(strcmp(signok,"OK"))
                {
                        printf("登入失敗\n");
                        strcpy(signok,"NOT");
                        send(cli_sd[i], signok, strlen(signok)+1, 0);
                }
            }
            if (!strcmp(manopti,"B"))  //註冊
            {
                printf("用戶註冊中\n");
                char regi[10];
                FILE *pFile;
                pFile = fopen( "member.txt","a+" );//可讀可寫，若檔案不存在會新增一個新的檔案
                n=recv(cli_sd[i], id, 8, 0);  //id
                id[n]='\0';
                printf("%s\n",id);
                strcpy(regi,"OK");
                //////id重複檢驗
                while((!feof(pFile)))
                {
                    oldid1=fread(oldid, sizeof(char), 4, pFile);
                    oldid[oldid1]='\0';
                    strcpy(regi,"OK");
                    oldid1=fread(oldname, sizeof(char), 4, pFile);
                    oldname[oldid1]='\0';
                    if(!strcmp(oldid,id))//若id相同
                    {
                        printf("重複id!\n");
                        strcpy(regi,"NO");
                        send(cli_sd[i], regi, strlen(regi)+1, 0);
                        n=recv(cli_sd[i], id, 8, 0);//收到ID
                        printf("%s\n",id);
                        fclose(pFile);
                        pFile = fopen( "member.txt","a+" );
                    }
                }
                if(!strcmp(regi,"OK"))  //會員註冊的id無重複
                {
                    send(cli_sd[i], regi, strlen(regi)+1, 0);
                    n=recv(cli_sd[i], name, 50, 0);  //name
                    printf("%s\n",name);
                    fwrite(id,1,strlen(id),pFile);
                    fwrite(name,1,strlen(name),pFile);
                    fclose(pFile);
                    printf("註冊完成!\n");
                }
                strcpy(regi,"NO");
            }
            ///#////////對此會員發送會員優惠(可以查個別累積點數)///////////////////
            if(!strcmp(signok,"OK"))
            {
                char offer1[100]="特惠 !! 會員買肥肥招牌麵包1元起~~",offer2[100]="1元可兌換10點，100點可兌換一個肥肥招牌麵包!";
                char offernum[10]="2",wel[50]="親愛的",wel2[50]="您好:",wel3[100]="本店已傳送廣告文宣給您!!(c:\\temp\\clibread.jpg) 歡迎到本店選購 : ) ";
                strcat(wel,name); //問好
                strcat(wel,wel2);
                muti=socket(AF_INET, SOCK_DGRAM, 0);
                int numid,bytes;
                numid=atoi(id);
                int multicastTTL=1;
                if (setsockopt(muti, IPPROTO_IP, IP_MULTICAST_TTL, (char *) &multicastTTL,sizeof(multicastTTL)) == SOCKET_ERROR)
                    printf("setsockopt() failed");
                serv_send.sin_family       = AF_INET;
                serv_send.sin_addr.s_addr  = inet_addr("224.1.1.2");
                serv_send.sin_port         = htons(numid);  //傳送給此登入成功的會員
                serv_len = sizeof(serv_send);
                Sleep(700);
                sendto(muti, offernum, strlen(offernum)+1, 0,(LPSOCKADDR)&serv_send,serv_len);
                Sleep(700);
                sendto(muti, wel, strlen(wel)+1, 0,(LPSOCKADDR)&serv_send,serv_len);
                //printf("%s%d%s%s\n","send to ",htons(serv_send.sin_port)," success:",wel);
                Sleep(700);
                sendto(muti, offer1, strlen(offer1)+1, 0,(LPSOCKADDR)&serv_send,serv_len);
                //printf("%s%d%s%s\n","send to ",htons(serv_send.sin_port)," success:",offer1);
                Sleep(700);
                sendto(muti, offer2, strlen(offer2)+1, 0,(LPSOCKADDR)&serv_send,serv_len);
                //printf("%s%d%s%s\n","send to ",htons(serv_send.sin_port)," success:",offer2);
                ///傳送圖片
                FILE *fptr2;
                fptr2=fopen("c:\\temp\\bread.jpg","rb");
                if(fptr2!=NULL)
                {
                    while(!feof(fptr2))
                    {
                        bytes=fread(str,sizeof(char),MAXLINE,fptr2);
                        sendto(muti, str,bytes,0,(LPSOCKADDR)&serv_send, serv_len);
                    }
                    char finish[5]="OK";
                    sendto(muti, finish, strlen(finish)+1, 0,(LPSOCKADDR)&serv_send,serv_len);
                }
                sendto(muti, wel3, strlen(wel3)+1, 0,(LPSOCKADDR)&serv_send,serv_len);
                printf("%s%d%s%s\n","send to ",htons(serv_send.sin_port)," success:","文宣已傳送");
                ///#//////////發送留言/////////////////
                char getsomsay[200],forwho1[50],sayno[5],saynum[5];
                int countsay,oldsayint;
                ///---計算此會員有幾個留言-----////
                FILE *pFile3;
                pFile3 = fopen( "message.txt","a+" );//可讀可寫，若檔案不存在會新增一個新的檔案
                countsay=0;
                strcat(name,"\n");
                while((!feof(pFile3)))
                {
                    fgets(forwho1,50,pFile3);
                    if(!strcmp(forwho1,"\n"))break;
                    fgets(getsomsay,200,pFile3);
                    if(!strcmp(name,forwho1))
                    {
                        countsay=countsay+1;
                    }
                    strcpy(forwho1,"");
                }
                fclose(pFile3);
                sprintf(saynum, "%d", countsay);
                printf("此會員有 %s%s\n",saynum," 個留言");  //此會員有這麼多個留言
                Sleep(500);
                //send(cli_sd[i], saynum, strlen(saynum)+1, 0);
                sendto(muti, saynum, strlen(saynum)+1, 0,(LPSOCKADDR)&serv_send,serv_len);
                ///---送出留言-----////
                pFile3 = fopen( "message.txt","a+" );//可讀可寫，若檔案不存在會新增一個新的檔案
                countsay=0;
                while((!feof(pFile3)))
                {
                    fgets(forwho1,50,pFile3);
                    if(!strcmp(forwho1,"\n"))break;
                    fgets(getsomsay,200,pFile3);
                    if(!strcmp(name,forwho1))
                    {
                        Sleep(500);
                        //send(cli_sd[i], getsomsay, strlen(getsomsay)+1, 0);
                        sendto(muti, getsomsay, strlen(getsomsay)+1, 0,(LPSOCKADDR)&serv_send,serv_len);
                        printf("留言已送出: %s\n",getsomsay);
                        //countsay=countsay+1;
                    }
                    strcpy(forwho1,"");
                }
            }
            strcpy(signok,"NO");
        }
    ///#/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        else if (!strcmp(opti,"3"))//chat room
        {
            char chatopt[5],mes[200];
            n=recv(cli_sd[i], chatopt, 5, 0); //recv option
            if(!strcmp(chatopt,"A"))
            {
                n=recv(cli_sd[i],mes, 200, 0);//recv message
                printf("New message from client:%s\n",mes);
            }
            else if(!strcmp(chatopt,"B"))
            {
                printf("可以開始對話!\n");
                count++;
                sprintf(mes,"%d",count);
                if(count>0)
                    flag=1;
                send(cli_sd[i], mes, strlen(mes)+1, 0);
                char mes[100];
                threadArgs = (struct ThreadArgs *) malloc(sizeof(struct ThreadArgs));
                threadArgs -> clntSock = cli_sd[i];
                if (CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) ThreadMain, threadArgs,0, (LPDWORD) &threadID) == NULL)
                    printf("thread_create() failed");
                while(1)
                {
                    fgets(mes, sizeof(mes), stdin);
                    send(cli_sd[i], mes, strlen(mes)+1, 0);
                }
            }
        }
        else if(!strcmp(opti,"4"))
        {

            break;
        }
    }
    i++;
    closesocket(cli_sd[i]);
    return 0;
}

void *ThreadMain(void *threadArgs) // chat thread
{
    int clntSock;
    clntSock = ((struct ThreadArgs *) threadArgs) -> clntSock;
    free(threadArgs);

	char echoBuffer[1000];
    int recvMsgSize;

    while (1)
    {
        if(flag==1)
        {
            break;
        }
        if ((recvMsgSize = recv(clntSock, echoBuffer, 1000,0)) < 0)
            printf("recv() failed");
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY | FOREGROUND_RED);
        printf("Message from Client:%s",echoBuffer);

        if(!strcmp(echoBuffer,"NNN\n"))
        {
            count--;
            printf("Client exit!\n");
            break;
        }
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    }
    closesocket(clntSock);
    return 0;
}
