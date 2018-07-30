#include<WINSOCK2.H>
#include<STDIO.H>
#include<iostream>
#include<string>
#include<cstring>
#include<time.h>

#pragma comment(lib,"ws2_32.lib")

#define MAXSIZE 255

using namespace std;
int main(int argc, char* argv[])
{
    //初始化WSA
    WORD sockVersion = MAKEWORD(2,2);
    WSADATA wsaData;
    if(WSAStartup(sockVersion, &wsaData)!=0)
    {
        cout<<"Couldn't find a useable winsock.dll"<<endl;
        exit(-1);
    }

    //创建套接字
    SOCKET msock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(msock == INVALID_SOCKET)
    {
        cout<<"Create socket failed!"<<endl;
        exit(-1);
    }

    //绑定IP和端口
    sockaddr_in servaddr;
    ZeroMemory(&servaddr,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(8888);
    servaddr.sin_addr.s_addr = INADDR_ANY;

    if(bind(msock, (struct sockaddr*)&servaddr, sizeof(struct sockaddr_in))<0)
    {
        cout<<"Bind port failed!"<<endl;
        exit(-1);
    }

    //开始监听
    listen(msock, 20);

    //循环接收数据
    while (true)
    {
        //printf("\n等待下一个连接...\n");
        sockaddr_in clientaddr;
        ZeroMemory(&clientaddr,sizeof(clientaddr));
        int len = sizeof(clientaddr);
        char revData[MAXSIZE];
        SOCKET ssock = accept(msock, (SOCKADDR *)&clientaddr, &len);
        if(ssock == INVALID_SOCKET)
        {
            printf("Accept error !");
            continue;
        }
        printf("接受到一个连接,来自：%s! \r\n", inet_ntoa(clientaddr.sin_addr));

        //接收数据
        int ret = recv(ssock, revData, MAXSIZE, 0);
        if(ret < 0)
        {
            cout<<"Recieve Data failed!"<<endl;
            exit(-1);
        }

        //处理并发送数据
        string str=revData;
        if(str.find("daytime")!=str.npos)
        {
            time_t timep;

            time(&timep); //获取time_t类型当前时间
            char * sendData=ctime(&timep);
            send(ssock, sendData, strlen(sendData), 0);
            cout<<"已发送给客户端当前时间!"<<endl;
        }
        else
        {
            char *sendData="服务器无法处理该请求!";
            send(ssock, sendData, strlen(sendData), 0);
            cout<<"未发现daytime子串!"<<endl;
        }
        closesocket(ssock);
    }

    closesocket(msock);
    WSACleanup();
    return 0;
}

