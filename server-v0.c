#include <stdio.h>
#include <string.h>
#include <winsock.h>
#include<windows.h>
#define MAXLINE 1024    /* �r��w�İϪ��� */
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
SOCKET	serv_sd,muti,broa;        //sk �e
int   	cli_len,cli_sd, n,serv_len,count=-1,flag=0;//0�S�H�b��ѫ�
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
   	listen(serv_sd, 5) ; //�I�s listen() �� socket �i�J�u��ť�v���A
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
            strcpy(str,"�����ܤj�n��!�g�U�A�Q�n��L(�o)������! A.�ǵ��Y�ӤH(�ӤH) B.�j�n��(�j�a) \n");
            send(cli_sd[i], str, strlen(str)+1, 0);
            strcpy(str,"�ڭ̱N�|���z�ǹF�T���ðe�o�@�Ӻ�o�p�J�|\n");
            Sleep(50);
            send(cli_sd[i], str, strlen(str)+1, 0);
            n=recv(cli_sd[i], bigsay, 100, 0);
            ///#///////�ǰe���S�w��H(������)////////////////////////
            if(!strcmp(bigsay,"A"))
            {
                FILE *pFile3;
                pFile3 = fopen( "message.txt","a+" );//�iŪ�i�g�A�Y�ɮפ��s�b�|�s�W�@�ӷs���ɮ�
                n=recv(cli_sd[i], forwho, 50, 0);
                n=recv(cli_sd[i], saysmthin, 200, 0);
                printf("%s%s%s%s\n","���H�Q�� ",forwho," ��: ",saysmthin);
                fwrite(forwho,1,strlen(forwho),pFile3);
                fwrite(line,1,strlen(line),pFile3);
                fwrite(saysmthin,1,strlen(saysmthin),pFile3);
                fclose(pFile3);
            }
            ///#///////�ǰe���j�a(�j�n��)////////////////////////
            if(!strcmp(bigsay,"B"))
            {
                n=recv(cli_sd[i], bigsay2, 100, 0);
                sendto(broa, bigsay2, strlen(bigsay2)+1, 0,(LPSOCKADDR)&broas,sizeof(broas));
                printf("�j�n��: %s",bigsay2);
            }
        }
    ///#/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        else if (!strcmp(opti,"2"))
        {
            char manopti[2],id[8],name[50],oldid[8],oldname[50],signok[5];
            int oldid1;
            FILE *pFile;
            pFile = fopen( "member.txt","a+" );//�iŪ�i�g�A�Y�ɮפ��s�b�|�s�W�@�ӷs���ɮ�
            n=recv(cli_sd[i], manopti, 2, 0);  //��.�n�J �� ��.���U
            ///#////////�n�J�ε��U�ݨD///////////////////
            if (!strcmp(manopti,"A"))  //�n�J
            {
                printf("�|���n�D�n�J\n");
                n=recv(cli_sd[i], id, 8, 0);  //id
                n=recv(cli_sd[i], name, 50, 0);  //name
                while((oldid1=fread(oldid, sizeof(char), 4, pFile))&& (!feof(pFile)))
                {
                    oldid[oldid1]='\0';
                    oldid1=fread(oldname, sizeof(char), 4, pFile);
                    oldname[oldid1]='\0';
                    if(!strcmp(oldid,id)&&!strcmp(oldname,name))//�Yid�ۦP
                    {
                        printf("�n�J���\\\n");
                        strcpy(signok,"OK");
                        send(cli_sd[i], signok, strlen(signok)+1, 0);
                        break;
                    }
                }
                if(strcmp(signok,"OK"))
                {
                        printf("�n�J����\n");
                        strcpy(signok,"NOT");
                        send(cli_sd[i], signok, strlen(signok)+1, 0);
                }
            }
            if (!strcmp(manopti,"B"))  //���U
            {
                printf("�Τ���U��\n");
                char regi[10];
                FILE *pFile;
                pFile = fopen( "member.txt","a+" );//�iŪ�i�g�A�Y�ɮפ��s�b�|�s�W�@�ӷs���ɮ�
                n=recv(cli_sd[i], id, 8, 0);  //id
                id[n]='\0';
                printf("%s\n",id);
                strcpy(regi,"OK");
                //////id��������
                while((!feof(pFile)))
                {
                    oldid1=fread(oldid, sizeof(char), 4, pFile);
                    oldid[oldid1]='\0';
                    strcpy(regi,"OK");
                    oldid1=fread(oldname, sizeof(char), 4, pFile);
                    oldname[oldid1]='\0';
                    if(!strcmp(oldid,id))//�Yid�ۦP
                    {
                        printf("����id!\n");
                        strcpy(regi,"NO");
                        send(cli_sd[i], regi, strlen(regi)+1, 0);
                        n=recv(cli_sd[i], id, 8, 0);//����ID
                        printf("%s\n",id);
                        fclose(pFile);
                        pFile = fopen( "member.txt","a+" );
                    }
                }
                if(!strcmp(regi,"OK"))  //�|�����U��id�L����
                {
                    send(cli_sd[i], regi, strlen(regi)+1, 0);
                    n=recv(cli_sd[i], name, 50, 0);  //name
                    printf("%s\n",name);
                    fwrite(id,1,strlen(id),pFile);
                    fwrite(name,1,strlen(name),pFile);
                    fclose(pFile);
                    printf("���U����!\n");
                }
                strcpy(regi,"NO");
            }
            ///#////////�惡�|���o�e�|���u�f(�i�H�d�ӧO�ֿn�I��)///////////////////
            if(!strcmp(signok,"OK"))
            {
                char offer1[100]="�S�f !! �|���R�ΪΩ۵P�ѥ]1���_~~",offer2[100]="1���i�I��10�I�A100�I�i�I���@�ӪΪΩ۵P�ѥ]!";
                char offernum[10]="2",wel[50]="�˷R��",wel2[50]="�z�n:",wel3[100]="�����w�ǰe�s�i��ŵ��z!!(c:\\temp\\clibread.jpg) �w��쥻������ : ) ";
                strcat(wel,name); //�ݦn
                strcat(wel,wel2);
                muti=socket(AF_INET, SOCK_DGRAM, 0);
                int numid,bytes;
                numid=atoi(id);
                int multicastTTL=1;
                if (setsockopt(muti, IPPROTO_IP, IP_MULTICAST_TTL, (char *) &multicastTTL,sizeof(multicastTTL)) == SOCKET_ERROR)
                    printf("setsockopt() failed");
                serv_send.sin_family       = AF_INET;
                serv_send.sin_addr.s_addr  = inet_addr("224.1.1.2");
                serv_send.sin_port         = htons(numid);  //�ǰe�����n�J���\���|��
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
                ///�ǰe�Ϥ�
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
                printf("%s%d%s%s\n","send to ",htons(serv_send.sin_port)," success:","��Ťw�ǰe");
                ///#//////////�o�e�d��/////////////////
                char getsomsay[200],forwho1[50],sayno[5],saynum[5];
                int countsay,oldsayint;
                ///---�p�⦹�|�����X�ӯd��-----////
                FILE *pFile3;
                pFile3 = fopen( "message.txt","a+" );//�iŪ�i�g�A�Y�ɮפ��s�b�|�s�W�@�ӷs���ɮ�
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
                printf("���|���� %s%s\n",saynum," �ӯd��");  //���|�����o��h�ӯd��
                Sleep(500);
                //send(cli_sd[i], saynum, strlen(saynum)+1, 0);
                sendto(muti, saynum, strlen(saynum)+1, 0,(LPSOCKADDR)&serv_send,serv_len);
                ///---�e�X�d��-----////
                pFile3 = fopen( "message.txt","a+" );//�iŪ�i�g�A�Y�ɮפ��s�b�|�s�W�@�ӷs���ɮ�
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
                        printf("�d���w�e�X: %s\n",getsomsay);
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
                printf("�i�H�}�l���!\n");
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