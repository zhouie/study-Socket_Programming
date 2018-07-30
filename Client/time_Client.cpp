#include<WINSOCK2.H>
#include<STDIO.H>
#include<iostream>
#include<cstring>

#pragma comment(lib, "ws2_32.lib")

#define MAXSIZE 255
using namespace std;
int main()
{
    WORD sockVersion = MAKEWORD(2, 2);
    WSADATA wcdata;
    if(WSAStartup(sockVersion, &wcdata)!=0)
    {
        cout<<"Couldn't find a useable winsock.dll"<<endl;
        exit(-1);
    }
    SOCKET sclient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(sclient == INVALID_SOCKET)
    {
        cout<<"Create socket failed!"<<endl;
        exit(-1);
    }

    sockaddr_in serAddr;
    serAddr.sin_family = AF_INET;
    serAddr.sin_port = htons(8888);
    serAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    if(connect(sclient, (sockaddr *)&serAddr, sizeof(serAddr))<0)
    {
        cout<<"Connect error !"<<endl;
        exit(-1);
    }
	cout<<"请输入请求："<<endl;
    string data;
    cin>>data;
    const char * sendData;
    sendData = data.c_str();   //string -> const char*
    send(sclient, sendData, strlen(sendData), 0);

    char recData[MAXSIZE];
    int ret = recv(sclient, recData, MAXSIZE, 0);
    if(ret<0)
    {
        cout<<"Recieve data failed!"<<endl;
        exit(-1);
    }
    recData[ret]=0x00;
    printf("%s\n",recData);
    system("pause");
    closesocket(sclient);
    WSACleanup();
    return 0;

}

